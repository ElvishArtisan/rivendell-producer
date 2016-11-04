// config.cpp
//
// Rivendell-Browser configuration
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

#include <stdio.h>

#include "config.h"
#include "profile.h"

Config *cnf; 

Config::Config()
{
}


QString Config::serverHostname() const
{
  return conf_server_hostname;
}


void Config::setServerHostname(const QString &str)
{
  conf_server_hostname=str;
}


QString Config::serverUsername() const
{
  return conf_server_username;
}


void Config::setServerUsername(const QString &str)
{
  conf_server_username=str;
}


QString Config::serverPassword() const
{
  return conf_server_password;
}


void Config::setServerPassword(const QString &str)
{
  conf_server_password=str;
}


QString Config::audioDeviceType() const
{
  return conf_audio_device_type;
}


void Config::setAudioDeviceType(const QString &str)
{
  conf_audio_device_type=str;
}


QString Config::audioDeviceName() const
{
  return conf_audio_device_name;
}


void Config::setAudioDeviceName(const QString &str)
{
  conf_audio_device_name=str;
}


QString Config::audioFormat() const
{
  return conf_audio_format;
}


void Config::setAudioFormat(const QString &str)
{
  conf_audio_format=str;
}


unsigned Config::audioSampleRate() const
{
  return conf_audio_sample_rate;
}


void Config::setAudioSampleRate(unsigned rate)
{
  conf_audio_sample_rate=rate;
}


unsigned Config::audioBitRate() const
{
  return conf_audio_bit_rate;
}


void Config::setAudioBitRate(unsigned rate)
{
  conf_audio_bit_rate=rate;
}


unsigned Config::audioChannels() const
{
  return conf_audio_channels;
}


void Config::setAudioChannels(unsigned chans)
{
  conf_audio_channels=chans;
}


bool Config::load()
{
  Profile *p=new Profile();
  bool ret=p->setSource(DEFAULT_CONF_FILE);

  conf_server_hostname=
    p->stringValue("Server","Hostname",DEFAULT_SERVER_HOSTNAME);
  conf_server_username=
    p->stringValue("Server","Username",DEFAULT_SERVER_USERNAME);
  conf_server_password=
    p->stringValue("Server","Password",DEFAULT_SERVER_PASSWORD);

  conf_audio_device_type=
    p->stringValue("Audio","DeviceType",DEFAULT_AUDIO_DEVICE_TYPE);
  conf_audio_device_name=
    p->stringValue("Audio","DeviceName",DEFAULT_AUDIO_DEVICE_NAME);
  conf_audio_format=p->stringValue("Audio","Format",DEFAULT_AUDIO_FORMAT);
  conf_audio_sample_rate=
    p->intValue("Audio","SampleRate",DEFAULT_AUDIO_SAMPLE_RATE);
  conf_audio_bit_rate=
    p->intValue("Audio","BitRate",DEFAULT_AUDIO_BIT_RATE);
  conf_audio_channels=p->intValue("Audio","Channels",DEFAULT_AUDIO_CHANNELS);

  return ret;
}


bool Config::save()
{
  FILE *f=NULL;

  if((f=fopen((DEFAULT_CONF_FILE+"-new").toUtf8(),"r"))==NULL) {
    return false;
  }
  fprintf(f,"[Server]\n");
  fprintf(f,"Hostname=%s\n",(const char *)conf_server_hostname.toUtf8());
  fprintf(f,"Username=%s\n",(const char *)conf_server_username.toUtf8());
  fprintf(f,"Password=%s\n",(const char *)conf_server_password.toUtf8());
  fprintf(f,"\n");

  fprintf(f,"[Audio]\n");
  fprintf(f,"DeviceType=%s\n",(const char *)conf_audio_device_type.toUtf8());
  fprintf(f,"DeviceName=%s\n",(const char *)conf_audio_device_name.toUtf8());
  fprintf(f,"Format=%s\n",(const char *)conf_audio_format.toUtf8());
  fprintf(f,"SampleRate=%u\n",conf_audio_sample_rate);
  fprintf(f,"BitRate=%u\n",conf_audio_bit_rate);
  fprintf(f,"Channels=%u\n",conf_audio_channels);
  fprintf(f,"\n");

  fclose(f);
  rename((DEFAULT_CONF_FILE+"-new").toUtf8(),DEFAULT_CONF_FILE.toUtf8());

  return true;
}
