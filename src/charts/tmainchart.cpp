/***************************************************************************
 *   Copyright (C) 2012-2013 by Tomasz Bojczuk                             *
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

#include "tmainchart.h"
#include "texam.h"
#include "texamview.h"
#include "tmainline.h"
#include "txaxis.h"
#include "tyaxis.h"
#include "tstafflinechart.h"
#include "tgraphicsline.h"
#include "ttipchart.h"
#include "sorting.h"
#include "texamlevel.h"
#include "tquestionaswdg.h"
#include "tnotename.h"
#include <QDebug>




TmainChart::TmainChart(Texam* exam, Tchart::Tsettings& settings, QWidget* parent):
  Tchart(parent),
  currExam(exam),
  chartSett(settings),
  hasListUnrelated(false),
  goodSize(0)
{
  setMouseTracking(true);
//  sortedLists.clear();
  if (settings.order == e_byNote || settings.order == e_byFret ||
          settings.order == e_byKey || settings.order == e_byAccid ||
          settings.type == e_bar)
      sort();  
  
}


TmainChart::~TmainChart()
{}

//####################################################################################
//##################### protected methods ############################################
//####################################################################################

void TmainChart::sort() {
        if (chartSett.separateWrong) {
            divideGoodAndBad(currExam->answList(), goodAnsw, badAnsw);
            if (chartSett.order == e_byNote)
              sortedLists = sortByNote(goodAnsw, currExam->level(), hasListUnrelated);
            else
              if (chartSett.order == e_byFret)
                sortedLists = sortByFret(goodAnsw, currExam->level(), hasListUnrelated);
              else
                if (chartSett.order == e_byKey)
                  sortedLists = sortByKeySignature(goodAnsw, currExam->level(), hasListUnrelated);
                else
                  if (chartSett.order == e_byAccid)
                  sortedLists = sortByAccidental(goodAnsw, currExam->level(), hasListUnrelated, kindOfAccids);
            goodSize = sortedLists.size(); // number without wrong answers
            if (chartSett.order == e_byNote)
              sortedLists.append(sortByNote(badAnsw, currExam->level(), hasListUnrelated));
            else
              if (chartSett.order == e_byFret)
                sortedLists.append(sortByFret(badAnsw, currExam->level(), hasListUnrelated));
              else
                if (chartSett.order == e_byKey)
                  sortedLists.append(sortByKeySignature(badAnsw, currExam->level(), hasListUnrelated));
                else
                  if (chartSett.order == e_byAccid)
                  sortedLists.append(sortByAccidental(badAnsw, currExam->level(), hasListUnrelated, kindOfAccids));
        } else {
            TgroupedQAunit convList = convertToPointers(currExam->answList());
            if (chartSett.order == e_byNote)
              sortedLists = sortByNote(convList, currExam->level(), hasListUnrelated);
            else
              if (chartSett.order == e_byFret)
                sortedLists = sortByFret(convList, currExam->level(), hasListUnrelated);
              else
                if (chartSett.order == e_byKey)
                  sortedLists = sortByKeySignature(convList, currExam->level(), hasListUnrelated);
                else
                  if (chartSett.order == e_byAccid)
                    sortedLists = sortByAccidental(convList, currExam->level(), hasListUnrelated, kindOfAccids);
            goodSize = sortedLists.size();
        }
}


void TmainChart::prepareChart(int maxX) {
  // Grid lines
  QColor lineColor = palette().foreground().color();
  for(int i = 5; i < maxX; i++) {
    if (i%5 == 0)
      scene->addLine(xAxis->mapValue(i) + xAxis->pos().x(), 0,
        xAxis->mapValue(i) + xAxis->pos().x(), yAxis->length(), 
                     QPen(QBrush(lineColor), 1, Qt::DashLine));
  }
  QList<double> listY;
  yAxis->getYforGrid(listY);
  if (listY.size()) {
      for(int i = 0; i < listY.size(); i++)
        scene->addLine(xAxis->pos().x(), listY[i],
        xAxis->pos().x() + xAxis->length(), listY[i],
                     QPen(QBrush(lineColor), 1, Qt::DashLine));
  }  
  QGraphicsSimpleTextItem *axisUnit = new QGraphicsSimpleTextItem();
  axisUnit->setBrush(QColor(palette().text().color()));
  scene->addItem(axisUnit);
  axisUnit->setText("[s]");
  axisUnit->setPos(xAxis->pos().x() + 7, -5);

}

