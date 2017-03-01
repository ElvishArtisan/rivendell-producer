// streamplayer_mme.cpp
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
#include <unistd.h>

#include <curl/curl.h>
#include <samplerate.h>

#include <QByteArray>

#include "streamplayer_mme.h"

#define SND_PCM_FORMAT_S16_LE 1
#define SND_PCM_FORMAT_S24_3LE 2
#define SND_PCM_FORMAT_FLOAT 3

#ifdef MME
QString __StreamPlayerMme_MmeError(MMRESULT err)
{
  char err_msg[200];

  if(waveOutGetErrorText(err,err_msg,200)==0) {
    return QString(err_msg);
  }
  return "Unknown MME error"+QString().sprintf(" [%d]",err);
}


void CALLBACK __StreamPlayerMmeCallback(HWAVEOUT hwo,UINT umsg,
					DWORD_PTR instance,  
					DWORD_PTR param1,DWORD_PTR param2)
{
  WAVEHDR *hdr=(WAVEHDR *)param1;

  switch(umsg) {
  case WOM_DONE:
    hdr->dwUser=false;
    break;
  }
}
#endif  // MME


bool __StreamPlayerMmeOpenPlayback(StreamPlayerHeader *hdr)
{
#ifdef MME
  MmeData *mme_data=(MmeData *)hdr->priv;
  MMRESULT merr;
  WAVEFORMATEX wfx;

  //
  // Open the output device
  //
  memset(&wfx,0,sizeof(wfx));
  wfx.wFormatTag=WAVE_FORMAT_PCM;
  wfx.nChannels=hdr->fmt_channels;
  wfx.nSamplesPerSec=hdr->fmt_samprate;
  wfx.nBlockAlign=hdr->fmt_channels*sizeof(int16_t);
  wfx.nAvgBytesPerSec=hdr->fmt_samprate*hdr->fmt_channels*sizeof(int16_t);
  wfx.wBitsPerSample=sizeof(int16_t)*8;

  if((merr=waveOutOpen(&mme_data->mme_handle,mme_data->mme_device_id,&wfx,
		       (DWORD_PTR)__StreamPlayerMmeCallback,
		       0,CALLBACK_FUNCTION))!=0) {
    mme_data->err_msg=__StreamPlayerMme_MmeError(merr);
    return false;
  }

  //
  // Allocate Buffers
  //
  for(int i=0;i<MME_PERIOD_QUAN;i++) {
    mme_data->mme_headers[i].lpData=
      (LPSTR)new int16_t[MME_BUFFER_SIZE*hdr->fmt_channels];
    mme_data->mme_headers[i].dwBufferLength=
      MME_BUFFER_SIZE*hdr->fmt_channels*sizeof(int16_t);
    if((merr=waveOutPrepareHeader(mme_data->mme_handle,
				  &(mme_data->mme_headers[i]),
				  sizeof(mme_data->mme_headers[i])))!=0) {
      mme_data->err_msg=__StreamPlayerMme_MmeError(merr);
      return false;
    }
  }

  //
  // Start Playout
  //
  for(int i=0;i<MME_PERIOD_QUAN;i++) {
    memset(mme_data->
	   mme_headers[i].lpData,0,mme_data->mme_headers[i].dwBufferLength);
    mme_data->mme_headers[i].dwUser=true;
    waveOutWrite(mme_data->mme_handle,&(mme_data->mme_headers[i]),
		 sizeof(mme_data->mme_headers[i]));
  }
  return true;
#else
  return false;
#endif  // MME
}


#ifdef MME
void __StreamPlayerMme_ClosePlayback(MmeData *mme_data)
{
  for(int i=0;i<MME_PERIOD_QUAN;i++) {
    while(mme_data->mme_headers[i].dwUser) {
      usleep(10000);
    }
    waveOutUnprepareHeader(mme_data->mme_handle,
			   &(mme_data->mme_headers[i]),
			   sizeof(mme_data->mme_headers[i]));
  }
  waveOutClose(mme_data->mme_handle);
  mme_data->mme_handle=NULL;
}
#endif  // MME


