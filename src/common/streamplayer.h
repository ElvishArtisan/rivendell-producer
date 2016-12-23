// streamplayer.h
//
// Abstract stream player class
//
//   (C) Copyright 2015-2016 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef STREAMPLAYER_H
#define STREAMPLAYER_H

#include <QObject>

#include "config.h"
#include "mpeg1decoder.h"

class StreamPlayer : public QObject
{
 Q_OBJECT;
 public:
  enum State {Stopped=0,Playing=1};
  StreamPlayer(Config *c,QObject *parent=0);
  State state() const;

 signals:
  void stateChanged(StreamPlayer::State state);
  void error(const QString &msg);

 public slots:
  void play(int cartnum,int cutnum,int start_pos,int end_pos);
  void stop();

 protected:
  virtual void startDevice(const QString &url,int start_pos,int end_pos)=0;
  virtual void stopDevice()=0;
  void setState(State state);
  void sendError(const QString &msg);
  Config *config() const;

 private:
  QString stream_next_url;
  int stream_next_start_pos;
  int stream_next_end_pos;
  State stream_state;
  Config *stream_config;
};




class StreamPlayerHeader {
public:
  StreamPlayerHeader();
  ~StreamPlayerHeader();
  void reset();
  int istate;
  uint16_t fmt_format;
  uint16_t fmt_channels;
  uint32_t fmt_samprate;
  uint32_t fmt_avgbytes;
  uint16_t fmt_blockalign;
  uint16_t fmt_bits;
  uint32_t data_bytes;
  QString chunk_name;
  uint32_t chunk_len;
  QByteArray chunk_data;
  uint32_t left_to_skip;
  Mpeg1Decoder *mpeg1_decoder;
  void *priv;
};


int StreamPlayerParseHeader(StreamPlayerHeader *hdr,void *buf,unsigned len);
void StreamPlayerPcm24ToPcm32(const char *pcm24,int *pcm32,unsigned samples);

#endif  // STREAMPLAYER_H
