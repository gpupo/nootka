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


#ifndef TTIPHANDLER_H
#define TTIPHANDLER_H

#include <qgraphicsitem.h>

class QTimer;
class TgroupedQAunit;
class TgraphicsTextTip;


/** This class manages a tip on a chart.
 * It displays it and removes after appropirate time. 
 * This is base class for chart itmes witch display tips.
 * Top level class inherited from @class TtipHandler has to:
 * - reimplement hoverEnterEvent(QGraphicsSceneHoverEvent *event)
 * and prepare there TgraphicsTextTip *tip
 * - then call handleTip(event->scenePos()) inside 
 * Static QTimer and TgraphicsTextTip art to have exactly only one instance
 * of them for all chart elements.
 */

class TtipHandler : public QGraphicsObject
{  
  Q_OBJECT
  
public:
    TtipHandler();
    virtual ~TtipHandler();

protected:
        /** This method should be called after creating a tip instance
         * in delivered class in its hoverEnterEvent.
         * @param scenePos is position of an event in a scene taken from event->scenePos()*/
    void handleTip(QPointF scenePos);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent*);
    static TgraphicsTextTip *tip;
    
private:
    static QTimer *m_delTimer;
  
private slots:
    void delayedDelete();
  
    
};

#endif // TTIPHANDLER_H
