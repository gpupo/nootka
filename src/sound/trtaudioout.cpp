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


#include "trtaudioout.h"
#include "taudioparams.h"
#include <QDebug>
#include <QTimer>
#include <unistd.h>


#if defined(Q_OS_WIN32)
  #include <windows.h>
  #define SLEEP(msecs) Sleep(msecs)
#else
  #define SLEEP(msecs) usleep(msecs * 1000)
#endif


/*static*/
QStringList TaudioOUT::getAudioDevicesList() {
    QStringList devList;
    RtAudio *rta = getRtAudio();
    int devCnt = rta->getDeviceCount();
    if (devCnt < 1)
        return devList;
    for (int i = 0; i < devCnt; i++) {
        RtAudio::DeviceInfo devInfo;
        try {
          devInfo = rta->getDeviceInfo(i);
        }
        catch (RtError& e) {
          qDebug() << "error when probing output device" << i;
//           e.printMessage();
          continue;
        }
        if (devInfo.probed && devInfo.outputChannels > 0)
          devList << QString::fromLocal8Bit(devInfo.name.data());
    }
    delete rta;
    return devList;
}


int TaudioOUT::m_samplesCnt = 0;
unsigned int TaudioOUT::m_bufferFrames = 1024;
int TaudioOUT::m_maxCBloops = 44100 / m_bufferFrames * 2;
TaudioOUT* TaudioOUT::instance = 0;


int TaudioOUT::outCallBack(void* outBuffer, void* inBuffer, unsigned int nBufferFrames, double streamTime, 
                           RtAudioStreamStatus status, void* userData) {
  if ( status )
    qDebug() << "Stream underflow detected!";
  m_samplesCnt++;
//   if (m_samplesCnt == 0)
//     qDebug("outCallBack");
  if (m_samplesCnt < m_maxCBloops - 10) {
      qint16 *out = (qint16*)outBuffer;
      int off = m_samplesCnt * (nBufferFrames / instance->ratioOfRate);
      qint16 sample;
      for (int i = 0; i < nBufferFrames / instance->ratioOfRate; i++) {
          sample = instance->oggScale->getSample(off + i);
          for (int r = 0; r < instance->ratioOfRate; r++) {
              *out++ = sample; // left channel
              *out++ = sample; // right channel
          }
      }
      return 0;
  } 
  else
      return 1;
}
/*end static*/

//---------------------------------------------------------------------------------------
//                CONSTRUCTOR
//---------------------------------------------------------------------------------------
TaudioOUT::TaudioOUT(TaudioParams *_params, QString &path, QObject *parent) :
  TabstractPlayer(parent),
  TrtAudioAbstract(_params),
  oggScale(new ToggScale(path)),
  ratioOfRate(1)
{
  setAudioOutParams(_params);
  instance = this;
  offTimer = new QTimer();
  connect(offTimer, SIGNAL(timeout()), this, SLOT(stopSlot()));
}


TaudioOUT::~TaudioOUT() 
{
  delete streamOptions;
  delete rtDevice;
  delete offTimer;
  delete oggScale;
}


void TaudioOUT::setAudioOutParams(TaudioParams* params) {
  setUseJACK(params->useJACK);
  if (deviceName != params->OUTdevName || !rtDevice) {
    playable = oggScale->loadAudioData();
    if (playable && setAudioDevice(params->OUTdevName))
        playable = true;
    else
        playable = false;
  }
}

bool TaudioOUT::setAudioDevice(QString &name) {
  if (!rtDevice)
    rtDevice = getRtAudio();
  int devId = -1;
  int devCount = rtDevice->getDeviceCount();
  if (devCount) {
    RtAudio::DeviceInfo devInfo;
    for(int i = 0; i < devCount; i++) { // Is there device on the list ??
        if (getDeviceInfo(devInfo , i)) {        
          if (devInfo.probed) {
            if (QString::fromStdString(devInfo.name) == name) { // Here it is !!
              devId = i;
              break;
            }
          }
        }
    }
    if (devId == -1) { // no device on the list - load default
        devId = rtDevice->getDefaultOutputDevice();
        if (rtDevice->getDeviceInfo(devId).outputChannels <= 0) {
          qDebug("wrong default output device");
          playable = false;
          return false;
        }
    }
  }
  RtAudio::DeviceInfo devInfo;
  if (!getDeviceInfo(devInfo, devId)) {
    playable = false;
    return false;
  }
  determineSampleRate(devInfo);
  streamParams.deviceId = devId;
  streamParams.nChannels = 2;
  streamParams.firstChannel = 0;
  if (rtDevice->getCurrentApi() == RtAudio::UNIX_JACK) {
    if (!streamOptions)
      streamOptions = new RtAudio::StreamOptions;
    streamOptions->streamName = "nootkaOUT";
  }
//   printSupportedFormats(devInfo);
  printSupportedSampleRates(devInfo);
//   sampleRate = devInfo.sampleRates.at(devInfo.sampleRates.size() - 1);
//   sampleRate = 192000;  
  ratioOfRate = sampleRate / 44100;
  if (ratioOfRate > 1) { // from here sample rate is sampleRate * ratioOfRate
    if (sampleRate == 88200 || sampleRate == 176400)
      sampleRate = 44100;
    else if (sampleRate == 96000 || sampleRate == 192000)
      sampleRate = 48000;
    m_bufferFrames = m_bufferFrames * ratioOfRate; // increase buffer to give time for resampling in oggScale
  }
  oggScale->setSampleRate(sampleRate);
  oggScale->setPitchOffset(audioParams->a440diff);
  if (!openStream(&streamParams, NULL, RTAUDIO_SINT16, sampleRate * ratioOfRate, &m_bufferFrames, &outCallBack, 0, streamOptions)) {
      playable = false;
      return false;
  }
  if (rtDevice->isStreamOpen()) {
//       m_maxCBloops = (sampleRate / (m_bufferFrames / 2) * (44100.0f / (float)sampleRate)) * ratioOfRate;
      m_maxCBloops = (88200 * ratioOfRate) / m_bufferFrames;
      qDebug() << "RtOUT:" << QString::fromStdString(rtDevice->getDeviceInfo(devId).name) << "samplerate:" << sampleRate * ratioOfRate 
            << "buffer:" << m_bufferFrames;
      deviceName = QString::fromStdString(rtDevice->getDeviceInfo(devId).name);
      return true;
  } else
    return false;
}


bool TaudioOUT::play(int noteNr) {
  if (!playable)
      return false;
  
  if (offTimer->isActive())
      offTimer->stop();
  
  noteNr = noteNr + int(audioParams->a440diff);
  if (noteNr < -11 || noteNr > 41)
      return false;
  
  openStream(&streamParams, NULL, RTAUDIO_SINT16, sampleRate * ratioOfRate, &m_bufferFrames, &outCallBack, 0, streamOptions);
  
  doEmit = true;
  m_samplesCnt = -1;
  oggScale->setNote(noteNr);
  int loops = 0;
  while (!oggScale->isReady() && loops < 40) { // 40ms - max latency
      SLEEP(1);
      loops++;
  }
  if (loops)
      qDebug() << "latency:" << loops << "ms";
  
  offTimer->start(1600);
  return startStream();
}


void TaudioOUT::stop() {
  if (offTimer->isActive()) {
    offTimer->stop();
    doEmit = false;
    stopSlot();
  }
}


void TaudioOUT::stopSlot() {
  offTimer->stop();
  if (rtDevice->getCurrentApi() == RtAudio::LINUX_PULSE)
    stopStream();
  else
    closeStram();
  if (doEmit)
    emit noteFinished();
}

