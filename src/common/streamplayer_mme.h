// streamplayer_mme.h
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

#ifndef STREAMPLAYER_MME_H
#define STREAMPLAYER_MME_H

#include <pthread.h>

#ifdef MME
#include <windows.h>
#include <mmreg.h>
#include <mmsystem.h>
#endif  // MME

#include <QTimer>

#include "ringbuffer.h"
#include "streamplayer.h"

#define MME_BUFFER_SIZE 2048
#define MME_PERIOD_QUAN 4

#ifdef MME
class MmeData {
 public:
  MmeData();
  ~MmeData();
  unsigned mme_device_id;
  HWAVEOUT mme_handle;
  WAVEHDR mme_headers[MME_PERIOD_QUAN];
  unsigned mme_current_header;
  pthread_t mme_pthread;
  unsigned ring_format;
  Ringbuffer *ring;
  bool running;
  QString err_msg;
};
#endif  // MME



class StreamPlayerMme : public StreamPlayer
{
 Q_OBJECT;
 public:
  StreamPlayerMme(Config *c,QObject *parent=0);
  ~StreamPlayerMme();

 private slots:
  void stateData();

 protected:
  void startDevice(const QString &url,int start_pos,int end_pos);
  void stopDevice();

 private:
#ifdef MME
  enum State {Stopped=0,Starting=1,Playing=2,Error=3};
  void CreateMultithread();
  void FreeMultithread();
  QString mme_url;
  int mme_start_pos;
  int mme_end_pos;
  pthread_t mme_curl_thread;
  QTimer *mme_state_timer;
  State mme_state;
  StreamPlayerHeader *mme_hdr;
  MmeData *mme_data;
  QString mme_state_error_string;
  friend void *__StreamPlayerMme_CurlThread(void *priv);
  friend size_t __StreamPlayerMme_CurlWriteCallback(char *ptr,size_t size,
						     size_t nmemb,
						     void *userdata);

#endif  // MME
};


#endif  // STREAMPLAYER_MME_H
