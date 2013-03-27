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


#ifndef TINTONATIONVIEW_H
#define TINTONATIONVIEW_H

#include "tabstractsoundview.h"


class TintonationView : public TabstractSoundView
{
  
  Q_OBJECT
  
public:
    enum Eaccuracy {
      e_perfect, 
      e_normal,
      e_sufficient
    };
  
    explicit TintonationView(int accuracy, QWidget *parent = 0);
    virtual ~TintonationView();
    
    void setAccuracy(int accuracy);
  
public slots:
    void pitchSlot(float pitch);
    
protected:
    virtual void paintEvent(QPaintEvent* );
    virtual void resizeEvent(QResizeEvent*);
    
private:
    QList<QColor>   m_tickColors;
    int             m_ticksCount;
    float           m_pitchDiff;
    int             m_noteX;
    float           m_hiTickStep;
    
    Eaccuracy       m_accuracy;
    float           m_accurValue;

};

#endif // TINTONATIONVIEW_H
