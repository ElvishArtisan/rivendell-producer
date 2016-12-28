// streamplayer.cpp
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

#include <stdio.h>

#include <QMessageBox>
#include <QStringList>

#include "streamplayer.h"

StreamPlayerHeader::StreamPlayerHeader()
{
  istate=0;
  fmt_format=0;
  fmt_channels=0;
  fmt_samprate=0;
  fmt_blockalign=0;
  fmt_avgbytes=0;
  fmt_bits=0;
  chunk_len=0;
  mpeg1_decoder=NULL;
  left_to_skip=0;
}


StreamPlayerHeader::~StreamPlayerHeader()
{
  if(mpeg1_decoder!=NULL) {
    delete mpeg1_decoder;
  }
}


void StreamPlayerHeader::reset()
{
  chunk_name="";
  chunk_len=0;
  left_to_skip=0;
  chunk_data.clear();
}


int StreamPlayerParseHeader(StreamPlayerHeader *hdr,void *buf,unsigned len)
{
  uint8_t *ptr=(uint8_t *)buf;

  for(unsigned i=0;i<len;i++) {
    switch(hdr->istate) {
    case 0:   // RIFF Header
      if((0xFF&ptr[i])=='R') {
	hdr->istate++;
      }
      break;

    case 1:   // RIFF Header
      if((0xFF&ptr[i])=='I') {
	hdr->istate++;
      }
      break;

    case 2:   // RIFF Header
    case 3:   // RIFF Header
      if((0xFF&ptr[i])=='F') {
	hdr->istate++;
      }
      break;

    case 4:   // File Size
    case 5:   // File Size
    case 6:   // File Size
    case 7:   // File Size
      hdr->istate++;
      break;

    case 8:   // WAVE Header
      if((0xFF&ptr[i])=='W') {
	hdr->istate++;
      }
      break;
      
    case 9:   // WAVE Header
      if((0xFF&ptr[i])=='A') {
	hdr->istate++;
      }
      break;
      
    case 10:   // WAVE Header
      if((0xFF&ptr[i])=='V') {
	hdr->istate++;
      }
      break;
      
    case 11:   // WAVE Header
      if((0xFF&ptr[i])=='E') {
	hdr->istate++;
      }
      break;

    case 12:   // Start of Chunk
      hdr->reset();
      hdr->chunk_name+=0xFF&ptr[i];
      hdr->istate++;
      break;
      
    case 13:   // Chunk Name
    case 14:   // Chunk Name
    case 15:   // Chunk Name
      hdr->chunk_name+=0xFF&ptr[i];
      hdr->istate++;
      break;

    case 16:   // Chunk Size
      hdr->chunk_len=0xFF&ptr[i];
      hdr->istate++;
      break;

    case 17:   // Chunk Size
      hdr->chunk_len=hdr->chunk_len|((0xFF&ptr[i])<<8);
      hdr->istate++;
      break;

    case 18:   // Chunk Size
      hdr->chunk_len=hdr->chunk_len|((0xFF&ptr[i])<<16);
      hdr->istate++;
      break;

    case 19:   // Chunk Size
      hdr->chunk_len=hdr->chunk_len|((0xFF&ptr[i])<<24);
      /*
      printf("CHUNK: %s  LEN: %u\n",
	     (const char *)hdr->chunk_name.toUtf8(),
	     hdr->chunk_len);
      */
      hdr->left_to_skip=hdr->chunk_len;
      if(hdr->chunk_name.trimmed()=="fmt") {
	hdr->istate=50;
      }
      else {
	if(hdr->chunk_name.trimmed()=="data") {
	  hdr->data_bytes=hdr->chunk_len;
	  return i+1;
	}
	hdr->istate++;
      }
      break;

    case 20:   // Skip Chunk Data
      if(--hdr->left_to_skip==0) {
	hdr->istate=12;
      }
      break;

    case 50:   // FMT Chunk : Format Tag
      hdr->fmt_format=0xFF&ptr[i];
      hdr->istate++;
      break;

    case 51:   // FMT Chunk : Format Tag
      hdr->fmt_format=hdr->fmt_format|(0xFF&ptr[i])<<8;
      hdr->istate++;
      break;

    case 52:   // FMT Chunk : Channels
      hdr->fmt_channels=0xFF&ptr[i];
      hdr->istate++;
      break;

    case 53:   // FMT Chunk : Channels
      hdr->fmt_channels=hdr->fmt_channels|(0xFF&ptr[i])<<8;
      hdr->istate++;
      break;

    case 54:   // FMT Chunk : Sample Rate
      hdr->fmt_samprate=0xFF&ptr[i];
      hdr->istate++;
      break;

    case 55:   // FMT Chunk : Sample Rate
      hdr->fmt_samprate=hdr->fmt_samprate|(0xFF&ptr[i])<<8;
      hdr->istate++;
      break;

    case 56:   // FMT Chunk : Sample Rate
      hdr->fmt_samprate=hdr->fmt_samprate|(0xFF&ptr[i])<<16;
      hdr->istate++;
      break;

    case 57:   // FMT Chunk : Sample Rate
      hdr->fmt_samprate=hdr->fmt_samprate|(0xFF&ptr[i])<<24;
      hdr->istate++;
      break;

    case 58:   // FMT Chunk : Average Bytes per Second
      hdr->fmt_avgbytes=0xFF&ptr[i];
      hdr->istate++;
      break;

    case 59:   // FMT Chunk : Average Bytes per Second
      hdr->fmt_avgbytes=hdr->fmt_avgbytes|(0xFF&ptr[i])<<8;
      hdr->istate++;
      break;

    case 60:   // FMT Chunk : Average Bytes per Second
      hdr->fmt_avgbytes=hdr->fmt_avgbytes|(0xFF&ptr[i])<<16;
      hdr->istate++;
      break;

    case 61:   // FMT Chunk : Average Bytes per Second
      hdr->fmt_avgbytes=hdr->fmt_avgbytes|(0xFF&ptr[i])<<24;
      hdr->istate++;
      break;

    case 62:   // FMT Chunk : Block Alignment
      hdr->fmt_blockalign=0xFF&ptr[i];
      hdr->istate++;
      break;

    case 63:   // FMT Chunk : Block Alignment
      hdr->fmt_blockalign=hdr->fmt_blockalign|(0xFF&ptr[i])<<8;
      hdr->istate++;
      break;

    case 64:   // FMT Chunk : Bits per Sample
      hdr->fmt_bits=0xFF&ptr[i];
      hdr->istate++;
      break;

    case 65:   // FMT Chunk : Bits per Sample
      hdr->fmt_bits=hdr->fmt_bits|(0xFF&ptr[i])<<8;
      hdr->istate++;
      break;

    case 66:   // Extension Size
      hdr->left_to_skip=0xFF&ptr[i];
      hdr->istate++;
      break;

    case 67:   // Extension Size
      hdr->left_to_skip=hdr->left_to_skip|(0xFF&ptr[i])<<8;
      if(hdr->left_to_skip==0) {
	hdr->istate=12;
      }
      else {
	hdr->left_to_skip-=18;
	hdr->istate=20;
      }
      break;
    }
  }
  return 0;
}