void *__StreamPlayerMme_MmeCallback(void *priv)
{
#ifdef MME
  static StreamPlayerHeader *hdr=NULL;
  static MmeData *mme_data=NULL;
  static unsigned hdrptr;
  static short *pcm16=NULL;
  static char *pcm24=NULL;
  static int *pcm32=NULL;
  static float *pcm=NULL;

  hdr=(StreamPlayerHeader *)priv;
  mme_data=(MmeData *)hdr->priv;

  pcm16=new short[MME_BUFFER_SIZE*hdr->fmt_channels];
  pcm24=new char[MME_BUFFER_SIZE*hdr->fmt_channels*3];
  pcm32=new int[MME_BUFFER_SIZE*hdr->fmt_channels];
  pcm=new float[MME_BUFFER_SIZE*hdr->fmt_channels];

  //
  // Wait for ring buffer to fill
  //
  switch(mme_data->ring_format) {
  case SND_PCM_FORMAT_S16_LE:
    while(mme_data->ring->readSpace()<MME_BUFFER_SIZE*MME_PERIOD_QUAN*sizeof(short)) {
      usleep(10000);
    }
    break;

  case SND_PCM_FORMAT_S24_3LE:
    while(mme_data->ring->readSpace()<MME_BUFFER_SIZE*MME_PERIOD_QUAN*3) {
      usleep(10000);
    }
    break;

  case SND_PCM_FORMAT_FLOAT:
    while(mme_data->ring->readSpace()<MME_BUFFER_SIZE*MME_PERIOD_QUAN*sizeof(float)) {
      usleep(10000);
    }
    break;
  }

  //
  // Main Loop
  //
  while(mme_data->running&&(mme_data->ring->readSpace()>0)) {
    //
    // Convert incoming PCM data to FLOAT
    //
    switch(mme_data->ring_format) {
    case SND_PCM_FORMAT_S16_LE:
      memset(pcm16,0,
	    MME_BUFFER_SIZE*hdr->fmt_channels*sizeof(short));
      mme_data->ring->read((char *)pcm16,
	  MME_BUFFER_SIZE*sizeof(short)*hdr->fmt_channels);
      src_short_to_float_array(pcm16,pcm,
		       MME_BUFFER_SIZE*hdr->fmt_channels);
      break;

    case SND_PCM_FORMAT_S24_3LE:
      mme_data->ring->read(pcm24,
	  MME_BUFFER_SIZE*3*hdr->fmt_channels);
      StreamPlayerPcm24ToPcm32(pcm24,pcm32,
	  MME_BUFFER_SIZE*hdr->fmt_channels);
      src_int_to_float_array(pcm32,pcm,
	  MME_BUFFER_SIZE*hdr->fmt_channels);
      break;

    case SND_PCM_FORMAT_FLOAT:
      mme_data->ring->read((char *)pcm,
	  MME_BUFFER_SIZE*sizeof(float)*hdr->fmt_channels);
      break;
    }

    //
    // Convert FLOAT data to card-specific format
    //
    hdrptr=mme_data->mme_current_header%MME_PERIOD_QUAN;
    while(mme_data->mme_headers[hdrptr].dwUser) {
      usleep(10000);
    }
    src_float_to_short_array(pcm,(short *)mme_data->mme_headers[hdrptr].lpData,
			     MME_BUFFER_SIZE*hdr->fmt_channels);

    //
    // Play it
    //
    mme_data->mme_headers[hdrptr].dwUser=true;
    waveOutWrite(mme_data->mme_handle,&(mme_data->mme_headers[hdrptr]),
		 sizeof(mme_data->mme_headers[hdrptr]));
    ++mme_data->mme_current_header;
  }

  //
  // Clean up
  //
  delete pcm;
  delete pcm16;
  delete pcm24;
  delete pcm32;

#endif  // MME
  return NULL;
}


