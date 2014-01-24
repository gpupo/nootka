/***************************************************************************
 *   Copyright (C) 2011-2014 by Tomasz Bojczuk                             *
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


#include "tlevelcreatordlg.h"
#include "tglobals.h"
#include "tlevelheaderwdg.h"
#include "questionssettings.h"
#include "accidsettings.h"
#include "levelsettings.h"
#include "rangesettings.h"
#include <texamparams.h>
#include <ttune.h>
#include <widgets/troundedlabel.h>
#include <QtGui>

extern Tglobals *gl;
bool isNotSaved;

TlevelCreatorDlg::TlevelCreatorDlg(QWidget *parent) :
    TsettingsDialogBase(parent)
{

    isNotSaved = false;
    setWindowTitle(levelCreatorTxt());

    navList->addItem(TlevelSelector::levelFilterTxt());
    navList->item(0)->setIcon(QIcon(gl->path+"picts/levelsSettings.png"));
    navList->item(0)->setTextAlignment(Qt::AlignCenter);
    navList->addItem(tr("Questions"));
    navList->item(1)->setIcon(QIcon(gl->path+"picts/questionsSettings.png"));
    navList->item(1)->setTextAlignment(Qt::AlignCenter);
    navList->addItem(tr("Accidentals"));
    navList->item(2)->setIcon(QIcon(gl->path+"picts/accidSettings.png"));
    navList->item(2)->setTextAlignment(Qt::AlignCenter);
    navList->addItem(tr("Range"));
    navList->item(3)->setIcon(QIcon(gl->path+"picts/rangeSettings.png"));
    navList->item(3)->setTextAlignment(Qt::AlignCenter);

    levelSett = new levelSettings(gl->path);
    questSett = new questionsSettings();
    accSett = new accidSettings();
    rangeSett = new rangeSettings();

    stackLayout->addWidget(levelSett);
    stackLayout->addWidget(questSett);
    stackLayout->addWidget(accSett);
    stackLayout->addWidget(rangeSett);
		
		if (gl->instrument == e_noInstrument)
			questSett->hideGuitarRelated();
		
    hint->setFixedHeight(fontMetrics().boundingRect("A").height() * 4);

    connect(navList, SIGNAL(currentRowChanged(int)), stackLayout, SLOT(setCurrentIndex(int)));

    navList->setCurrentRow(0);
		okBut = buttonBox->addButton(tr("Check"), QDialogButtonBox::AcceptRole);
			okBut->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
			okBut->setStatusTip(tr("Check, Are your settings for the level possible to perform."));
		cancelBut = buttonBox->addButton(QDialogButtonBox::Close);
			cancelBut->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
    
    connect(okBut, SIGNAL(clicked()), this, SLOT(checkLevelSlot()));
    connect(levelSett->levelSelector, SIGNAL(levelChanged(Tlevel)),
            this, SLOT(levelWasSelected(Tlevel))); // to load level to widgets
    connect(rangeSett, SIGNAL(rangeChanged()), this, SLOT(levelNotSaved()));
    connect(questSett, SIGNAL(questSettChanged()), this, SLOT(levelNotSaved()));
    connect(accSett, SIGNAL(accidsChanged()), this, SLOT(levelNotSaved()));
    connect(levelSett->saveBut, SIGNAL(clicked()), this, SLOT(saveToFile()));
    connect(levelSett->levelSelector, SIGNAL(levelToLoad()), this, SLOT(loadFromFile()));
    connect(levelSett->startExamBut, SIGNAL(clicked()), this, SLOT(startExam()));
    
    connect(questSett, SIGNAL(scoreEnabled(bool)), accSett, SLOT(enableKeys(bool)));
    connect(questSett, SIGNAL(accidEnabled(bool)), accSett, SLOT(enableAccids(bool)));
    connect(rangeSett, SIGNAL(allStringsChecked(bool)), questSett, SLOT(stringsCheckedSlot(bool)));
}


void TlevelCreatorDlg::levelWasSelected(Tlevel level) {
    if (isNotSaved)
        saveLevel();
    questSett->loadLevel(level);
    accSett->loadLevel(level);
    rangeSett->loadLevel(level);
		if (levelSett->levelSelector->isSuitable())
				levelSett->startExamBut->setDisabled(false);
		else
				levelSett->startExamBut->setDisabled(true);
}


void TlevelCreatorDlg::levelNotSaved() {
    navList->item(0)->setIcon(QIcon(gl->path+"picts/notSaved.png"));
    setWindowTitle(levelCreatorTxt() + "  (" + tr("level not saved!") + ")");
}


void TlevelCreatorDlg::saveLevel() {
    if ( QMessageBox::question(this, tr("level not saved!"), tr("Level was changed and not saved!"),
			QMessageBox::Save, QMessageBox::Cancel) == QMessageBox::Save ) {
        saveToFile();
    } else 
				levelSaved();
}


void TlevelCreatorDlg::saveToFile() {
    Tlevel newLevel;
    questSett->saveLevel(newLevel);
    accSett->saveLevel(newLevel);
    rangeSett->saveLevel(newLevel);
    if (!newLevel.canBeGuitar() && !newLevel.answerIsSound() ) { // no guitar and no played sound  
      // adjust fret range - validation will skip it for non guitar levels
      newLevel.loFret = 0; // Set range to fret number and rest will be done by function preparing question list
      newLevel.hiFret = gl->GfretsNumber;
      newLevel.onlyLowPos = true; // otherwise the above invokes doubled/tripled questions in the list
    // set all strings as available
      for (int str = 0; str < 6; str++)
        newLevel.usedStrings[str] = true;
    }
    QString isLevelValid = validateLevel(newLevel);
    if (isLevelValid != "") {
        showValidationMessage(isLevelValid);
        return;
    }
    // set instrument to none when it is not important for the level
		newLevel.instrument = newLevel.detectInstrument(gl->instrument);
    TlevelHeaderWdg *saveDlg = new TlevelHeaderWdg(this);
    QStringList nameList = saveDlg->getLevelName();
    newLevel.name = nameList[0];
    newLevel.desc = nameList[1];
  // Saving to file
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save exam level"),
                                                    QDir::toNativeSeparators(gl->E->levelsDir + "/" + newLevel.name + ".nel"),
                                              TlevelSelector::levelFilterTxt() + " (*.nel)", 0 , QFileDialog::DontUseNativeDialog);
    if (fileName == "")
        return;
    if (fileName.right(4) != ".nel")
      fileName += ".nel";
    gl->E->levelsDir = QFileInfo(fileName).absoluteDir().absolutePath();
		if (!Tlevel::saveToFile(newLevel, fileName)) {
				QMessageBox::critical(this, " ", tr("Cannot open file for writing"));
				return;
		}
    isNotSaved = false;
    levelSett->levelSelector->addLevel(newLevel, fileName, true);
    levelSett->levelSelector->selectLevel(); // select the last
    levelSaved();
}


void TlevelCreatorDlg::levelSaved() {
    isNotSaved = false;
    navList->item(0)->setIcon(QIcon(gl->path+"picts/levelsSettings.png"));
    setWindowTitle(levelCreatorTxt());
}


void TlevelCreatorDlg::loadFromFile() {
    if (isNotSaved)
        saveLevel();
    levelSett->levelSelector->loadFromFile();
}


QString TlevelCreatorDlg::validateLevel(Tlevel &l) {
    QString res = "";
  // Check has a level sense - are there an questions and answers
    if (!l.canBeScore() && ! l.canBeName() && !l.canBeGuitar() && !l.canBeSound()) {
        res = tr("There aren't any questions or answers selected.<br>Level makes no sense.");
        return res;
    }      
  // checking range
  // determine the highest note of fret range on available strings
    if (l.canBeGuitar() || (l.instrument != e_noInstrument && l.answerIsSound())) {
			// only when guitar is enabled otherwise frets range was adjusted automatically
      int hiAvailStr, loAvailStr, cnt = -1;
      do {
          cnt ++;
      } while (!l.usedStrings[gl->strOrder(cnt)] && cnt < gl->Gtune()->stringNr());
      hiAvailStr = gl->strOrder(cnt);
      cnt = gl->Gtune()->stringNr();
      do {
          cnt--;
      } while (!l.usedStrings[gl->strOrder(cnt)] && cnt >= 0);
      loAvailStr = gl->strOrder(cnt);
      if (l.loNote.getChromaticNrOfNote() > gl->Gtune()->str(hiAvailStr + 1).getChromaticNrOfNote() + l.hiFret ||
          l.hiNote.getChromaticNrOfNote() < gl->Gtune()->str(loAvailStr + 1).getChromaticNrOfNote() + l.loFret)
// 			if (l.loNote.getChromaticNrOfNote() > gl->Gtune()->str(loAvailStr + 1).getChromaticNrOfNote() + l.loFret ||
//           l.hiNote.getChromaticNrOfNote() < gl->Gtune()->str(hiAvailStr + 1).getChromaticNrOfNote() + l.hiFret)
          res += tr("<li>Range of frets is beyond the scale of this level</li>");
    }
	// Check is level range fit to instrument scale
		if (l.canBeGuitar() || l.answerIsSound()) {
			if (!l.inScaleOf(gl->loString().getChromaticNrOfNote(), gl->hiString().getChromaticNrOfNote() + gl->GfretsNumber))
				res += "<li>" + TlevelSelector::rangeBeyondScaleTxt() + "</li>";
		}
  // checking are accidentals needed because of hi and low notes in range
    char acc = 0;
    if (l.loNote.acidental) acc = l.loNote.acidental;
    if (l.hiNote.acidental) acc = l.hiNote.acidental;
    if (acc) {
        if ( (acc == 1 && !l.withSharps) || (acc == -1 && !l.withFlats))
            res += tr("<li>In range of notes some accidentals are used<br>but not available in this level</li>");
    }
  // Force accidentals enabled but any accidental was selected
    if (l.forceAccids && (!l.withFlats && !l.withSharps && !l.withDblAcc))
			res += tr("<li>Force appropriate accidental is enabled but any accidental was selected.</li>");
	// When no accidentals and no style and question and answers are note name
    if ((l.questionAs.isName() && l.answersAs[TQAtype::e_asName].isName())) {
			if ((l.withFlats && l.withSharps) || l.withDblAcc || l.requireStyle) {
				// do nothing - level is valid
			} else 
				res += tr("<li>Questions and answers as note names will be the same. To avoid that level has to use flats and sharps and/or double accidentals and/or to use different name styles.</li>");
		}
  // Check is possible of using naming style
    if (l.requireStyle && !l.canBeName())
        res += tr("<li>'Use different naming styles' was checked but neither questions nor answers as note name are checked.<br>Check this type of answer/question or uncheck 'Use different naming styles'.</li>");
  // Check are questions and answers as note on the staff have sense (are different)
    if (l.questionAs.isNote() && l.answersAs[TQAtype::e_asNote].isNote())
      if (!l.manualKey && !l.forceAccids)
        res += tr("<li>Questions and answers as notes on the staff will be the same. Manually selecting keys or forcing accidentals has to be selected to avoid that.</li>");
  // Check is possible of manualKey
    if (l.useKeySign && l.manualKey) {
      if (!l.answersAs[TQAtype::e_asNote].isNote() && !l.answersAs[TQAtype::e_asName].isNote() &&
        !l.answersAs[TQAtype::e_asFretPos].isNote() && !l.answersAs[TQAtype::e_asSound].isNote() )
          res += tr("<li>Manual selecting of a key signature was checked but answer as note on the staff was not checked.</li>");
		}
	// 'Fret to fret' has to have suitable fret range to be possible
		if (l.questionAs.isFret() && l.answersAs[TQAtype::e_asFretPos].isFret()) {
			int minRange = 0; // first determine a minimal range for current tune
			int startStr = gl->Gtune()->str(gl->strOrder(0) + 1).getChromaticNrOfNote();
			for (int i = 1; i < gl->Gtune()->stringNr(); i++) {
				minRange = qMax(minRange, startStr - gl->Gtune()->str(gl->strOrder(i) + 1).getChromaticNrOfNote());
				startStr = gl->Gtune()->str(gl->strOrder(i) + 1).getChromaticNrOfNote();
			}
			if (l.hiFret - l.loFret < minRange)
				res += tr("<li>Fret range is not enough to find any note in different positions. At least <b>%1</b> frets range is required.</li>").arg(minRange);
		}
  // Resume warnings
    if (res != "") {
        res.prepend("<ul>");
        res += "</ul></center>";
    }
    return res;

}

void TlevelCreatorDlg::loadLevelFile(QString levelFile) {
    levelSett->levelSelector->loadFromFile(levelFile);
}


void TlevelCreatorDlg::startExam() {
    accept();
}


Tlevel TlevelCreatorDlg::selectedLevel() {
    return levelSett->levelSelector->getSelectedLevel();
}

void TlevelCreatorDlg::checkLevelSlot() {
    Tlevel tmpLevel;
    questSett->saveLevel(tmpLevel);
    accSett->saveLevel(tmpLevel);
    rangeSett->saveLevel(tmpLevel);
    QString validMessage =  validateLevel(tmpLevel);
    if (validMessage != "")
      showValidationMessage(validMessage);
    else
      QMessageBox::information(this, tr("Level validation"), tr("Level seems to be correct"));
}


void TlevelCreatorDlg::showValidationMessage(QString message) {
      if (message != "") {
				QString title = tr("Level validation");
        if (message.contains("</li>")) { // when <li> exist - warring
          message.prepend(tr("<center><b>It seems the level has some mistakes:</b>"));
          QMessageBox::warning(this, title, message); 
        }
        else { // no questions nor answers
          QMessageBox::critical(this, title, message); 
        }
    }
}
