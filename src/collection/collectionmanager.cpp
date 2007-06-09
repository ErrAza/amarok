/*
 *  Copyright (c) 2007 Maximilian Kossick <maximilian.kossick@googlemail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#define DEBUG_PREFIX "CollectionManager"

#include "collectionmanager.h"

#include "debug.h"

#include "collection.h"
#include "metaquerybuilder.h"
#include "meta/lastfm/LastFmMeta.h"
#include "pluginmanager.h"
#include "SqlStorage.h"

#include <QtAlgorithms>
#include <QtGlobal>
#include <QList>
#include <QTimer>

#include <kservice.h>

struct CollectionManager::Private
{
    QList<Collection*> collections;
    QList<CollectionFactory*> factories;
    SqlStorage *sqlDatabase;
    QList<Collection*> unmanagedCollections;
    QList<Collection*> managedCollections;
};

CollectionManager::CollectionManager()
    : QObject()
    , d( new Private )
{
    init();
}

CollectionManager::~CollectionManager()
{
    d->collections.clear();
    d->unmanagedCollections.clear();
    qDeleteAll( d->managedCollections );

    foreach( CollectionFactory *fac, d->factories )
        PluginManager::unload( fac );

    delete d;
}

void
CollectionManager::init()
{
    DEBUG_BLOCK

    d->sqlDatabase = 0;
    KService::List plugins = PluginManager::query( "[X-KDE-Amarok-plugintype] == 'collection'" );
    debug() << "Received [" << QString::number( plugins.count() ) << "] collection plugin offers" << endl;
    foreach( KService::Ptr service, plugins )
    {
        Amarok::Plugin *plugin = PluginManager::createFromService( service );
        if ( plugin )
        {
            CollectionFactory* factory = dynamic_cast<CollectionFactory*>( plugin );
            if ( factory )
            {
                connect( factory, SIGNAL( newCollection( Collection* ) ), this, SLOT( slotNewCollection( Collection* ) ) );
                d->factories.append( factory );
                factory->init();
            }
            else
            {
                debug() << "Plugin has wrong factory class" << endl;
                continue;
            }
        }
    }

}

CollectionManager *
CollectionManager::instance( )
{
    static CollectionManager instance;
    return &instance;
}

void
CollectionManager::startFullScan()
{
    foreach( Collection *coll, d->collections )
    {
        coll->startFullScan();
    }
}

QueryMaker*
CollectionManager::queryMaker()
{
    return new MetaQueryBuilder( d->collections );
}

void
CollectionManager::slotNewCollection( Collection* newCollection )
{
    if( !newCollection )
    {
        debug() << "Warning, newCollection in slotNewCollection is 0" << endl;
        return;
    }
    debug() << "New collection with collectionId: " << newCollection->collectionId() << endl;
    d->collections.append( newCollection );
    d->managedCollections.append( newCollection );
    connect( newCollection, SIGNAL( remove() ), SLOT( slotRemoveCollection() ), Qt::QueuedConnection );
    SqlStorage *sqlCollection = dynamic_cast<SqlStorage*>( newCollection );
    if( sqlCollection )
    {
        if( d->sqlDatabase )
        {
            if( d->sqlDatabase->sqlDatabasePriority() < sqlCollection->sqlDatabasePriority() )
                d->sqlDatabase = sqlCollection;
        }
        else
            d->sqlDatabase = sqlCollection;
    }
    emit collectionAdded( newCollection );
}

void
CollectionManager::slotRemoveCollection()
{
    Collection* collection = dynamic_cast<Collection*>( sender() );
    if( collection )
    {
        d->collections.removeAll( collection );
        d->managedCollections.removeAll( collection );
        SqlStorage *sqlDb = dynamic_cast<SqlStorage*>( collection );
        if( sqlDb && sqlDb == d->sqlDatabase )
        {
            SqlStorage *newSqlDatabase = 0;
            foreach( Collection* tmp, d->collections )
            {
                SqlStorage *sqlDb = dynamic_cast<SqlStorage*>( tmp );
                if( sqlDb )
                {
                    if( newSqlDatabase )
                    {
                        if( newSqlDatabase->sqlDatabasePriority() < sqlDb->sqlDatabasePriority() )
                            newSqlDatabase = sqlDb;
                    }
                    else
                        newSqlDatabase = sqlDb;
                }
            }
            d->sqlDatabase = newSqlDatabase;
        }
        emit collectionRemoved( collection->collectionId() );
        QTimer::singleShot( 0, collection, SLOT( deleteLater() ) );
    }
}

QList<Collection*>
CollectionManager::collections()
{
    return d->collections;
}

SqlStorage*
CollectionManager::sqlStorage() const
{
    return d->sqlDatabase;
}

Meta::TrackPtr
CollectionManager::trackForUrl( const KUrl &url )
{
    //TODO method stub
    //check all collections
    //might be a podcast, in that case we'll have additional meta information
    //might be a lastfm track, another stream
    //or a file which is not in any collection
    debug() << "track for url: " << url.url() << endl;
    //check lastfm track
    if( url.protocol() == "lastfm" )
        return Meta::TrackPtr( new LastFm::Track( url.url() ) );

    foreach( Collection *coll, d->collections )
    {
        if( coll->possiblyContainsTrack( url ) )
        {
            Meta::TrackPtr track = coll->trackForUrl( url );
            if( track )
                return track;
        }
    }

    return Meta::TrackPtr( 0 );
}

void
CollectionManager::addUnmanagedCollection( Collection *newCollection )
{
    if( newCollection && d->collections.indexOf( newCollection ) == -1 )
    {
        d->unmanagedCollections.append( newCollection );
        d->collections.append( newCollection );
        emit collectionAdded( newCollection );
    }
}

void
CollectionManager::removeUnmanagedCollection( Collection *collection )
{
    //do not remove it from collection if it is not in unmanagedCollections
    if( collection && d->unmanagedCollections.removeAll( collection ) )
    {
        d->collections.removeAll( collection );
        emit collectionRemoved( collection->collectionId() );
    }
}

#include "collectionmanager.moc"