size_t __StreamPlayerMme_CurlWriteCallback(char *ptr,size_t size,size_t nmemb,
					    void *userdata)
{
#ifdef MME
  static unsigned long len=0;
  static StreamPlayerMme *dev=NULL;
  static StreamPlayerHeader *hdr=NULL;
  static MmeData *mme_data=NULL;
  static int data_start=0;
  static pthread_attr_t pthread_attr;

  len=size*nmemb;
  dev=(StreamPlayerMme *)userdata;
  hdr=(StreamPlayerHeader *)dev->mme_hdr;
  mme_data=(MmeData *)dev->mme_data;
  mme_data->err_msg="";

  if(mme_data->mme_handle==NULL) {
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
	  mme_data->ring_format=SND_PCM_FORMAT_S16_LE;
	  break;

	case 24:
	  mme_data->ring_format=SND_PCM_FORMAT_S24_3LE;
	  break;

	default:
	mme_data->err_msg=
	  QString().sprintf("unsupported PCM bit depth [%d]",hdr->fmt_bits);
	}
	break;

      case WAVE_FORMAT_MPEG:
	mme_data->ring_format=SND_PCM_FORMAT_FLOAT;
	hdr->mpeg1_decoder=
	  new Mpeg1Decoder(mme_data->ring,&mme_data->running);
	break;

      default:
	mme_data->err_msg="No audio available";
	return 0;
      }
      if(hdr->mpeg1_decoder==NULL) {
	mme_data->ring->write((char *)ptr+data_start,len-data_start);
      }
      else {
	hdr->mpeg1_decoder->addData(ptr+data_start,len-data_start);
      }
      hdr->data_bytes-=(len-data_start);
      if(!__StreamPlayerMmeOpenPlayback(hdr)) {
	mme_data->err_msg=QString().sprintf("unable to start MME device %d",
					    mme_data->mme_device_id);
	return 0;
      }
      mme_data->running=true;
      pthread_attr_init(&pthread_attr);
      pthread_attr_setschedpolicy(&pthread_attr,SCHED_FIFO);
      pthread_create(&mme_data->mme_pthread,&pthread_attr,
		     __StreamPlayerMme_MmeCallback,hdr);
      dev->mme_state=StreamPlayerMme::Playing;
    }
  }
  else {
    if(len>hdr->data_bytes) {
      len=hdr->data_bytes;
    }
    if(hdr->mpeg1_decoder==NULL) {
      while(mme_data->ring->writeSpace()<len) {
	if(!mme_data->running) {
	  return 0;
	}
	usleep(10000);
      }
      mme_data->ring->write(ptr,len);
    }
    else {
      if(!hdr->mpeg1_decoder->addData(ptr,len)) {
	return 0;
      }
    }
    hdr->data_bytes-=len;
  }
#endif  // MME
  return size*nmemb;
}


void *__StreamPlayerMme_CurlThread(void *priv)
{
#ifdef MME
  static StreamPlayerMme *dev=NULL;
  static CURL *curl=NULL;
  static CURLcode curl_code;
  static long response_code=0;

  dev=(StreamPlayerMme *)priv;
  curl=curl_easy_init();
  curl_easy_setopt(curl,CURLOPT_URL,dev->mme_url.toUtf8().constData());
  curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,
		   __StreamPlayerMme_CurlWriteCallback);
  curl_easy_setopt(curl,CURLOPT_WRITEDATA,dev);
  curl_easy_setopt(curl,CURLOPT_NOPROGRESS,1);

  if((curl_code=curl_easy_perform(curl))!=CURLE_OK) {
    switch(curl_code) {
    case CURLE_WRITE_ERROR:
      if(dev->mme_data->mme_handle!=NULL) {
	pthread_join(dev->mme_data->mme_pthread,NULL);
	waveOutClose(dev->mme_data->mme_handle);
	dev->mme_data->mme_handle=NULL;
	dev->mme_state=StreamPlayerMme::Stopped;
      }
      else {
	dev->mme_state_error_string=dev->mme_data->err_msg;
	dev->mme_state=StreamPlayerMme::Error;
      }
      break;

    default:
      dev->mme_state_error_string=
	QString().sprintf("CURL Error [%d]: "+curl_code)+
      curl_easy_strerror(curl_code);
      break;
    }
    curl_easy_cleanup(curl);
    __StreamPlayerMme_ClosePlayback(dev->mme_data);
    dev->mme_state=StreamPlayerMme::Error;
    return NULL;
  }
  curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE,&response_code);
  if((response_code<200)||(response_code>299)) {
    curl_easy_cleanup(curl);
    dev->mme_state_error_string=
      QString().sprintf("Audio store returned code: %ld",response_code);
    __StreamPlayerMme_ClosePlayback(dev->mme_data);
    dev->mme_state=StreamPlayerMme::Error;
    return NULL;
  }

  curl_easy_cleanup(curl);
  pthread_join(dev->mme_data->mme_pthread,NULL);
  __StreamPlayerMme_ClosePlayback(dev->mme_data);
  dev->mme_state=StreamPlayerMme::Stopped;