void StreamPlayerPcm24ToPcm32(const char *pcm24,int *pcm32,unsigned samples)
{
  for(unsigned i=0;i<samples;i++) {
    pcm32[i]=((0xFF&pcm24[i*3])<<8)+
      ((0xFF&pcm24[i*3+1])<<16)+
      ((0xFF&pcm24[i*3+2])<<24);
  }
}





StreamPlayer::StreamPlayer(Config *c,QObject *parent)
  : QObject(parent)
{
  stream_state=StreamPlayer::Stopped;
  stream_config=c;
  stream_next_url="";
  stream_next_start_pos=-1;
  stream_next_end_pos=-1;
}


StreamPlayer::State StreamPlayer::state() const
{
  return stream_state;
}


void StreamPlayer::play(int cartnum,int cutnum,int start_pos,int end_pos)
{
  QString url="http://"+config()->serverHostname()+
    QString().sprintf("/snd/%06u_%03d.wav",cartnum,cutnum);
  switch(state()) {
  case StreamPlayer::Playing:
    stream_next_url=url;
    stream_next_start_pos=start_pos;
    stream_next_end_pos=end_pos;
    stopDevice();
    break;

  case StreamPlayer::Stopped:
    stream_next_url="";
    stream_next_start_pos=-1;
    stream_next_end_pos=-1;
    startDevice(url,start_pos,end_pos);
    break;
  }
}


void StreamPlayer::stop()
{
  stopDevice();
}


void StreamPlayer::setState(StreamPlayer::State state)
{
  if(state!=stream_state) {
    stream_state=state;
    emit stateChanged(stream_state);
    switch(state) {
    case StreamPlayer::Playing:
      break;

    case StreamPlayer::Stopped:
      if(!stream_next_url.isEmpty()) {
	startDevice(stream_next_url,stream_next_start_pos,stream_next_end_pos);
	stream_next_url="";
      }
      break;
    }
  }
}


void StreamPlayer::sendError(const QString &msg)
{
  emit error(msg);
}


Config *StreamPlayer::config() const
{
  return stream_config;
}
