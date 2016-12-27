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

#include <QCoreApplication>
#include <QSettings>

#include "config.h"
#include "profile.h"

Config *cnf; 

Config::Config()
{
  QCoreApplication::setOrganizationName("Paravel Systems");
  QCoreApplication::setOrganizationDomain("paravelsystems.com");
  QCoreApplication::setApplicationName("rivendell-browser");
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


QString Config::audioDeviceName() const
{
  return conf_audio_device_name;
}


void Config::setAudioDeviceName(const QString &str)
{
  conf_audio_device_name=str;
}


bool Config::load()
{
#ifdef WIN32
  QSettings s;
  conf_server_hostname=
    s.value("Server/Hostname",DEFAULT_SERVER_HOSTNAME).toString();
  conf_server_username=
    s.value("Server/Username",DEFAULT_SERVER_USERNAME).toString();
  conf_server_password=
    s.value("Server/Password",DEFAULT_SERVER_PASSWORD).toString();

  conf_audio_device_name=
    s.value("Audio/DeviceName",DEFAULT_AUDIO_DEVICE_NAME).toString();

  return true;
#else
  Profile *p=new Profile();
  bool ret=p->setSource(DEFAULT_CONF_FILE);

  conf_server_hostname=
    p->stringValue("Server","Hostname",DEFAULT_SERVER_HOSTNAME);
  conf_server_username=
    p->stringValue("Server","Username",DEFAULT_SERVER_USERNAME);
  conf_server_password=
    p->stringValue("Server","Password",DEFAULT_SERVER_PASSWORD);

  conf_audio_device_name=
    p->stringValue("Audio","DeviceName",DEFAULT_AUDIO_DEVICE_NAME);

  return ret;
#endif  // WIN32
}


bool Config::save()
{
#ifdef WIN32
  QSettings s;
  s.setValue("Server/Hostname",conf_server_hostname);
  s.setValue("Server/Username",conf_server_username);
  s.setValue("Server/Password",conf_server_password);

  s.setValue("Audio/DeviceName",conf_audio_device_name);

  return true;
#else
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
  fprintf(f,"DeviceName=%s\n",(const char *)conf_audio_device_name.toUtf8());
  fprintf(f,"\n");

  fclose(f);
  rename((DEFAULT_CONF_FILE+"-new").toUtf8(),DEFAULT_CONF_FILE.toUtf8());

  return true;
#endif  // WIN32
}
