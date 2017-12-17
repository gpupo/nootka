/** This file is part of Nootka (http://nootka.sf.net)               *
 * Copyright (C) 2017 by Tomasz Bojczuk (seelook@gmail.com)          *
 * on the terms of GNU GPLv3 license (http://www.gnu.org/licenses)   */

import QtQuick 2.9
import QtGraphicalEffects 1.0


Item {
  property alias color: bg.color
  property alias radius: bg.radius
  property bool rised: true
  property alias shadowRadius: shadow.radius

  Rectangle {
    id: bg
    anchors.fill: parent
    color: activPal.base
    radius: Noo.fontSize() / 4
    visible: false
  }

  DropShadow {
    id: shadow
    anchors.fill: bg
    horizontalOffset: rised ? Noo.fontSize() / 4 : 0
    verticalOffset: rised ? Noo.fontSize() / 4 : 0
    radius: 8.0
    samples: 17
    color: activPal.shadow
    source: bg
  }
}
