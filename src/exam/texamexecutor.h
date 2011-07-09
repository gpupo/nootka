/***************************************************************************
 *   Copyright (C) 2011 by Tomasz Bojczuk  				   *
 *   tomaszbojczuk@gmail.com   						   *
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
 *  You should have received a copy of the GNU General Public License	   *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 ***************************************************************************/


#ifndef TEXAMEXECUTOR_H
#define TEXAMEXECUTOR_H

#include "texamlevel.h"
#include "tqaunit.h"
#include "ttune.h"
#include <QList>

class MainWindow;
class QAction;

    /** This class manages of exam executing*/
class TexamExecutor : public QObject
{
    Q_OBJECT

public:
    explicit TexamExecutor(MainWindow *mainW);

    struct TglStore {
        bool showEnharmNotes;
        bool showKeySignName;
        bool showOtherPos;
        Tnote::EnameStyle nameStyleInNoteName;
        bool useDblAccids;
        bool useKeySign;
        Ttune tune;
    };
    struct TanswerRequire {
        bool octave;
        bool accid;
        bool key;
    };
        /** Magic number in exam file to identify it.*/
    static const qint32 examVersion;
    bool closeNootka();

protected:
    MainWindow *mW;
    QAction *nextQuestAct, *prevQuestAct, *checkAct;

protected slots:
    void askQuestion();
        /** If it is called by pressing "check answer" it obviously shows results
        * but if app is closeing it only checks answer and save it.*/
    void checkAnswer(bool showResults = true);
    void stopExamSlot();
    void repeatQuestion();

private:
    void createQuestionsList();
    Tnote determineAccid(Tnote n);
        /** */
    Tnote forceEnharmAccid(Tnote n);

    void prepareToExam();
    void restoreAfterExam();
        /** Disables score, noteName and guitar*/
    void disableWidgets();
    void clearWidgets();
    QString getTextHowAccid(Tnote::Eacidentals accid);
    QString saveExamToFile();



    TexamLevel m_level;
    QString m_userName;
    QList<TQAunit::TQAgroup> m_questList;
    QList<TQAunit> m_answList;
//    TQAunit m_curQ;
        /** Previous accidentals used*/
    Tnote::Eacidentals m_prevAccid;
        /** intervals between asking about double accidentals.
        * By default every forth question is with double adccid.*/
    int m_dblAccidsCntr;
    Tnote m_note2;
        /** to switch nameing style between "do re mi" and "c d e"
        * when question and answer are note name.*/
    bool m_isSolfege;
    Tnote::EnameStyle m_prevStyle;
    TglStore m_glStore;
    TanswerRequire m_answRequire;
    QString m_examFile;

};

#endif // TEXAMEXECUTOR_H
