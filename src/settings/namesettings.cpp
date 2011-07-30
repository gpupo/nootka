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


#include "namesettings.h"
#include "tglobals.h"
#include "select7note.h"
#include <QtGui>

extern Tglobals *gl;

NameSettings::NameSettings(QWidget *parent) :
        QWidget(parent)
{
    QVBoxLayout *mainLay = new QVBoxLayout;
    mainLay->setAlignment(Qt::AlignCenter);
    QHBoxLayout *nLay = new QHBoxLayout;
    nameStyleGr = new TnotationRadioGroup(gl->NnameStyleInNoteName, this);
    nLay->addWidget(nameStyleGr);
    nLay->addSpacing(5);

    select7 = new Select7note(this);
    nLay->addWidget(select7);

    select7->set7th_B(gl->seventhIs_B);

    mainLay->addLayout(nLay);
    mainLay->addStretch(1);

    /** @todo example label with all scale*/

    octInNameCh = new QCheckBox(tr("show octave in the note's name"),this);
    mainLay->addWidget(octInNameCh);
    octInNameCh->setStatusTip(tr("Shows formated note's name. For small octave - the name is small letter,<br>for great octave - the name starts with a capital letter,<br>for one-line, digit <sup>1</sup> is added, and so on." ));
    octInNameCh->setChecked(gl->NoctaveInNoteNameFormat);
    mainLay->addStretch(1);
    setLayout(mainLay);

    connect (select7, SIGNAL(seventhIsBchanged(bool)), this, SLOT(seventhNoteWasChanged(bool)));
}

void NameSettings::saveSettings() {
    gl->NnameStyleInNoteName = nameStyleGr->getNameStyle();
    gl->NoctaveInNoteNameFormat = octInNameCh->isChecked();
    gl->seventhIs_B = select7->is7th_B();
}

void NameSettings::seventhNoteWasChanged(bool isB) {
        nameStyleGr->seventhNoteWasChanged(isB);
        emit seventhIsBChanged(isB);
}
