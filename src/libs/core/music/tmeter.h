/***************************************************************************
 *   Copyright (C) 2014-2016 by Tomasz Bojczuk                             *
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
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 ***************************************************************************/

#ifndef TMETER_H
#define TMETER_H

#include <nootkacoreglobal.h>
#include <QtCore/qobject.h>
#include <QtCore/qxmlstream.h>
#include <QtGui/qpixmap.h>


/**
 * Describes musical time signature
 */
class NOOTKACORE_EXPORT Tmeter
{

  Q_GADGET

public:

  enum Emeter {
    NoMeter = 0,
    Meter_2_4 = 1, Meter_3_4 = 2, Meter_4_4 = 4, Meter_5_4 = 8, Meter_6_4 = 16, Meter_7_4 = 32,
    Meter_3_8 = 64, Meter_5_8 = 128, Meter_6_8 = 256, Meter_7_8 = 512, Meter_9_8 = 1024, Meter_12_8 = 2048
  };

  Q_ENUM(Emeter)

  Tmeter(Emeter meter = NoMeter) : m_meter(meter) {}

  Emeter meter() const { return m_meter; }

  int upper() const; /**< Returns upper digit of time signature  */
  int lower() const; /**< Returns lower digit of time signature  */

    /** Returns numeric value representing duration of single measure,
     * which is based on Trhythm calculation (RVALUE)
     * 3/4 is 72, 4/4 is 96 (RVALUE), etc. */
  quint16 duration() const;

  QPixmap pixmap(int fontSize, const QColor& c = -1); /**< Returns QPixmap of this time signature with given font size and color. */

  void toXml(QXmlStreamWriter& xml) const;
  bool fromXml(QXmlStreamReader& xml);

  void debug(const QString& text = QString()); /**< Prints current meter to std out with given text */

  bool operator==(const Tmeter& m) const { return meter() == m.meter(); }
  bool operator!=(const Tmeter& m) const { return meter() != m.meter(); }

private:
  Emeter        m_meter;
};

#endif // TMETER_H