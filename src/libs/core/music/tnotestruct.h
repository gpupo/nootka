/***************************************************************************
 *   Copyright (C) 2014 by Tomasz Bojczuk                                  *
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

#ifndef TNOTESTRUCT_H
#define TNOTESTRUCT_H


#include <nootkacoreglobal.h>
#include <cmath>
#include "tnote.h"

/** 
 * Structure that stores pitch and its parameters as such as frequency and duration in [s].
 * It also stores numbers of start/end chunk of a note.
 * It may return average pitch of all registered pitches of a played note
 * @p totalAverage()
 * or average pitch of 'inner' pitches - a few pitch values from the middle of a note
 * (usually pitches of 3 to 6 chunk - usable for guitar)
 * @p shortAverage()
 */
class NOOTKACORE_EXPORT TnoteStruct
{

public:
	TnoteStruct(const Tnote& p, qreal pF, qreal f = 0.0, qreal dur = 0.0) : pitch(p), pitchF(pF), freq(f), duration(dur) {}
	
	TnoteStruct() {	freq = 0; duration = 0; pitchF = 0;	} /** Default constructor  */


      /** Initializes and cleans @class TnoteStruct values  */
	void init(int _index, int chunkNr, qreal floatPitch) {
    startChunk = chunkNr;
    endChunk = chunkNr;
    pitchF = floatPitch;
    index = _index;
    maxVol = 0.0f;
    minVol = 1.0f;
    m_totalAver = 0.0;
    m_shortAver = 0.0;
    m_pList.clear();
    m_pList << floatPitch;
    bestPitch = 0.0;
  }

      /** Updates values. Calculates minimal and maximal volume already occurred */
  void update(int chunkNr, qreal floatPitch, float vol) {
    if (numChunks() == 2) {
      pitchF = floatPitch; // is better to take base pitch not from first chunks
      basePitch = qRound(pitchF);
    }
    m_pList << floatPitch;
    endChunk = chunkNr;
    maxVol = qMax<float>(maxVol, vol);
    if (numChunks() > 3) // skip first 3 chunks - Tartini may detected a note with low volume
      minVol = qMin<float>(minVol, vol);
    if (qAbs<qreal>(floatPitch - (qreal)basePitch) < qAbs<qreal>(bestPitch - (qreal)basePitch))
      bestPitch = floatPitch;
  }

      /** Summarizes a note. Sets duration in seconds [s].
      * Given parameter @p chunkTime is duration of single chunk in seconds.
      * It sets frequency of a note from @p bestPitch value.  */
  void sumarize(qreal chunkTime) {
    freq = pitchToFreq(bestPitch);
    duration = numChunks() * chunkTime;
    pitchF = bestPitch;
  }

			/** Checks is float value of a note pitch different than its root pitch in range of given threshold.
			 * Returns @p TRUE if @p pitchF value is into threshold range or @p FALSE when not
			 * This way intonation accuracy is checked */
	bool inTune(float threshold) {
		if (qAbs(pitchF - (float)qRound(pitchF)) >= threshold)
			return false;
		else
			return true;
	}
	
	void set(qreal midiPitch = 0.0, qreal f = 0.0, qreal dur = 0.0) {
		midiPitch ? pitch = Tnote(qRound(midiPitch) - 47) : pitch = Tnote(); pitchF = midiPitch; freq = f; duration = dur;
	} /** Sets values of note structure, or sets them to NULL if no values defined. */
	
	int     index;            /** Note index in entire channel */
	Tnote   pitch; 		        /** Note pitch like C, D, E or C3, D#  */
	qreal   pitchF; 	        /** Chromatic note number in MIDI scale, C1 = 60 */
	qreal   bestPitch;        /** Closest value to rounded (perfect) pitch among all occurred pitches. */
	int     basePitch;        /** Midi value (rounded to integer) of a pitch */
	qreal   freq; 		        /** Frequency of a note */
	qreal   duration;         /** Duration of a note */
	int     startChunk;       /** Chunk in which note was noticed */
	int     endChunk;         /** Last chunk with this note */
	float   maxVol;           /** Loudest volume occurred */
	float   minVol;           /** Less volume occurred */

	int     numChunks() { return endChunk - startChunk + 1; } /** Note duration in chunks */

	qreal   totalAverage() { if (m_totalAver == 0.0) getTotalAverage(); return m_totalAver; } /** Average pitch of all chunk pitches */
	qreal   shortAverage() { if (m_shortAver == 0.0) getShortAverage(); return m_shortAver; } /** Average pitch of all chunk pitches */
	qreal   averageFreq() { return pitchToFreq(totalAverage()); } /** Average frequency of total average pitch */
	qreal   averageShortFreq() { return pitchToFreq(shortAverage()); } /** Average frequency of total average pitch */

	QList<qreal>* pitches() { return &m_pList; } /** Pointer to list of pitches */

      /** Static method that converts given pitch to a frequency (in Hz) */
	static qreal pitchToFreq(qreal midiPitch) { return std::pow(10.0, (midiPitch + 36.3763165622959152488) / 39.8631371386483481); }

      /** Returns average pitch in given chunks range. First chunk is 1. */
	qreal getAverage(unsigned int start, unsigned int stop) {
    qreal sum = 0.0;
    int cnt = 0;
    for (int i = qMin<int>(start - 1, m_pList.size() - 1); i < qMin<int>(m_pList.size(), stop); ++i) {
      sum += m_pList[i];
      cnt++;
    }
    return sum / cnt;
	}

private:
  QList<qreal>            m_pList;
  qreal                   m_totalAver, m_shortAver;

  void    getTotalAverage() {
    m_totalAver = getAverage(1, m_pList.size());
  }

  void getShortAverage() {
    m_shortAver = getAverage(3, 6);
  }
};


#endif // TNOTESTRUCT_H
