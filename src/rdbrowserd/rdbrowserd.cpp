// rdbrowserd.cpp
//
// Audio streaming daemon for Rivendell-browser
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

#include <syslog.h>

#include <QCoreApplication>

#include "cmdswitch.h"
#include "config.h"
#include "db.h"
#include "rdbrowserd.h"

MainObject::MainObject(QObject *parent)
  : QObject(parent)
{
  QString err_msg;

  //
  // Read Command Options
  //
  CmdSwitch *cmd=new CmdSwitch("walltime-panel",VERSION,RDBROWSERD_USAGE);
  for(unsigned i=0;i<cmd->keys();i++) {
  }
  delete cmd;

  //
  // Configuration
  //
  cnf=new Config();
  cnf->load();

  //
  // Open Syslog
  //
  openlog("rdbrowserd",LOG_PERROR,LOG_DAEMON);

  //
  // Open Database
  //
  if(!OpenDb(&err_msg)) {
    syslog(LOG_ERR,"%s",(const char *)err_msg.toUtf8());
    exit(256);
  }

  //
  // Web Server
  //
  main_http_server=new HttpServer(this);
  if(!main_http_server->listen(cnf->webserverPort())) {
    syslog(LOG_ERR,"unable to bind port %u",cnf->webserverPort());
    exit(256);
  }
  syslog(LOG_DEBUG,"listening for HTTP requests on port %u",
	 cnf->webserverPort());
}


int main(int argc,char *argv[])
{
  QCoreApplication a(argc,argv);

  new MainObject();
  return a.exec();
}
