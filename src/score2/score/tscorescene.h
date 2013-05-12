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

#ifndef TSCORESCENE_H
#define TSCORESCENE_H

#include <QGraphicsScene>

class TscoreStaff;


class TscoreScene : public QGraphicsScene
{
  
  Q_OBJECT
  
public:
    TscoreScene(QObject* parent = 0);
    
    void addScoreItem(TscoreStaff* it);
    
    void setDoubleAccidsEnabled(bool enable);
      /** Returns value 2 when double accidentals are enabled and 1 if not. */
    qint8 doubleAccidsFuse() { return m_dblAccFuse; }
    
signals:
    void statusTip(QString);
    
protected slots:
    void statusTipChanged(QString status) { emit statusTip(status); }
    
private:
      /** It is @p 2 if double accidentals are enabled and @p 1 if not*/
    qint8 m_dblAccFuse;
    
    

};

#endif // TSCORESCENE_H
