// mpeg1decoder.h
//
// Decoder class for MPEG-1 audio streams
//
//   (C) Copyright 2016 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as
//   published by the Free Software Foundation; either version 2 of
//   the License, or (at your option) any later version.
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

#ifndef MPEG1DECODER_H
#define MPEG1DECODER_H

#include <mad.h>

#include <QByteArray>

#include "ringbuffer.h"

class Mpeg1Decoder
{
 public:
  Mpeg1Decoder(Ringbuffer *rb,bool *is_running);
  ~Mpeg1Decoder();
  bool addData(const char *data,unsigned long len);
  void finish();

 private:
  bool WriteRingbuffer(float *pcm,unsigned long bytes);
  Ringbuffer *mpeg1_ring;
  bool *mpeg1_is_running;
  struct mad_stream mpeg1_mad_stream;
  struct mad_frame mpeg1_mad_frame;
  struct mad_synth mpeg1_mad_synth;
  struct mad_header mpeg1_mad_header;
  QByteArray mpeg1_mpeg;
};


#endif  // MPEG1DECODER_H
