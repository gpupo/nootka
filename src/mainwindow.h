/***************************************************************************
 *   Copyright (C) 2011 by Tomasz Bojczuk  				   *
 *   tomaszbojczuk@gmail.com   						   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License	   *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 ***************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tscorewidget.h"
#include "tnotename.h"
#include "tfingerboard.h"
#include "tnote.h"
#include "texamlevel.h"
#include "texamview.h"
//#include <QtGui>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();


public slots:
    void createSettingsDialog();
    void createExamSettingsDlg();
    void startExamSlot();
    void aboutSlot();

    void noteWasClicked(int index, Tnote note);
    void noteNameWasChanged(Tnote note);
    void guitarWasClicked(Tnote note);

protected:
    void resizeEvent(QResizeEvent *);
    bool event(QEvent *event);

private:
    TscoreWidget *m_score;
    TnoteName *m_noteName;
    TfingerBoard *m_guitar;
    TexamView *m_examView;
    QLabel *m_statLab;

//    TexamLevel m_level;

    QAction *settingsAct, *examSetAct, *startAct, *aboutAct;
    QToolBar *nootBar;

    void createActions();

};

#endif // MAINWINDOW_H
