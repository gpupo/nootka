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


#ifndef TUPDATEPROCESS_H
#define TUPDATEPROCESS_H

#include <QObject>

class QTimer;
class QProcess;


/** Performs update checking calling standalone nootka-updater.
 * signal updateOutput emits output messages generated by nootka-updater.
 * m_timer is conuting 7 sec.
 * When updated is successfull (version was checked) timer is stopped
 * and user gets dialog with info
 * if some error occured proces is killed
*/
class TupdateProcess : public QObject
{
  
  Q_OBJECT
  
public:
    TupdateProcess(bool respectRules, QObject *parent = 0);
    virtual ~TupdateProcess();
    
    void start();
        /** Returns TRUE when nootka-updater file exist. */
    static bool isPossible();
    
signals:
    void updateOutput(QString);
    
    
protected slots:
  void processTimeOut();
  void processSays();
  
private:
  bool m_respectRules;
  QProcess *m_process;
  QTimer *m_timer;
  QString m_exec;
  
};

#endif // TUPDATEPROCESS_H