#endif  // MME
  return NULL;
}




#ifdef MME
MmeData::MmeData()
{
  mme_device_id=0;
  mme_handle=NULL;
  memset(mme_headers,0,sizeof(WAVEHDR)*MME_PERIOD_QUAN);
  mme_current_header=0;
  ring_format=0;
  ring=new Ringbuffer(262144);
  running=false;
}


MmeData::~MmeData()
{
  delete ring;
}
#endif  // MME



StreamPlayerMme::StreamPlayerMme(Config *c,QObject *parent)
  : StreamPlayer(c,parent)
{
#ifdef MME
  mme_state=StreamPlayerMme::Stopped;
  mme_hdr=NULL;
  mme_data=NULL;

  mme_state_timer=new QTimer(this);
  connect(mme_state_timer,SIGNAL(timeout()),this,SLOT(stateData()));
#endif  // MME
}


StreamPlayerMme::~StreamPlayerMme()
{
}


void StreamPlayerMme::stateData()
{
#ifdef MME
  switch(mme_state) {
  case StreamPlayerMme::Starting:
    break;

  case StreamPlayerMme::Playing:
    setState(StreamPlayer::Playing);
    break;

  case StreamPlayerMme::Stopped:
    mme_state=StreamPlayerMme::Stopped;
    FreeMultithread();
    setState(StreamPlayer::Stopped);
    break;

  case StreamPlayerMme::Error:
      if(!mme_state_error_string.isEmpty()) {
      sendError(mme_state_error_string);
      mme_state_error_string="";
    }
    mme_state=StreamPlayerMme::Stopped;
    FreeMultithread();
    setState(StreamPlayer::Stopped);
    break;
  }
#endif  // MME
}


void StreamPlayerMme::startDevice(const QString &url,int start_pos,int end_pos)
{
#ifdef MME
  mme_url=url;
  mme_start_pos=start_pos;
  mme_end_pos=end_pos;

  CreateMultithread();

  mme_state=StreamPlayerMme::Starting;
#endif  // MME
}


void StreamPlayerMme::stopDevice()
{
#ifdef MME
  mme_data->running=false;
#endif  // MME
}


#ifdef MME
void StreamPlayerMme::CreateMultithread()
{
  pthread_attr_t pthread_attr;

  mme_hdr=new StreamPlayerHeader();
  mme_data=new MmeData();
  mme_data->mme_device_id=config()->audioDeviceName().toUInt();
  mme_hdr->priv=mme_data;
  pthread_attr_init(&pthread_attr);
  pthread_create(&mme_curl_thread,&pthread_attr,__StreamPlayerMme_CurlThread,
		 this);
  mme_state_timer->start(50);
}


void StreamPlayerMme::FreeMultithread()
{
  mme_state_timer->stop();
  pthread_join(mme_curl_thread,NULL);
  delete mme_data;
  mme_data=NULL;
  delete mme_hdr;
  mme_hdr=NULL;
}
#endif  // MME
