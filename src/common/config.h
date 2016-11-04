// config.h
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

#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

#define DEFAULT_CONF_FILE QString("/etc/rivendell-browser.conf")
#define DEFAULT_SERVER_HOSTNAME "localhost"
#define DEFAULT_SERVER_USERNAME "user"
#define DEFAULT_SERVER_PASSWORD ""

#define DEFAULT_AUDIO_DEVICE_TYPE QString("ALSA")
#define DEFAULT_AUDIO_DEVICE_NAME QString("plughw:0")
#define DEFAULT_AUDIO_FORMAT QString("MP2")
#define DEFAULT_AUDIO_SAMPLE_RATE 48000
#define DEFAULT_AUDIO_BIT_RATE 192000
#define DEFAULT_AUDIO_CHANNELS 2

class Config
{
 public:
  Config();
  QString serverHostname() const;
  void setServerHostname(const QString &str);
  QString serverUsername() const;
  void setServerUsername(const QString &str);
  QString serverPassword() const;
  void setServerPassword(const QString &str);
  QString audioDeviceType() const;
  void setAudioDeviceType(const QString &str);
  QString audioDeviceName() const;
  void setAudioDeviceName(const QString &str);
  QString audioFormat() const;
  void setAudioFormat(const QString &str);
  unsigned audioSampleRate() const;
  void setAudioSampleRate(unsigned rate);
  unsigned audioBitRate() const;
  void setAudioBitRate(unsigned rate);
  unsigned audioChannels() const;
  void setAudioChannels(unsigned chans);
  bool load();
  bool save();

 private:
  QString conf_server_hostname;
  QString conf_server_username;
  QString conf_server_password;
  QString conf_audio_device_type;
  QString conf_audio_device_name;
  QString conf_audio_format;
  unsigned conf_audio_sample_rate;
  unsigned conf_audio_bit_rate;
  unsigned conf_audio_channels;
};

extern Config *cnf; 

#endif  // CONFIG_H
