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

  fclose(f);
  rename((DEFAULT_CONF_FILE+"-new").toUtf8(),DEFAULT_CONF_FILE.toUtf8());

  return true;
}
