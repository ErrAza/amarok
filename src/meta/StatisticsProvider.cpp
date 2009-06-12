/*
 *  Copyright 2009 Maximilian Kossick <maximilian.kossick@googlemail.com>
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

#include "StatisticsProvider.h"

#include "Amarok.h"

Meta::StatisticsProvider::StatisticsProvider()
        : m_score( 0.0 )
        , m_rating( 0 )
        , m_playCount( 0 )
{
}

Meta::StatisticsProvider::~StatisticsProvider()
{
}

void
Meta::StatisticsProvider::played( double playedFraction )
{
    m_lastPlayed = QDateTime::currentDateTime();
    if( !m_firstPlayed.isNull() )
    {
        m_firstPlayed = QDateTime::currentDateTime();
    }
    m_playCount++;
    m_score = Amarok::computeScore( m_score, m_playCount, playedFraction );
    save();
}

QDateTime
Meta::StatisticsProvider::firstPlayed() const
{
    return m_firstPlayed;
}

QDateTime
Meta::StatisticsProvider::lastPlayed() const
{
    return m_lastPlayed;
}

int
Meta::StatisticsProvider::playCount() const
{
    return m_playCount;
}

int
Meta::StatisticsProvider::rating() const
{
    return m_rating;
}

double
Meta::StatisticsProvider::score() const
{
    return m_score;
}

void
Meta::StatisticsProvider::setRating( int newRating )
{
    m_rating = newRating;
    save();
}

void
Meta::StatisticsProvider::setScore( double newScore )
{
    m_score = newScore;
    save();
}
