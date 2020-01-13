/** This file is part of Nootka (http://nootka.sf.net)               *
 * Copyright (C) 2020 by Tomasz Bojczuk (seelook@gmail.com)          *
 * on the terms of GNU GPLv3 license (http://www.gnu.org/licenses)   */

import QtQuick 2.9
import QtQuick.Controls 2.2

import Nootka 1.0


Rectangle {
  id: menuButton

  property Taction action
  property alias text: butText.text
  property alias iconHeight: icon.height
  property alias containsMouse: ma.containsMouse
  property alias containsPress: ma.containsPress
  property alias textColor: butText.color

  width: parent.width - Noo.fontSize()
  implicitHeight: Noo.fontSize() * 2.8
  anchors.horizontalCenter: parent.horizontalCenter
  color: ma.containsPress ? Noo.alpha(activPal.highlight, 50) : "transparent"
  enabled: !action || action.enabled

  signal clicked()

  onActionChanged: {
    if (action && action.checkable)
      radioComp.createObject(menuButton)
  }

  Image {
    id: icon
    source: action ? action.icon : ""
    height: Noo.fontSize() * 2.2; width: height * (sourceSize.width / sourceSize.height)
    anchors.verticalCenter: parent.verticalCenter
    visible: menuButton.enabled
  }
  Text {
    id: butText
    x: Noo.fontSize() * (action && (action.icon !== "" || action.checkable) ? 4.0 : 0.8)
    anchors.verticalCenter: parent.verticalCenter
    text: action ? action.text : ""
    font { pixelSize: Noo.fontSize() * 1.1 }
    color: enabled ? activPal.text : disdPal.text
    width: parent.width - x - Noo.fontSize()
    fontSizeMode: Text.Fit; minimumPixelSize: Noo.fontSize() / 2; minimumPointSize: minimumPixelSize
    elide: Text.ElideRight
    textFormat: Text.StyledText
  }

  Component {
    id: radioComp
    TcheckBox {
      anchors.verticalCenter: parent.verticalCenter
      checked: menuButton.action.checked
      onClicked: buttonClicked()
      x: (Noo.fontSize() * 3.5 - width) / 2
    }
  }

  MouseArea {
    id: ma
    anchors.fill: parent
//     hoverEnabled: true
    onClicked: buttonClicked()
//     onHoveredChanged: {
//       if (action) {
//         if (GLOB.showHints && action.tip !== "" && ma.containsMouse)
//           Noo.setStatusTip(action.tip, action.tipPos)
//         else
//           Noo.setStatusTip("", action.tipPos)
//       }
//     }
  }

  function buttonClicked() {
    menuButton.clicked()
    if (action)
      action.trigger()
  }
}
