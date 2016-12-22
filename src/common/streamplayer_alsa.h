// streamplayer_alsa.h
//
// Stream player class for GlassPlayer
//
//   (C) Copyright 2016 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License version 2 as
//   published by the Free Software Foundation.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public
//   License along with this program; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#ifndef STREAMPLAYER_ALSA_H
#define STREAMPLAYER_ALSA_H

#include <pthread.h>

#ifdef ALSA
#include <alsa/asoundlib.h>
#endif  // ALSA

#include <QTimer>

#include "ringbuffer.h"
#include "streamplayer.h"

#define ALSA_PERIOD_QUANTITY 4

class AlsaData {
 public:
  AlsaData();
  ~AlsaData();
  snd_pcm_t *pcm;
  unsigned alsa_format;
  unsigned alsa_samprate;
  unsigned alsa_channels;
  unsigned alsa_xfer_frames;
  unsigned alsa_periods;
  snd_pcm_uframes_t alsa_buffer_size;
  char *alsa_buffer;
  Ringbuffer *ring;
  pthread_t alsa_pthread;
  bool running;
  char err_msg[200];
};




class StreamPlayerAlsa : public StreamPlayer
{
 Q_OBJECT;
 public:
  StreamPlayerAlsa(Config *c,QObject *parent=0);
  ~StreamPlayerAlsa();

 private slots:
  void stateData();

 protected:
  void startDevice(const QString &url,int start_pos,int end_pos);
  void stopDevice();

 private:
  enum State {Stopped=0,Starting=1,Playing=2,Error=3};
  void CreateMultithread();
  void FreeMultithread();
  QString alsa_url;
  int alsa_start_pos;
  int alsa_end_pos;
  pthread_t alsa_curl_thread;
  QTimer *alsa_state_timer;
  State alsa_state;
  StreamPlayerHeader *alsa_hdr;
  AlsaData *alsa_data;
  QString alsa_state_error_string;
  friend void *__StreamPlayerAlsa_CurlThread(void *priv);
  friend size_t __StreamPlayerAlsa_CurlWriteCallback(char *ptr,size_t size,
						     size_t nmemb,
						     void *userdata);
};


#endif  // STREAMPLAYER_ALSA_H
