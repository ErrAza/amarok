/****************************************************************************************
 * Copyright (c) 2008 Maximilian Kossick <maximilian.kossick@googlemail.com>            *
 *                                                                                      *
 * This program is free software; you can redistribute it and/or modify it under        *
 * the terms of the GNU General Public License as published by the Free Software        *
 * Foundation; either version 2 of the License, or (at your option) any later           *
 * version.                                                                             *
 *                                                                                      *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY      *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.             *
 *                                                                                      *
 * You should have received a copy of the GNU General Public License along with         *
 * this program.  If not, see <http://www.gnu.org/licenses/>.                           *
 ****************************************************************************************/

#ifndef MEMORYFILTER_H
#define MEMORYFILTER_H

#include "amarok_export.h"
#include "core/meta/Meta.h"
#include "core/collections/QueryMaker.h"

#include <QList>
#include <QRegExp>
#include <QString>

class MemoryFilter;

namespace FilterFactory
{
    MemoryFilter* filter( qint64 value, const QString &filter, bool matchBegin, bool matchEnd );
    MemoryFilter* numberFilter( qint64 value, qint64 filter, Collections::QueryMaker::NumberComparison compare );
}

class AMAROK_EXPORT MemoryFilter
{
    public:
        MemoryFilter();
        virtual ~MemoryFilter();
        virtual bool filterMatches( Meta::TrackPtr track ) const = 0;
};

class AMAROK_EXPORT ContainerMemoryFilter : public MemoryFilter
{
    public:
        ContainerMemoryFilter();
        virtual ~ContainerMemoryFilter();
        void addFilter( MemoryFilter *filter );
    protected:
        QList<MemoryFilter*> m_filters;
};

class AMAROK_EXPORT AndContainerMemoryFilter : public ContainerMemoryFilter
{
    public:
        AndContainerMemoryFilter();
        virtual ~AndContainerMemoryFilter();
        virtual bool filterMatches( Meta::TrackPtr track ) const;
};

class AMAROK_EXPORT OrContainerMemoryFilter : public ContainerMemoryFilter
{
    public:
        OrContainerMemoryFilter();
        virtual ~OrContainerMemoryFilter();
        virtual bool filterMatches( Meta::TrackPtr track ) const;
};

class AMAROK_EXPORT NegateMemoryFilter : public MemoryFilter
{
    public:
        NegateMemoryFilter( MemoryFilter *filter );
        virtual ~NegateMemoryFilter();
        virtual bool filterMatches( Meta::TrackPtr track ) const;
    private:
        MemoryFilter *m_filter;
};

class AMAROK_EXPORT StringMemoryFilter : public MemoryFilter
{
    public:
        StringMemoryFilter();
        virtual ~StringMemoryFilter();
        virtual bool filterMatches( Meta::TrackPtr track ) const;

        void setFilter( const QString &filter, bool matchBegin, bool matchEnd );
    protected:
        virtual QString value( Meta::TrackPtr track ) const = 0;

    private:
        QString m_filter;
        bool m_matchBegin;
        bool m_matchEnd;
};


class AMAROK_EXPORT NumberMemoryFilter : public MemoryFilter
{
    public:
        NumberMemoryFilter();
        virtual ~NumberMemoryFilter();
        void setFilter( qint64 filter, Collections::QueryMaker::NumberComparison compare );
        bool filterMatches( Meta::TrackPtr track ) const;
    protected:
        virtual qint64 value( Meta::TrackPtr track ) const = 0;
    private:
        qint64 m_filter;
        Collections::QueryMaker::NumberComparison m_compare;
};


class AMAROK_EXPORT LabelFilter : public MemoryFilter
{
public:
    LabelFilter( const QString &filter, bool matchBegin, bool matchEnd );
    virtual ~ LabelFilter();
    bool filterMatches( Meta::TrackPtr track ) const;

private:
    QRegExp m_expression;
};

#endif
