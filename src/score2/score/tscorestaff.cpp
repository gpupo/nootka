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

#include "tscorestaff.h"
#include "tscorescene.h"
#include "tscoreclef.h"
#include "tscorenote.h"
#include "tscorekeysignature.h"
#include <QGraphicsView>

#include <QDebug>

TscoreStaff::TscoreStaff(TscoreScene* scene) :
  TscoreItem(scene)
{
  setAcceptHoverEvents(true);
  for (int i = 0; i < 5; i++) {
    m_lines[i] = new QGraphicsLineItem();
    registryItem(m_lines[i]);
    m_lines[i]->setPen(QPen(scene->views()[0]->palette().windowText().color(), 0.2));
    m_lines[i]->setLine(1, 16 + i * 2, boundingRect().width() - 2, 16 + i * 2);
  }
  
  m_clef = new TscoreClef(scene, Tclef());
  connect(m_clef, SIGNAL(clefChanged(Tclef::Etype)), this, SLOT(onClefChanged(Tclef::Etype)));
  
  m_keySignature = new TscoreKeySignature(scene);
  m_keySignature->setPos(m_clef->boundingRect().width() + 0.5, 0);
  m_keySignature->setClef(m_clef->clef());
  
  m_notes << new TscoreNote(scene);
  m_notes[0]->setPos(m_clef->boundingRect().width() + m_keySignature->boundingRect().width() + 1, 0);
  
  for (int i = 0; i < 7; i++)
    accidInKeyArray[i] = 0;
 
  setStatusTip(tr("This is a staff"));
}


TscoreStaff::~TscoreStaff() {}


QRectF TscoreStaff::boundingRect() const {
  return QRectF(0, 0, 80, 40);
}


void TscoreStaff::onClefChanged(Tclef::Etype ) {
  if (m_keySignature)
    m_keySignature->setClef(m_clef->clef());
}


