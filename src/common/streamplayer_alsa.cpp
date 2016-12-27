// streamplayer_alsa.cpp
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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <curl/curl.h>
#include <samplerate.h>

#include <QByteArray>

#include "streamplayer_alsa.h"

#define WAVE_FORMAT_PCM 0x0001
#define WAVE_FORMAT_MPEG 0x0050

bool __StreamPlayerOpenPlayback(StreamPlayerHeader *hdr)
{
#ifdef ALSA
  snd_pcm_hw_params_t *hwparams;
  snd_pcm_sw_params_t *swparams;
  int dir;
  int aerr;
  AlsaData *alsa_data=(AlsaData *)hdr->priv;
  
  if(snd_pcm_open(&alsa_data->pcm,alsa_data->alsa_device.toUtf8(),
		  SND_PCM_STREAM_PLAYBACK,0)!=0) {
    alsa_data->err_msg=
      "unable to open ALSA device \""+alsa_data->alsa_device+"\"";
    return false;
  }
  snd_pcm_hw_params_alloca(&hwparams);
  snd_pcm_hw_params_any(alsa_data->pcm,hwparams);

  //
  // Access Type
  //
  if(snd_pcm_hw_params_test_access(alsa_data->pcm,hwparams,
				   SND_PCM_ACCESS_RW_INTERLEAVED)<0) {
    alsa_data->err_msg="interleaved access not supported";
    return false;
  }
  snd_pcm_hw_params_set_access(alsa_data->pcm,hwparams,
			       SND_PCM_ACCESS_RW_INTERLEAVED);

  //
  // Sample Format
  //
  if(snd_pcm_hw_params_set_format(alsa_data->pcm,hwparams,
				  SND_PCM_FORMAT_S32_LE)==0) {
    alsa_data->alsa_format=SND_PCM_FORMAT_S32_LE;
  }
  else {
    if(snd_pcm_hw_params_set_format(alsa_data->pcm,hwparams,
				    SND_PCM_FORMAT_S16_LE)==0) {
      alsa_data->alsa_format=SND_PCM_FORMAT_S16_LE;
    }
    else {
      alsa_data->err_msg="incompatible sample format";
      return false;
    }
  }

  //
  // Sample Rate
  //
  alsa_data->alsa_samprate=hdr->fmt_samprate;
  snd_pcm_hw_params_set_rate_near(alsa_data->pcm,hwparams,
				  &alsa_data->alsa_samprate,&dir);

  //
  // Channels
  //
  alsa_data->alsa_channels=hdr->fmt_channels;
  snd_pcm_hw_params_set_channels_near(alsa_data->pcm,hwparams,
				      &alsa_data->alsa_channels);

  //
  // Buffer Parameters
  //
  alsa_data->alsa_periods=ALSA_PERIOD_QUANTITY;
  snd_pcm_hw_params_set_periods_near(alsa_data->pcm,hwparams,
				     &alsa_data->alsa_periods,&dir);
  alsa_data->alsa_buffer_size=4096;
  snd_pcm_hw_params_set_buffer_size_near(alsa_data->pcm,hwparams,
					 &alsa_data->alsa_buffer_size);
  alsa_data->alsa_xfer_frames=
    alsa_data->alsa_buffer_size/(ALSA_PERIOD_QUANTITY*2);

  //
  // Fire It Up
  //
  if((aerr=snd_pcm_hw_params(alsa_data->pcm,hwparams))<0) {
    alsa_data->err_msg=QString("ALSA device error 1: ")+snd_strerror(aerr);
    return false;
  }
  alsa_data->alsa_buffer=
    new char[alsa_data->alsa_buffer_size*alsa_data->alsa_channels*4];

  //
  // Set Wake-up Timing
  //
  snd_pcm_sw_params_alloca(&swparams);
  snd_pcm_sw_params_current(alsa_data->pcm,swparams);
  snd_pcm_sw_params_set_avail_min(alsa_data->pcm,swparams,
				  alsa_data->alsa_buffer_size/2);
  if((aerr=snd_pcm_sw_params(alsa_data->pcm,swparams))<0) {
    alsa_data->err_msg=QString("ALSA device error 2: ")+snd_strerror(aerr);
    return false;
  }

  return true;
#else
  return false;
#endif  // ALSA
}


