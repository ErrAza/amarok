/***************************************************************************
 *   Copyright (c) 2009  Nikolaj Hald Nielsen <nhnFreespirit@gmail.com>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/
 
#include "TimecodeTrackProvider.h"

#include "TimecodeMeta.h"

#include "Debug.h"

#include "QRegExp"

TimecodeTrackProvider::TimecodeTrackProvider()
{
    DEBUG_BLOCK
}


TimecodeTrackProvider::~TimecodeTrackProvider()
{
}

bool TimecodeTrackProvider::possiblyContainsTrack( const KUrl & url ) const
{
    DEBUG_BLOCK;
    
    QString urlString = url.url();
    debug() << "got url: " << urlString;
    debug() << "contains ':\\d+-\\d+$' " << urlString.contains( QRegExp(":\\d+-\\d+$") );
    return urlString.contains( QRegExp(":\\d+-\\d+$") );
}

Meta::TrackPtr TimecodeTrackProvider::trackForUrl( const KUrl & url )
{
    DEBUG_BLOCK;
    
    QString urlString = url.url();
    debug() << "got url: " << urlString;

    QRegExp rx;
    rx.setPattern( "^(.+):(\\d+)-(\\d+)$" );
    if (rx.indexIn( urlString ) != -1) {
        QString baseUrl = rx.cap(1);
        int start = rx.cap(2).toInt();
        int end = rx.cap(3).toInt();

        debug() << "Base: " << baseUrl;
        debug() << "Start: " << start;
        debug() << "End: " << end;

        Meta::TimecodeTrack * track = new Meta::TimecodeTrack( "TimecodeTrack", baseUrl, start * 1000, end * 1000 );
        return Meta::TrackPtr( track );

    }
    else
        return Meta::TrackPtr();
}

