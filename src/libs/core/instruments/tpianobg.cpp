/***************************************************************************
 *   Copyright (C) 2017 by Tomasz Bojczuk                                  *
 *   seelook@gmail.com                                                     *
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

#include "tpianobg.h"
#include "tpath.h"
#include "music/tnote.h"

#include <QtGui/qpainter.h>
#include <QtGui/qguiapplication.h>
#include <QtGui/qscreen.h>
#include <QtCore/qmath.h>
#include <QtCore/qdebug.h>

#include "checktime.h"


TpianoBg::TpianoBg(QQuickItem* parent) :
  QQuickPaintedItem(parent),
  m_keyWidth(32.0),
  m_firstOctave(-2)
{
  setAcceptHoverEvents(true);
  setRenderTarget(QQuickPaintedItem::FramebufferObject);
  //   setPerformanceHint(QQuickPaintedItem::FastFBOResizing);
  setAntialiasing(true);
  setAcceptedMouseButtons(Qt::LeftButton);
}


TpianoBg::~TpianoBg()
{

}


void TpianoBg::setKeyWidth(qreal kw) {
  if (m_keyWidth != kw) {
    m_keyWidth = kw;
    calculateMetrics(width());
    update();
  }
}


void TpianoBg::setNote(const Tnote& n) {
  
}


void TpianoBg::setFirstOctave(int firstO) {
  auto fo = static_cast<char>(firstO);
  if (fo != m_firstOctave) {
    
  }
}


void TpianoBg::setReadOnly(bool ro) {
  if (ro != m_readOnly) {
    m_readOnly = ro;
    setAcceptedMouseButtons(m_readOnly ? Qt::NoButton : Qt::LeftButton);
  }
}


void TpianoBg::paint(QPainter* painter) {
CHECKTIME (
  int kw = qFloor(m_keyWidth);
  auto keyPix = QPixmap(Tpath::img("pianokey")).scaled(qRound(m_keyWidth * 0.8), height() / 2, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  painter->setPen(QPen(Qt::black, m_keyWidth / 16, Qt::SolidLine));
  painter->setBrush(Qt::white);
  for (int k = 0; k < m_keysNumber; ++k) {
    painter->drawRoundedRect(m_margin + k * kw, 1, kw, height() - 2, m_keyWidth / 5, m_keyWidth / 5);
    if (k % 7 != 0 && k % 7 != 3)
      painter->drawPixmap(m_margin + k * kw - qRound(m_keyWidth * 0.4), kw, keyPix);
  }
  painter->setBrush(Qt::black);
  painter->drawRect(0, 0, width(), kw);
  if (m_margin > 0) {
    painter->drawRect(0, kw, m_margin, height() - kw);
    painter->drawRect(width() - m_margin, kw, m_margin, height() - kw);
  }
)
}

//#################################################################################################
//###################              PROTECTED           ############################################
//#################################################################################################

void TpianoBg::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) {
  if (oldGeometry.width() != newGeometry.width() || oldGeometry.height() != newGeometry.height()) {
    calculateMetrics(qFloor(newGeometry.width()));
    update();
  }
}


void TpianoBg::hoverEnterEvent(QHoverEvent*) {
  m_active = true;
  emit activeChanged();
}


void TpianoBg::hoverLeaveEvent(QHoverEvent*) {
  m_active = false;
  m_keyRect = QRectF();
  emit activeChanged();
  emit keyRectChanged();
}


void TpianoBg::hoverMoveEvent(QHoverEvent* event) {
  if (event->pos().y() > m_keyWidth) {
    int kw = qFloor(m_keyWidth);
    int keyNr = qFloor((event->pos().x() - m_margin) / kw);
    Tnote newNote(static_cast<char>(keyNr % 7 + 1), m_firstOctave + static_cast<char>(keyNr / 7), 0);
    bool isWhite = true;
    if (event->pos().y() < height() / 2) { // black keys
      if (keyNr % 7 != 0 && keyNr % 7 != 3) {
        --newNote.note;
        newNote.alter = Tnote::e_Sharp;
        isWhite = false;
      }
    }
    if (m_activeNote != newNote) {
      m_activeNote.note = newNote.note; m_activeNote.octave = newNote.octave; m_activeNote.alter = newNote.alter;
      if (isWhite)
        m_keyRect.setRect(m_margin + keyNr * kw, m_keyWidth, m_keyWidth, height() - m_keyWidth);
      else
        m_keyRect.setRect(m_margin + keyNr * kw - m_keyWidth * 0.4, m_keyWidth, m_keyWidth * 0.8, height() / 2.0);
      emit keyRectChanged();
    }
  }
}


void TpianoBg::mousePressEvent(QMouseEvent* event) {
  if (event->buttons() & Qt::LeftButton) {
    m_note = m_activeNote;
    emit noteChanged();
  }
}



void TpianoBg::calculateMetrics(int newWidth) {
  m_keysNumber = newWidth / qRound(m_keyWidth);
  m_margin = (newWidth - m_keysNumber * qFloor(m_keyWidth)) / 2;
}