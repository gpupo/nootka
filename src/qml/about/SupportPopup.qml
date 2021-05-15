/** This file is part of Nootka (http://nootka.sf.net)               *
 * Copyright (C) 2021 by Tomasz Bojczuk (seelook@gmail.com)          *
 * on the terms of GNU GPLv3 license (http://www.gnu.org/licenses)   */

import QtQuick 2.9

import "../"


TpopupDialog {

  property bool bigScr: NOO.shortScreenSide() / NOO.fingerPixels() >= 12 // 8,4 cm width, 7" screens

  caption: NOO.TR("TaboutNootka", "Support")

  visible: true; modal: false
  width: bigScr ?  parent.width * 0.9 : parent.width - NOO.factor()
  height: bigScr ? parent.height * 0.9 : parent.height - NOO.factor()

  acceptButton.text: NOO.TR("GotIt", "GOT IT!")
  rejectButton.visible: false

  bgColor: Qt.tint(activPal.base, NOO.alpha(activPal.highlight, 20))
  border { color: activPal.highlight; width: NOO.factor() / 4.0 }
  glowRect.radius: NOO.factor() * 2

  Rectangle {
    id: bgRect
    width: parent.width; height: parent.height
    color: NOO.alpha(activPal.base, 230)
    z: -1
    Image {
      anchors { right: parent.right; bottom: parent.bottom }
      source: NOO.pix("pane/support")
      height: parent.height / 2; width: height
      z: -1
    }
    SupportPage {}
  }

  onClosed: destroy()
}
