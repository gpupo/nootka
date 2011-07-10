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


#include "examsettingsdlg.h"
#include "tglobals.h"
#include "tlevelheaderwdg.h"
#include <QDebug>

extern Tglobals *gl;
bool isNotSaved;

/*static*/
QString examSettingsDlg::examSettTxt = QObject::tr("exam's settings");

examSettingsDlg::examSettingsDlg(QWidget *parent) :
    TsettingsDialogBase(parent)
{

    isNotSaved = false;
    setWindowTitle(examSettTxt);

    navList->addItem(tr("Levels"));
    navList->item(0)->setIcon(QIcon(gl->path+"picts/levelsSettings.png"));
    navList->item(0)->setTextAlignment(Qt::AlignCenter);
    navList->addItem(tr("Questions"));
    navList->item(1)->setIcon(QIcon(gl->path+"picts/questionsSettings.png"));
    navList->item(1)->setTextAlignment(Qt::AlignCenter);
    navList->addItem(tr("Range"));
    navList->item(2)->setIcon(QIcon(gl->path+"picts/rangeSettings.png"));
    navList->item(2)->setTextAlignment(Qt::AlignCenter);

    levelSett = new levelSettings();
    questSett = new questionsSettings();
    rangeSett = new rangeSettings();

    stackLayout->addWidget(levelSett);
    stackLayout->addWidget(questSett);
    stackLayout->addWidget(rangeSett);

    connect(navList, SIGNAL(currentRowChanged(int)), stackLayout, SLOT(setCurrentIndex(int)));

    navList->setCurrentRow(0);

    connect(levelSett->levelSelector, SIGNAL(levelChanged(TexamLevel)), this, SLOT(levelWasSelected(TexamLevel))); // to load level to widgets
    connect(rangeSett, SIGNAL(rangeChanged()), this, SLOT(levelNotSaved()));
    connect(questSett, SIGNAL(questSettChanged()), this, SLOT(levelNotSaved()));
    connect(levelSett->saveBut, SIGNAL(clicked()), this, SLOT(saveToFile()));
    connect(levelSett->levelSelector, SIGNAL(levelToLoad()), this, SLOT(loadFromFile()));
//    connect(this, SIGNAL(accepted()), this, SLOT(acceptLevel()));

}

void examSettingsDlg::levelWasSelected(TexamLevel level) {
    if (isNotSaved)
        saveLevel();
    questSett->loadLevel(level);
    rangeSett->loadLevel(level);
}

void examSettingsDlg::levelNotSaved() {
    navList->item(0)->setIcon(QIcon(gl->path+"picts/notSaved.png"));
    setWindowTitle(examSettTxt + " (level not saved !!)");
}

void examSettingsDlg::saveLevel() {
    if ( QMessageBox::question(this, "", tr("Exam's level was changed\nand not saved !!"), QMessageBox::Save, QMessageBox::Cancel) == QMessageBox::Save ) {
        saveToFile();
    }
    levelSaved();
}

void examSettingsDlg::saveToFile() {
    TexamLevel newLevel;
    questSett->saveLevel(newLevel);
    rangeSett->saveLevel(newLevel);
    QString isLevelValid = validateLevel(newLevel);
    if (isLevelValid != "") {
        isLevelValid.prepend(tr("<b>It seems the level has got some mitakes:<b><br>"));
        QMessageBox::warning(this, "", isLevelValid);
        return;
    }
    TlevelHeaderWdg *saveDlg = new TlevelHeaderWdg(this);
    QStringList nameList = saveDlg->getLevelName();
    newLevel.name = nameList[0];
    newLevel.desc = nameList[1];
  // Saving to file
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save exam's level"), QDir::toNativeSeparators(QDir::homePath()+"/"+newLevel.name), TlevelSelector::levelFilterTxt);
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_4_7);
        out << TlevelSelector::levelVersion << newLevel;
        levelSett->levelSelector->updateRecentLevels(fileName);
    }
    else
        QMessageBox::critical(this, "", tr("Cannot open file for writing\n%1").arg(qPrintable(file.errorString())));
    isNotSaved = false;

    levelSett->levelSelector->addLevel(newLevel);
    levelSett->levelSelector->selectLevel(); // select the last
    levelSaved();
}

void examSettingsDlg::levelSaved() {
    isNotSaved = false;
    navList->item(0)->setIcon(QIcon(gl->path+"picts/levelsSettings.png"));
    setWindowTitle(examSettTxt);
}

void examSettingsDlg::loadFromFile() {
    if (isNotSaved)
        saveLevel();
    levelSett->levelSelector->loadFromFile();
}

void examSettingsDlg::acceptLevel() {
//    mainLevel = levelSett->levelSelector->getSelectedLevel();
}

QString examSettingsDlg::validateLevel(TexamLevel &l) {
    QString res = "";
  // checking range
    if (l.loNote.getChromaticNrOfNote() > gl->hiString().getChromaticNrOfNote()+l.hiFret ||
        l.hiNote.getChromaticNrOfNote() < gl->loString().getChromaticNrOfNote()+l.loFret)
        res += tr("<li>Range of frets is beyond scale of this level</li>");

    if (res != "") {
        res.prepend("<ul>");
        res += "</ul>";
    }
    return res;

}
