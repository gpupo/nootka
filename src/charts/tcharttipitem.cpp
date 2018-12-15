/***************************************************************************
 *   Copyright (C) 2018 by Tomasz Bojczuk                                  *
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

#include "tcharttipitem.h"
#include "tgroupedqaunit.h"
#include <tcolor.h>
#include <tnootkaqml.h>
#include <exam/texam.h>
#include <exam/tlevel.h>
#include <exam/tresulttext.h>
#include <exam/textrans.h>
#include <score/tscoreobject.h>

#include <QtGui/qguiapplication.h>
#include <QtGui/qpalette.h>
#include <QtCore/qdebug.h>


TchartTipItem::TchartTipItem(QQuickItem* parent) :
  QQuickItem(parent)
{
}


TchartTipItem::~TchartTipItem()
{
}


int TchartTipItem::number() const {
  return m_question ? m_question->nr() : 0;
}


QColor TchartTipItem::color() const {
  return m_question ? m_question->color() : QColor(0, 0, 0, 0);
}


QPointF TchartTipItem::pos() const {
  return m_question ? m_question->cursorPos() : QPointF();
}


QString TchartTipItem::qaText() const {
  return m_question ? tr("question") + QLatin1String(": <b>") + NOO->qaTypeText(m_question->qaUnit()->questionAs) + QLatin1String("</b><br>") +
          tr("answer") + QLatin1String(": <b>") + NOO->qaTypeText(m_question->qaUnit()->answerAs) : QString();
}


bool TchartTipItem::isMelody() const {
  return m_exam && m_exam->melodies();
}


bool TchartTipItem::hasSecondScore() const {
  return m_exam && m_exam->level()->answerIsNote();
}


bool TchartTipItem::leftScoreVisible() const {
  return m_question && (m_question->qaUnit()->questionOnScore() || (m_question->qaUnit()->questionAsSound() && m_question->qaUnit()->answerAsSound()));
}


bool TchartTipItem::rightScoreVisible() const {
  return m_question && !m_exam->melodies() && m_question->qaUnit()->answerOnScore();
}


QString TchartTipItem::questionText() const {
  if (m_question && (!m_question->qaUnit()->questionOnScore() || !m_question->qaUnit()->questionAsSound())) {
    if (m_question->qaUnit()->questionAsName())
      return QLatin1String("<span style=\"font-size: xx-large;\">") + m_question->qaUnit()->qa.note.toRichText(m_question->qaUnit()->styleOfQuestion())
             + QLatin1String("</span>");
    else if (m_question->qaUnit()->questionOnInstr())
      return m_question->qaUnit()->qa.pos().toHtml();
  }
  return QString();
}


QString TchartTipItem::answerText() const {
  if (m_question && !m_question->qaUnit()->answerOnScore()) {
    TQAgroup& qaGroup = m_question->qaUnit()->questionAs == m_question->qaUnit()->answerAs ? m_question->qaUnit()->qa_2 : m_question->qaUnit()->qa;
    if (m_question->qaUnit()->answerAsName())
      return QLatin1String("<span style=\"font-size: xx-large;\">") + qaGroup.note.toRichText(m_question->qaUnit()->styleOfAnswer())
             + QLatin1String("</span>");
    else if (m_question->qaUnit()->answerOnInstr())
      return qaGroup.pos().toHtml();
    else if (m_question->qaUnit()->answerAsSound())
      return QString(" <span style=\"font-size: %1px; font-family: Nootka\">n</span>").arg(NOO->fontSize() * 3);
  }
  return QString();
}



void TchartTipItem::setLeftScore(TscoreObject* ls) {
  if (ls != m_leftScore) {
    m_leftScore = ls;
  }
}


void TchartTipItem::setSecondScore(TscoreObject* ss) {
  if (ss != m_secondScore) {
    m_secondScore = ss;
  }
}


QString TchartTipItem::resultText() const {
  return m_question ? wasAnswerOKtext(m_question->qaUnit()) : QString();
}


QString TchartTipItem::timeText() const {
  if (m_question) {
    QString txt;
    if (m_question->qaUnit()->melody() && !m_question->qaUnit()->isWrong())
      txt += TexTrans::effectTxt() + QString(": <b>%1%</b><br>").arg(m_question->qaUnit()->effectiveness(), 0, 'f', 1, '0');
    txt += TexTrans::reactTimeTxt() + QString("<font size=\"4\"><b>  %1</b></font>").arg(Texam::formatReactTime(m_question->qaUnit()->time, true));
    return txt;
  }
  return QString();
}


QString TchartTipItem::tipText() const {
  return m_question ? m_question->tipText : QString();
}


void TchartTipItem::setQuestion(TtipInfo* q) {
  if (m_question != q) {
    bool emitShow = m_question == nullptr || q == nullptr;
    m_question = q;
    if (emitShow)
      emit showChanged();
    if (m_question) {
      emit questionUpdated();
      if (m_question->kind == TtipInfo::e_point) {
          emit questionTipUpdated();
          if (m_leftScore) {
            if (m_question->qaUnit()->melody()) {
                int transposition = 0;
                auto tempKey = m_question->qaUnit()->melody()->key();
                if (m_question->qaUnit()->melody()->key() != m_question->qaUnit()->key) {
                  transposition = m_question->qaUnit()->melody()->key().difference(m_question->qaUnit()->key);
                  m_question->qaUnit()->melody()->setKey(m_question->qaUnit()->key);
                }
                m_leftScore->setMelody(m_question->qaUnit()->melody(), true, 7, transposition);
                m_question->qaUnit()->melody()->setKey(tempKey);
            } else {
                if (m_question->qaUnit()->questionOnScore() || (m_question->qaUnit()->questionAsSound() && m_question->qaUnit()->answerAsSound())) {
                  if (m_leftScore->notesCount())
                    m_leftScore->setNote(0, m_question->qaUnit()->qa.note);
                  else
                    m_leftScore->addNote(m_question->qaUnit()->qa.note);
                  if (m_exam->level()->useKeySign)
                    m_leftScore->setKeySignature(m_question->qaUnit()->key.value());
                }
                bool qaTheSmaeType = m_question->qaUnit()->questionAs == m_question->qaUnit()->answerAs;
                if (m_question->qaUnit()->answerOnScore()) {
                  if (m_secondScore->notesCount())
                    m_secondScore->setNote(0, qaTheSmaeType ? m_question->qaUnit()->qa_2.note : m_question->qaUnit()->qa.note);
                  else
                    m_secondScore->addNote(qaTheSmaeType ? m_question->qaUnit()->qa_2.note : m_question->qaUnit()->qa.note);
                  if (m_exam->level()->useKeySign)
                    m_secondScore->setKeySignature(m_question->qaUnit()->key.value());
                }
            }
          }
      } else if (m_question->kind == TtipInfo::e_line) {
          emit lineTipUpdated();
      }
      if (m_kindOfTip != m_question->kind) {
        m_kindOfTip = m_question->kind;
        emit tipTypeChanged();
      }
    }
  }
}


void TchartTipItem::setExam(Texam* e) {
  m_exam = e;
  m_leftScore->clearScore();
  emit examChanged();
  if (!m_exam->melodies()) {
    m_leftScore->setClefType(m_exam->level()->clef.type());
    m_leftScore->setMeter(Tmeter::NoMeter);
    m_leftScore->setKeySignatureEnabled(m_exam->level()->useKeySign);
  }
  if (hasSecondScore()) {
    m_secondScore->setMeter(Tmeter::NoMeter);
    m_secondScore->setClefType(m_exam->level()->clef.type());
    m_secondScore->setKeySignatureEnabled(m_exam->level()->useKeySign);
  }
}


int TchartTipItem::tipType() const {
  return m_kindOfTip;
}