void *__StreamPlayerAlsa_AlsaCallback(void *priv)
{
#ifdef ALSA
  static AlsaData *alsa_data=NULL;
  static short *pcm16=NULL;
  static char *pcm24=NULL;
  static int *pcm32=NULL;
  static float *pcm=NULL;

  alsa_data=(AlsaData *)priv;
  pcm16=new short[alsa_data->alsa_xfer_frames*alsa_data->alsa_channels];
  pcm24=new char[alsa_data->alsa_xfer_frames*alsa_data->alsa_channels*3];
  pcm32=new int[alsa_data->alsa_xfer_frames*alsa_data->alsa_channels];
  pcm=new float[alsa_data->alsa_xfer_frames*alsa_data->alsa_channels];

  //
  // Wait for ring buffer to fill
  //
  switch(alsa_data->ring_format) {
  case SND_PCM_FORMAT_S16_LE:
    while(alsa_data->ring->readSpace()<alsa_data->alsa_xfer_frames*ALSA_PERIOD_QUANTITY*sizeof(short)) {
      usleep(10000);
    }
    break;

  case SND_PCM_FORMAT_S24_3LE:
    while(alsa_data->ring->readSpace()<alsa_data->alsa_xfer_frames*ALSA_PERIOD_QUANTITY*3) {
      usleep(10000);
    }
    break;

  case SND_PCM_FORMAT_FLOAT:
    while(alsa_data->ring->readSpace()<alsa_data->alsa_xfer_frames*ALSA_PERIOD_QUANTITY*sizeof(float)) {
      usleep(10000);
    }
    break;
  }

  //
  // Main Loop
  //
  while(alsa_data->running&&(alsa_data->ring->readSpace()>0)) {
    //
    // Convert incoming PCM data to FLOAT
    //
    switch(alsa_data->ring_format) {
    case SND_PCM_FORMAT_S16_LE:
      memset(pcm16,0,
	    alsa_data->alsa_xfer_frames*alsa_data->alsa_channels*sizeof(short));
      alsa_data->ring->read((char *)pcm16,
	  alsa_data->alsa_xfer_frames*sizeof(short)*alsa_data->alsa_channels);
      src_short_to_float_array(pcm16,pcm,
		       alsa_data->alsa_xfer_frames*alsa_data->alsa_channels);
      break;

    case SND_PCM_FORMAT_S24_3LE:
      alsa_data->ring->read(pcm24,
	  alsa_data->alsa_xfer_frames*3*alsa_data->alsa_channels);
      StreamPlayerPcm24ToPcm32(pcm24,pcm32,
	  alsa_data->alsa_xfer_frames*alsa_data->alsa_channels);
      src_int_to_float_array(pcm32,pcm,
	  alsa_data->alsa_xfer_frames*alsa_data->alsa_channels);
      break;

    case SND_PCM_FORMAT_FLOAT:
      alsa_data->ring->read((char *)pcm,
	  alsa_data->alsa_xfer_frames*sizeof(float)*alsa_data->alsa_channels);
      break;
    }

    //
    // Convert FLOAT data to card-specific format
    //
    switch(alsa_data->alsa_format) {
    case SND_PCM_FORMAT_S32_LE:
      src_float_to_int_array(pcm,(int *)alsa_data->alsa_buffer,
			 alsa_data->alsa_xfer_frames*alsa_data->alsa_channels);
      break;

    case SND_PCM_FORMAT_S16_LE:
      src_float_to_short_array(pcm,(short *)alsa_data->alsa_buffer,
			 alsa_data->alsa_xfer_frames*alsa_data->alsa_channels);
      break;
    }

    //
    // Play it
    //
    if(snd_pcm_state(alsa_data->pcm)!=SND_PCM_STATE_RUNNING) {
      snd_pcm_drop(alsa_data->pcm);
      snd_pcm_prepare(alsa_data->pcm);
    }
    snd_pcm_writei(alsa_data->pcm,alsa_data->alsa_buffer,
		   alsa_data->alsa_xfer_frames);
  }

  //
  // Clean up
  //
  snd_pcm_drain(alsa_data->pcm);
  delete pcm;
  delete pcm16;
  delete pcm24;
  delete pcm32;

#endif  // ALSA
  return NULL;
}


