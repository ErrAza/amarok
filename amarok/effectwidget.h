/***************************************************************************
                          effectwidget.h  -  description
                             -------------------
    begin                : Mar 6 2003
    copyright            : (C) 2003 by Mark Kretschmann
    email                : markey@web.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef EFFECTWIDGET_H
#define EFFECTWIDGET_H

#include <qlistview.h>

#include <kdialogbase.h>

class QGroupBox;
class QMoveEvent;
class QPoint;
class QPushButton;
class QWidget;
class KComboBox;

class EffectListItem;

/**
 *@author mark
 */

//singleton
class EffectWidget : public KDialogBase
{
    Q_OBJECT

    public:
        EffectWidget( QWidget* parent = 0 );
        ~EffectWidget();

// ATTRIBUTES ------
        static EffectWidget* self;
        static QPoint        save_pos;
    
    private slots:
        void slotButtonTop();
        void slotButtonBotConf();
        void slotButtonBotRem();
        void slotItemClicked( QListViewItem *pCurrentItem );

    private:
        void closeEvent( QCloseEvent* );

// ATTRIBUTES ------
        static const int EffectWidget::BUTTON_WIDTH = 30;
        
        KComboBox   *m_pComboBox;
        QListView *m_pListView;

        QPushButton *m_pButtonTopDown;
        QPushButton *m_pButtonBotConf;
        QPushButton *m_pButtonBotRem;

        QGroupBox *m_pGroupBoxTop;
        QGroupBox *m_pGroupBoxBot;
};

////////////////////////////////////////////////////////////////////////////////
// CLASS EffectListItem
////////////////////////////////////////////////////////////////////////////////

class EffectListItem : public QListViewItem
{
    public:
        EffectListItem( QListView *parent, const QString &label );
        EffectListItem( QListView *parent, const QString &label, long id );
        ~EffectListItem();

        void configure();
        bool configurable() const;

// ATTRIBUTES ------
        long m_id;
};


#endif
