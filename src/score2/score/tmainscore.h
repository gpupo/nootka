/***************************************************************************
 *   Copyright (C) 2013 by Tomasz Bojczuk                                  *
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

#ifndef TMAINSCORE_H
#define TMAINSCORE_H

#include "tsimplescore.h"

class TmainScore : public TsimpleScore
{
    Q_OBJECT

public:
    TmainScore(int noteNumber, QWidget *parent = 0);
    ~TmainScore();
		
				/** Sets scordature to value kept in Tglobal. */
    void setScordature();
		
protected slots:
				/** Refresh some things (scordature) after switch. */
		void onPianoSwitch();

private:

};

#endif // TMAINSCORE_H
