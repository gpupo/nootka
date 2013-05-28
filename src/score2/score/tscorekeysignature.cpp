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

#include "tscorekeysignature.h"
#include "tscorescene.h"
#include "tscorenote.h"
#include "tscorestaff.h"
#include <QGraphicsView>
#include <QGraphicsSceneHoverEvent>
#include <QDebug>

char TscoreKeySignature::m_posOfAccid[7] = {
    0, // Fes & Fis (F#)
    3, // Ces (C#)
    -1, // Ges (G#)
    2, // Des (D#)
    5, // As  (A#)
    1, // ES  (E#)
    4 // B   (H#)  (Bb - B#) in west
};

char TscoreKeySignature::m_posOfAccidFlats[7] = { 4, 1, 5, 2, 6, 3, 7 };

TscoreKeySignature::TscoreKeySignature(TscoreScene* scene, TscoreStaff* staff, char keySign) :
  TscoreItem(scene),
  m_keySignature(keySign),
  m_clef(Tclef())
{
  setStaff(staff);
	setParentItem(staff);
  m_height = staff->height();

  QFont font(TscoreNote::getAccidFont());
  for (int i = 0; i < 7; i++) {
        m_accidentals[i] = new QGraphicsSimpleTextItem();
        registryItem(m_accidentals[i]);
        m_accidentals[i]->setBrush(scene->views()[0]->palette().windowText().color());
        m_accidentals[i]->setFont(font);
        m_accidentals[i]->hide();
	}
  m_keyNameText = new QGraphicsSimpleTextItem();
	registryItem(m_keyNameText);
	m_keyNameText->setBrush(QBrush(scene->views()[0]->palette().windowText().color()));
    
    
    setStatusTip(tr("Key signature - to change it, click above or below the staff or use mouse wheel."));
}


TscoreKeySignature::~TscoreKeySignature()
{}


void TscoreKeySignature::setKeySignature(char keySign) {
  for (int i = 1; i < 8; i++) {
        int base = 0;
        char sign = 1;
        bool isFlat = false;
        int startVal = 48;
        if (keySign < 0) {
            base = 8;
            sign = -1;
            isFlat = true;
            startVal = 38;
        }
        if (i <= qAbs(keySign)) {// show accid
            m_accidentals[i - 1]->setText(TscoreNote::getAccid(sign));
            m_accidentals[i - 1]->setPos( (i - 1) * 1.1, getPosOfAccid(i - 1, isFlat) - 5.35);
            staff()->accidInKeyArray[(startVal + sign * (i * 4)) % 7] = sign;
            m_accidentals[i-1]->show();
        }
        else { // hide
            m_accidentals[i - 1]->hide();
            staff()->accidInKeyArray[(startVal + sign * (i * 4)) % 7] = 0;
        }
    }
//       qDebug() << (int)staff()->accidInKeyArray[0] << (int)staff()->accidInKeyArray[1] << 
//       (int)staff()->accidInKeyArray[2] << (int)staff()->accidInKeyArray[3] << 
//       (int)staff()->accidInKeyArray[4] << (int)staff()->accidInKeyArray[5] << (int)staff()->accidInKeyArray[6];
    m_keySignature = keySign;
//     showKeyName();
    emit keySignatureChanged();
}


char TscoreKeySignature::getPosOfAccid(int noteNr, bool flatKey) {
  char yPos;
  if (flatKey)
    yPos = m_posOfAccidFlats[noteNr] + staff()->upperLinePos() + (staff()->noteOffset() - 3);
  else {
    yPos = m_posOfAccid[noteNr] + staff()->upperLinePos() + (staff()->noteOffset() - 3);
    if (m_clef.type() == Tclef::e_tenor_C && (noteNr == 0 || noteNr == 2))
        yPos += 7;
  }
  return yPos;
}


void TscoreKeySignature::setClef(Tclef clef) {
  m_clef = clef;
  setKeySignature(keySignature());
}

QRectF TscoreKeySignature::boundingRect() const{
  return QRectF(0, 0, 9, m_height);
}


void TscoreKeySignature::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
  painter->setBrush(QColor(0, 0, 255, 30));
  painter->setPen(Qt::NoPen);
  painter->drawRect(boundingRect());
}


//##########################################################################################################
//########################################## PROTECTED   ###################################################
//##########################################################################################################

void TscoreKeySignature::mousePressEvent(QGraphicsSceneMouseEvent* event) {
  if (event->button() == Qt::LeftButton) {
    if (event->pos().y() > 20.0)
      increaseKey(-1);
    else
      increaseKey(1);
  }
}


void TscoreKeySignature::wheelEvent(QGraphicsSceneWheelEvent* event) {
    if (event->delta() > 0)
        increaseKey(1);
    else
        increaseKey(-1);
}


void TscoreKeySignature::increaseKey(int step) {
  char prevKey = m_keySignature;
  if (step == 1) {
      if (m_keySignature < 7) m_keySignature++;
  } else {
      if (m_keySignature > -7) m_keySignature--;
  }
  if (m_keySignature != prevKey)
    setKeySignature(m_keySignature);
}



