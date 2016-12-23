// mpeg1decoder.cpp
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

#include <string.h>
#include <unistd.h>

#include "mpeg1decoder.h"

Mpeg1Decoder::Mpeg1Decoder(Ringbuffer *rb,bool *is_running)
{
  mpeg1_ring=rb;
  mpeg1_is_running=is_running;

  mad_stream_init(&mpeg1_mad_stream);
  mad_synth_init(&mpeg1_mad_synth);
  mad_frame_init(&mpeg1_mad_frame);
  memset(&mpeg1_mad_header,0,sizeof(mpeg1_mad_header));
}


Mpeg1Decoder::~Mpeg1Decoder()
{
  mad_frame_finish(&mpeg1_mad_frame);
  mad_synth_finish(&mpeg1_mad_synth);
  mad_stream_finish(&mpeg1_mad_stream);
}


bool Mpeg1Decoder::addData(const char *data,unsigned long len)
{
  float pcm[32768*4];
  int frame_offset=0;
  int err_count=0;

  mpeg1_mpeg.append(data,len);
  mad_stream_buffer(&mpeg1_mad_stream,(const unsigned char *)mpeg1_mpeg.data(),
		    mpeg1_mpeg.length());
  do {
    while(mad_frame_decode(&mpeg1_mad_frame,&mpeg1_mad_stream)==0) {  
      mad_synth_frame(&mpeg1_mad_synth,&mpeg1_mad_frame);
      for(int i=0;i<mpeg1_mad_synth.pcm.length;i++) {
	for(int j=0;j<mpeg1_mad_synth.pcm.channels;j++) {
	  pcm[frame_offset+i*mpeg1_mad_synth.pcm.channels+j]=
	    (float)mad_f_todouble(mpeg1_mad_synth.pcm.samples[j][i]);
	}
      }
      frame_offset+=(mpeg1_mad_synth.pcm.length*mpeg1_mad_synth.pcm.channels);
    }
    if(MAD_RECOVERABLE(mpeg1_mad_stream.error)!=0) {
      if(err_count++>10) {
	return false;
      }
    }
  } while(mpeg1_mad_stream.error!=MAD_ERROR_BUFLEN);
  if(mpeg1_mad_stream.error==MAD_ERROR_BUFLEN) {
    if(frame_offset>0) {
      mpeg1_mad_header=mpeg1_mad_frame.header;
      if(!WriteRingbuffer(pcm,frame_offset*sizeof(float))) {
	return false;
      }
    }
    mpeg1_mpeg=
      mpeg1_mpeg.right(mpeg1_mad_stream.bufend-mpeg1_mad_stream.next_frame);
  }

  return true;
}


void Mpeg1Decoder::finish()
{
  float pcm[32768*4];
  int frame_offset=0;
  int err_count=0;

  mpeg1_mpeg.append(0,MAD_BUFFER_GUARD);
  mad_stream_buffer(&mpeg1_mad_stream,(const unsigned char *)mpeg1_mpeg.data(),
		    mpeg1_mpeg.length());
  do {
    while(mad_frame_decode(&mpeg1_mad_frame,&mpeg1_mad_stream)==0) {  
      mad_synth_frame(&mpeg1_mad_synth,&mpeg1_mad_frame);
      for(int i=0;i<mpeg1_mad_synth.pcm.length;i++) {
	for(int j=0;j<mpeg1_mad_synth.pcm.channels;j++) {
	  pcm[frame_offset+i*mpeg1_mad_synth.pcm.channels+j]=
	    (float)mad_f_todouble(mpeg1_mad_synth.pcm.samples[j][i]);
	}
      }
      frame_offset+=(mpeg1_mad_synth.pcm.length*mpeg1_mad_synth.pcm.channels);
    }
    if(MAD_RECOVERABLE(mpeg1_mad_stream.error)!=0) {
      if(err_count++>10) {
	return;
      }
    }
  } while(mpeg1_mad_stream.error!=MAD_ERROR_BUFLEN);
  WriteRingbuffer(pcm,frame_offset*sizeof(float));
}


bool Mpeg1Decoder::WriteRingbuffer(float *pcm,unsigned long bytes)
{
  while(mpeg1_ring->writeSpace()<bytes) {
    if(!(*mpeg1_is_running)) {
      return false;
    }
    usleep(10000);
  }
  mpeg1_ring->write((char *)pcm,bytes);
  return true;
}