size_t __StreamPlayerAlsa_CurlWriteCallback(char *ptr,size_t size,size_t nmemb,
					    void *userdata)
{
#ifdef ALSA
  static unsigned long len=0;
  static StreamPlayerAlsa *dev=NULL;
  static StreamPlayerHeader *hdr=NULL;
  static AlsaData *alsa_data=NULL;
  static int data_start=0;
  static pthread_attr_t pthread_attr;

  len=size*nmemb;
  dev=(StreamPlayerAlsa *)userdata;
  hdr=(StreamPlayerHeader *)dev->alsa_hdr;
  alsa_data=(AlsaData *)dev->alsa_data;
  alsa_data->err_msg="";

  if(alsa_data->pcm==NULL) {
    if((data_start=StreamPlayerParseHeader(hdr,ptr,len))>=0) {
      /*
      printf("  format: %u\n",0xFFFF&hdr->fmt_format);
      printf("  channels: %u\n",0xFFFF&hdr->fmt_channels);
      printf("  samprate: %u\n",hdr->fmt_samprate);
      printf("  avg bytes: %u\n",hdr->fmt_avgbytes);
      printf("  block align: %u\n",0xFFFF&hdr->fmt_blockalign);
      printf("  bits: %u\n",0xFFFF&hdr->fmt_bits);
      printf("  data_start: 0x%04X\n",data_start);
      */
      switch(hdr->fmt_format) {
      case WAVE_FORMAT_PCM:
	switch(hdr->fmt_bits) {
	case 16:
	  alsa_data->ring_format=SND_PCM_FORMAT_S16_LE;
	  break;

	case 24:
	  alsa_data->ring_format=SND_PCM_FORMAT_S24_3LE;
	  break;

	default:
	alsa_data->err_msg=
	  QString().sprintf("unsupported PCM bit depth [%d]",hdr->fmt_bits);
	}
	break;

      case WAVE_FORMAT_MPEG:
	alsa_data->ring_format=SND_PCM_FORMAT_FLOAT;
	hdr->mpeg1_decoder=
	  new Mpeg1Decoder(alsa_data->ring,&alsa_data->running);
	break;

      default:
	alsa_data->err_msg=
	  QString().sprintf("unsupported audio encoding [wFormatTag: %d]",
			    hdr->fmt_format);
	return 0;
      }
      if(hdr->mpeg1_decoder==NULL) {
	alsa_data->ring->write((char *)ptr+data_start,len-data_start);
      }
      else {
	hdr->mpeg1_decoder->addData(ptr+data_start,len-data_start);
      }
      hdr->data_bytes-=(len-data_start);
      if(!__StreamPlayerOpenPlayback(hdr)) {
	alsa_data->err_msg="unable to start ALSA device \""+
	  alsa_data->alsa_device+"\"";
	return 0;
      }
      alsa_data->running=true;
      pthread_attr_init(&pthread_attr);
      pthread_attr_setschedpolicy(&pthread_attr,SCHED_FIFO);
      pthread_create(&alsa_data->alsa_pthread,&pthread_attr,
		     __StreamPlayerAlsa_AlsaCallback,alsa_data);
      dev->alsa_state=StreamPlayerAlsa::Playing;
    }
  }
  else {
    if(len>hdr->data_bytes) {
      len=hdr->data_bytes;
    }
    if(hdr->mpeg1_decoder==NULL) {
      while(alsa_data->ring->writeSpace()<len) {
	if(!alsa_data->running) {
	  return 0;
	}
	usleep(10000);
      }
      alsa_data->ring->write(ptr,len);
    }
    else {
      if(!hdr->mpeg1_decoder->addData(ptr,len)) {
	return 0;
      }
    }
    hdr->data_bytes-=len;
  }
#endif  // ALSA
  return size*nmemb;
}


