/***************************************************************************
 *   Copyright (C) 2014 by Tomasz Bojczuk                                  *
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


#ifndef TNOTECONTROL_H
#define TNOTECONTROL_H

#include "tscoreitem.h"
#include <QPointer>

class TscoreNote;
class TscoreStaff;



class TnoteControl : public TscoreItem
{

	Q_OBJECT
	
public:
	TnoteControl(TscoreStaff* staff, TscoreScene* scene);
	virtual ~TnoteControl();

	TscoreNote* scoreNote() { return m_scoreNote; }
	void setScoreNote(TscoreNote* sn);
	
	void adjustSize(); /** Grabs height from staff and adjust items to it. */
	
	void hideWithDelay();
	
			/** Adds accidentals symbols to the controller. Detects are double accidentals enabled.
			 * It can be also used to refresh double accidentals state - add/remove them */
	void addAccidentals();
	
	void setAccidental(int acc);
	
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
	virtual QRectF boundingRect() const;
	
signals:
	void nameMenu(TscoreNote* scoreNote);
		
protected:
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent* event);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
	
	void itemSelected(const QPointF& cPos);
	
//Android
	virtual void touched(const QPointF& cPos);
	virtual void untouched(const QPointF& cPos);
	virtual void touchMove(const QPointF& cPos);
	virtual void shortTap(const QPointF& cPos);
	
protected slots:
	void hideDelayed();
	void hoverEnterDelayed(); /** To avoid blinking with status tip when mouse is flying over score */
	void showDelayed();
		
private:
		TscoreNote																*m_scoreNote;
		qreal						 								 					 m_height;
		bool						 								 					 m_entered;
		QGraphicsSimpleTextItem										*m_plus, *m_name;
		QGraphicsSimpleTextItem									  *m_dblSharp, *m_sharp, *m_flat, *m_dblFlat;
		QGraphicsRectItem													*m_accidHi;
		QGraphicsLineItem													*m_minus;
//Android
		QGraphicsItem															*m_underItem; // Item under mouse
		bool														 					 m_moveNote; // True when note cursor is moved with finger
		int																				 m_currAccid;
		QGraphicsSimpleTextItem 									*m_prevAccidIt;
		QString 																	 m_statusTip;
		
private:
		QGraphicsSimpleTextItem* createNootkaTextItem(const QString& aText);
		inline qreal centeredX(QGraphicsItem* it); /** X coordinate of centered item position */
		void accidChanged(QGraphicsItem* accidIt);
		void markItemText(QGraphicsSimpleTextItem* textItem);
		
};

#endif // TNOTECONTROL_H
