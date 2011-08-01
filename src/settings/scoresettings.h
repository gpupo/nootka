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


#ifndef SCORESETTINGS_H
#define SCORESETTINGS_H

#include "tnotationradiogroup.h"
#include "tnote.h"

class QCheckBox;
class QLabel;
class QLineEdit;

class ScoreSettings : public QWidget
{
    Q_OBJECT
public:
    explicit ScoreSettings(QWidget *parent = 0);

    static QString forExample() { return tr("for example"); }
    static QString showKeySigName() { return tr("show names of key signature"); }

    QString getMajorExample(Tnote::EnameStyle nameStyle);
    QString getMinorExample(Tnote::EnameStyle nameStyle);

signals:

public slots:
    void enableKeySignGroup(bool enable);
    void nameStyleWasChanged(Tnote::EnameStyle nameStyle);
    void majorExtensionChanged();
    void minorExtensionChanged();
    void saveSettings();
    void seventhIsBChanged(bool isB);

private:
    QCheckBox *enablKeySignCh;
    QGroupBox *enablKeyNameGr, *nameExtGr;
    QLabel *majExtLab, *minExtLab, *majExampl, *minExampl;
    QLineEdit *majEdit, *minEdit;
    TnotationRadioGroup *nameStyleGr;
    Tnote::EnameStyle m_workStyle;


};


#endif // SCORESETTINGS_H
