/***************************************************************************
 *   Copyright (C) 2017 by Tomasz Bojczuk                                  *
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
 *  You should have received a copy of the GNU General Public License	     *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 ***************************************************************************/

import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3

import "fakeTrans.js" as Fake


Dialog {
  title: qsTr("About Nootka")
  visible: true
//  width: pages.width; height: pages.height

  width: nootkaWindow.width * 0.75; height: nootkaWindow.height * 0.75

  PagesDialog {
    id: pages
    anchors.fill: parent
    model: ListModel { id: pageModel }
    Component.onCompleted: {
      pageModel.append({"iconName": "nootka-frame", "buttonText": qsTr("About"), "page": "About"})
      pageModel.append({"iconName": "help-frame", "buttonText": Fake.tr("QShortcut", "Help"), "page": "Help"})
      pageModel.append({"iconName": "author", "buttonText": qsTr("Authors"), "page": "Authors"})
      pageModel.append({"iconName": "license", "buttonText": qsTr("License"), "page": "License"})
      pageModel.append({"iconName": "support", "buttonText": qsTr("Support"), "page": "Support"})
      pageModel.append({"iconName": "donors", "buttonText": qsTr("Donors", "Would be 'Sponsors' or even 'Backers' - translate as such as You fill, what sounds/looks better in Your language"), "page": "Donors"})
      pageModel.append({"iconName": "chlog", "buttonText": qsTr("Changes"), "page": "Changes"})
      pageModel.append({"iconName": "qt", "buttonText": "Qt", "page": "Qt"})
    }
  }

}