void *__StreamPlayerAlsa_CurlThread(void *priv)
{
#ifdef ALSA
  static StreamPlayerAlsa *dev=NULL;
  static CURL *curl=NULL;
  static CURLcode curl_code;
  static long response_code=0;

  dev=(StreamPlayerAlsa *)priv;
  curl=curl_easy_init();
  curl_easy_setopt(curl,CURLOPT_URL,dev->alsa_url.toUtf8().constData());
  curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,
		   __StreamPlayerAlsa_CurlWriteCallback);
  curl_easy_setopt(curl,CURLOPT_WRITEDATA,dev);
  curl_easy_setopt(curl,CURLOPT_NOPROGRESS,1);

  if((curl_code=curl_easy_perform(curl))!=CURLE_OK) {
    switch(curl_code) {
    case CURLE_WRITE_ERROR:
      if(dev->alsa_data->pcm!=NULL) {
	pthread_join(dev->alsa_data->alsa_pthread,NULL);
	snd_pcm_close(dev->alsa_data->pcm);
	dev->alsa_data->pcm=NULL;
	dev->alsa_state=StreamPlayerAlsa::Stopped;
      }
      else {
	dev->alsa_state_error_string=dev->alsa_data->err_msg;
	dev->alsa_state=StreamPlayerAlsa::Error;
      }
      break;

    default:
      dev->alsa_state_error_string=
	QString().sprintf("CURL Error [%d]: "+curl_code)+
      curl_easy_strerror(curl_code);
      break;
    }
    curl_easy_cleanup(curl);
    dev->alsa_state=StreamPlayerAlsa::Error;
    return NULL;
  }
  curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE,&response_code);
  if((response_code<200)||(response_code>299)) {
    curl_easy_cleanup(curl);
    dev->alsa_state_error_string=
      QString().sprintf("Audio store returned code: %ld",response_code);
    dev->alsa_state=StreamPlayerAlsa::Error;
    return NULL;
  }

  curl_easy_cleanup(curl);
  pthread_join(dev->alsa_data->alsa_pthread,NULL);
  snd_pcm_close(dev->alsa_data->pcm);
  dev->alsa_data->pcm=NULL;
  dev->alsa_state=StreamPlayerAlsa::Stopped;
#endif  // ALSA
  return NULL;
}




#ifdef ALSA
AlsaData::AlsaData()
{
  pcm=NULL;
  alsa_format=0;
  alsa_samprate=0;
  alsa_channels=0;
  alsa_xfer_frames=0;
  alsa_periods=0;
  alsa_buffer_size=0;
  alsa_buffer=NULL;
  ring=new Ringbuffer(262144);
  running=false;
}


AlsaData::~AlsaData()
{
  delete ring;
}
#endif  // ALSA



StreamPlayerAlsa::StreamPlayerAlsa(Config *c,QObject *parent)
  : StreamPlayer(c,parent)
{
#ifdef ALSA
  alsa_state=StreamPlayerAlsa::Stopped;
  alsa_hdr=NULL;
  alsa_data=NULL;

  alsa_state_timer=new QTimer(this);
  connect(alsa_state_timer,SIGNAL(timeout()),this,SLOT(stateData()));
#endif  // ALSA
}


StreamPlayerAlsa::~StreamPlayerAlsa()
{
}


void StreamPlayerAlsa::stateData()
{
#ifdef ALSA
  switch(alsa_state) {
  case StreamPlayerAlsa::Starting:
    break;

  case StreamPlayerAlsa::Playing:
    setState(StreamPlayer::Playing);
    break;

  case StreamPlayerAlsa::Stopped:
    alsa_state=StreamPlayerAlsa::Stopped;
    FreeMultithread();
    setState(StreamPlayer::Stopped);
    break;

  case StreamPlayerAlsa::Error:
      if(!alsa_state_error_string.isEmpty()) {
      sendError(alsa_state_error_string);
      alsa_state_error_string="";
    }
    alsa_state=StreamPlayerAlsa::Stopped;
    FreeMultithread();
    setState(StreamPlayer::Stopped);
    break;
  }
#endif  // ALSA
}


void StreamPlayerAlsa::startDevice(const QString &url,int start_pos,int end_pos)
{
#ifdef ALSA
  alsa_url=url;
  alsa_start_pos=start_pos;
  alsa_end_pos=end_pos;

  CreateMultithread();

  alsa_state=StreamPlayerAlsa::Starting;
#endif  // ALSA
}


void StreamPlayerAlsa::stopDevice()
{
#ifdef ALSA
  alsa_data->running=false;
#endif  // ALSA
}


#ifdef ALSA
void StreamPlayerAlsa::CreateMultithread()
{
  pthread_attr_t pthread_attr;

  alsa_hdr=new StreamPlayerHeader();
  alsa_data=new AlsaData();
  alsa_data->alsa_device=config()->audioDeviceName();
  alsa_hdr->priv=alsa_data;
  pthread_attr_init(&pthread_attr);
  pthread_create(&alsa_curl_thread,&pthread_attr,__StreamPlayerAlsa_CurlThread,
		 this);
  alsa_state_timer->start(50);
}


void StreamPlayerAlsa::FreeMultithread()
{
  alsa_state_timer->stop();
  pthread_join(alsa_curl_thread,NULL);
  delete alsa_data;
  alsa_data=NULL;
  delete alsa_hdr;
  alsa_hdr=NULL;
}
#endif  // ALSA
