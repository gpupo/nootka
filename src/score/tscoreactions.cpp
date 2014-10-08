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


#include "tscoreactions.h"
#include "tmainscore.h"
#include "tscorekeys.h"
#include <widgets/tpushbutton.h>
#include <tpath.h>
#include <QAction>
#include <QStyle>
#include <QShortcut>
#include <QDebug>


TscoreActions::TscoreActions(TmainScore* sc) :
	QObject(sc)
{
	m_outZoom = createAction("zoom-out", tr("Zoom score out"));
		connect(m_outZoom, SIGNAL(triggered()), sc, SLOT(zoomScoreSlot()));
// 		m_outZoom->setShortcut(QKeySequence(QKeySequence::ZoomOut));
// 		m_outZoom->setShortcutContext(Qt::ApplicationShortcut);
	m_inZoom = createAction("zoom-in", tr("Zoom score in"));
		connect(m_inZoom, SIGNAL(triggered()), sc, SLOT(zoomScoreSlot()));
	m_firstNote = createAction(QIcon(sc->style()->standardIcon(QStyle::SP_ArrowBack)), tr("Go to the first note"));
		connect(m_firstNote, SIGNAL(triggered()), sc, SLOT(moveSelectedNote()));
	m_staffUp = createAction(QIcon(sc->style()->standardIcon(QStyle::SP_ArrowUp)), tr("Go to the staff above"));
		connect(m_staffUp, SIGNAL(triggered()), sc, SLOT(moveSelectedNote()));
	m_staffDown = createAction(QIcon(sc->style()->standardIcon(QStyle::SP_ArrowDown)), tr("Go to the staff below"));
		connect(m_staffDown, SIGNAL(triggered()), sc, SLOT(moveSelectedNote()));
	m_lastNote = createAction(QIcon(sc->style()->standardIcon(QStyle::SP_ArrowForward)), tr("Go to the last note"));
		connect(m_lastNote, SIGNAL(triggered()), sc, SLOT(moveSelectedNote()));
	
	m_clear = createAction("clear-score", tr("Delete all notes on the score"));
	connect(m_clear, SIGNAL(triggered()), sc, SLOT(deleteNotes()));
	
	m_accidsButt = new TpushButton("$", sc); // (#)
		m_accidsButt->setStatusTip(tr("Shows accidentals from the key signature also next to a note. <b>WARRING! It never occurs in real scores - use it only for theoretical purposes.</b>"));
		m_accidsButt->setThisColors(Qt::red, sc->palette().highlightedText().color());
	m_namesButt = new TpushButton("c", sc);
		m_namesButt->setStatusTip(tr("Shows names of all notes on the score"));
	QList<TpushButton*> buttons;
	buttons << m_accidsButt << m_namesButt;
	for (int i = 0; i < buttons.size(); i++) {
		QFont nf("nootka");
		nf.setPointSizeF(24.0);
		QFontMetrics fm(nf);
		nf.setPointSizeF(qRound(nf.pointSizeF() * (nf.pointSizeF() / fm.boundingRect("x").height())));
		buttons[i]->setFont(nf);
		fm = QFontMetrics(nf);
		buttons[i]->setMaximumWidth(fm.boundingRect("xx").width());
	}
	connect(m_accidsButt, SIGNAL(clicked()), sc, SLOT(extraAccidsSlot()));
	connect(m_namesButt, SIGNAL(clicked()), sc, SLOT(showNamesSlot()));

		
}


void TscoreActions::assignKeys(TscoreKeys* sKeys) {
	m_keys = sKeys;
	assocActionAndKey(m_firstNote, m_keys->firstNote());
	assocActionAndKey(m_lastNote, m_keys->lastNote());
	assocActionAndKey(m_staffDown, m_keys->staffDown());
	assocActionAndKey(m_staffUp, m_keys->staffUp());
	assocActionAndKey(m_clear, m_keys->clearScore());
}


void TscoreActions::disableActions(bool dis) {
	if (dis != m_firstNote->signalsBlocked()) { // disability of actions is different than required, so do it
		foreach(QAction* a, m_actions)
			a->blockSignals(dis);
	}
}


//####################################################################################################
//########################################## PRIVATE #################################################
//####################################################################################################

void TscoreActions::assocActionAndKey(QAction* act, QShortcut* key) {
	QString keyText = m_keys->firstNote()->key().toString();
	if (keyText != "")
		act->setStatusTip(act->statusTip() + " (" + key->key().toString() + ")");
	connect(key, SIGNAL(activated()), act, SLOT(trigger()));
}


QAction* TscoreActions::createAction(const QString& ico, const QString& statTip) {
	QAction *a = new QAction(QIcon(Tpath::img(ico)), "", this);
	equipAcction(a, statTip);
	return a;
}


QAction* TscoreActions::createAction(const QIcon& ico, const QString& statTip) {
	QAction *a = new QAction(ico, "", this);
	equipAcction(a, statTip);
	return a;
}


void TscoreActions::equipAcction(QAction* act, const QString& statTip) {
	act->setStatusTip(statTip);
	m_actions << act;
}










