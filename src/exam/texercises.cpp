/***************************************************************************
 *   Copyright (C) 2013 by Tomasz Bojczuk                                  *
 *   tomaszbojczuk@gmail.com                                               *
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
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 ***************************************************************************/

#include "texercises.h"
#include "texam.h"
#include <troundedlabel.h>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPushButton>
#include <QStyle>
#include <QVBoxLayout>
#include <QGroupBox>

// #include <QDebug>

Texercises::Texercises(Texam* exam) :
	m_exam(exam),
	m_checkNow(false)
{
}


void Texercises::setSuggestionEnabled(int obligateNr) {
	if (obligateNr > 0) {
			m_max = qMax(obligateNr / 5, 10); // not less than 10
			m_checkNow = true;
			m_currentGood = 0;
	} else {
			m_checkNow = false;
	}
}


void Texercises::checkAnswer() {
	if (!m_checkNow)
		return;
	if (m_exam->curQ().isCorrect()) {
		m_currentGood++;
	} else {
		m_currentGood = 0;
	}
	if (m_currentGood == m_max) {
		TsuggestExam *suggExam = new TsuggestExam();
		TsuggestExam::Esuggest what = suggExam->suggest();
		if (suggExam->result() == QDialog::Accepted) {
			switch(what) {
				case TsuggestExam::e_readyToExam:
					break;
				case TsuggestExam::e_forAmoment:
					m_currentGood = 0; // or m_currentGood -= 10
					break;
				case TsuggestExam::e_notThisTime:
					m_checkNow = false;
					break;
				case TsuggestExam::e_neverEver:
					m_chekInFuture = false;
					break;
			}
		} else
				m_currentGood -= 10;
		delete suggExam;
	}
}


//####################################################################################################################
//##################################### TsuggestExam #################################################################
//####################################################################################################################
TsuggestExam::TsuggestExam() :
	QDialog()
{
		setWindowTitle(tr("Start an exam"));
		TroundedLabel *mainLab = new TroundedLabel(tr("You are very good in this exercise!<br>Would you like to pass an exam on the same level and got a certificate?"), this);
		mainLab->setAlignment(Qt::AlignCenter);
		m_redyExamRadio = new QRadioButton(tr("Sure! Lets start an exam!"), this);
		m_notNowRadio = new QRadioButton(tr("Not now, ask me for a moment."), this);
		m_notThisExRadio = new QRadioButton(tr("Not in this exercise."), this);
		m_neverAskRadio = new QRadioButton(tr("Never more suggest me to start an exam!"), this);
		
		QButtonGroup *buttonGr = new QButtonGroup(this);
		buttonGr->addButton(m_redyExamRadio);
		buttonGr->addButton(m_notNowRadio);
		buttonGr->addButton(m_notThisExRadio);
		buttonGr->addButton(m_neverAskRadio);
		m_redyExamRadio->setChecked(true);
		
		QPushButton *okButt = new QPushButton(tr("OK"), this);
		okButt->setIcon(QIcon(style()->standardIcon(QStyle::SP_DialogOkButton)));
		
		QVBoxLayout *lay = new QVBoxLayout;
		lay->addWidget(mainLab);
		QGroupBox *radioBox = new QGroupBox(this);
		QVBoxLayout *radioLay = new QVBoxLayout;
			radioLay->addWidget(m_redyExamRadio, 1, Qt::AlignCenter);
			radioLay->addWidget(m_notNowRadio, 1, Qt::AlignCenter);
			radioLay->addWidget(m_notThisExRadio, 1, Qt::AlignCenter);
			radioLay->addWidget(m_neverAskRadio, 1, Qt::AlignCenter);
		radioBox->setLayout(radioLay);
		lay->addWidget(radioBox);
		lay->addStretch(1);
		lay->addWidget(okButt, 1, Qt::AlignCenter);
		setLayout(lay);
		
		connect(okButt, SIGNAL(clicked()), this, SLOT(accept()));
}


TsuggestExam::Esuggest TsuggestExam::suggest() {
		exec();
		if (m_redyExamRadio->isChecked())
			return e_readyToExam;
		if (m_notNowRadio->isChecked())
			return e_forAmoment;
		if (m_notThisExRadio->isChecked())
			return e_notThisTime;
		if (m_neverAskRadio->isChecked()) {
			return e_neverEver;
		}
}









