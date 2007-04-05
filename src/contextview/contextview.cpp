/***************************************************************************
 * copyright            : (C) 2007 Seb Ruiz <ruiz@kde.org>                 *
 **************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "albumbox.h"
#include "cloudbox.h"
#include "collectiondb.h"
#include "contextbox.h"
#include "contextview.h"
#include "fadingimageitem.h"


#include <kstandarddirs.h>

#include <math.h> // scaleView()
#include <QBrush>
#include <QColor>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QWheelEvent>

using namespace Context;

ContextView *ContextView::s_instance = 0;

ContextView::ContextView()
    : QGraphicsView()
{
    s_instance = this; // we are a singleton class

    m_contextScene = new QGraphicsScene( this );
    m_contextScene->setItemIndexMethod( QGraphicsScene::BspTreeIndex ); //mainly static, so let's keep an index

    QColor color = palette().highlight();
    m_contextScene->setBackgroundBrush( color );

    setRenderHints( QPainter::Antialiasing );
    setCacheMode( QGraphicsView::CacheBackground ); // this won't be changing regularly
    setScene( m_contextScene );

    showHome();
}

#include "debug.h"

void ContextView::showHome()
{

//     ContextBox *welcomeBox = new ContextBox( 0, m_contextScene );
//     welcomeBox->setTitle( "Hooray, welcome to Amarok::ContextView!" );
//
//     m_contextScene->addItem( welcomeBox );
//
//     welcomeBox->setPos( 2, 2 );

 /*   AlbumBox *album = new AlbumBox( 0, m_contextScene );
    const QString &cover = CollectionDB::instance()->albumImage( "3 Doors Down", "The Better Life", false, 50 );
    debug() << "cover: " << cover ;
    album->addAlbumInfo( cover, "3 Doors Down - The Better Life" );
    m_contextScene->addItem( album );
   */

   FadingImageItem * fadeingImage = new FadingImageItem (QPixmap( KStandardDirs::locate("data", "amarok/images/splash_screen.jpg" ) ) );


   QColor color = palette().highlight();
   fadeingImage->setFadeColor( color );
   fadeingImage->setTargetAlpha( 200 );


   m_contextScene->addItem(fadeingImage);

   fadeingImage->startFading();

}

void ContextView::scaleView( qreal factor )
{
    qreal scaleF = matrix().scale( factor, factor).mapRect(QRectF(0, 0, 1, 1)).width();
    if( scaleF < 0.07 || scaleF > 100 )
         return;

    scale( factor, factor );
}

void ContextView::wheelEvent( QWheelEvent *event )
{
     scaleView( pow( (double)2, -event->delta() / 240.0) );
}

void ContextView::clear()
{
    //remove amd delete all elements from scene

    QList<QGraphicsItem *> sceneItems = m_contextScene->items();

    foreach ( QGraphicsItem * item, sceneItems ) {
        if ( item->scene() == m_contextScene ) { 
            m_contextScene->removeItem( item );
            delete item;
        }
    }

}

void ContextView::addContextBox( ContextBox * newBox )
{
    m_contextScene->addItem( newBox );
}

