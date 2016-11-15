// httpserver.cpp
//
// Integrated web server for rdbrowserd(8)
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

#include <errno.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <QUrl>

#include "config.h"
#include "db.h"
#include "httpserver.h"

HttpServer::HttpServer(QObject *parent)
  : WHHttpServer(parent)
{
}


void HttpServer::processFinishedData(int exit_code,QProcess::ExitStatus status)
{
  /*
  for(unsigned i=http_processes.size()-1;i>=0;i--) {
    if(http_processes.at(i)->state()==QProcess::NotRunning) {
      http_processes.at(i)->deleteLater();
      http_processes.erase(http_processes.begin()+i);
    }
  }
  */
}


void HttpServer::requestReceived(WHHttpConnection *conn)
{
  QStringList hdrs;
  QStringList values;
  QUrl url(conn->uri());

  if(authenticateUser(WEBSERVER_REALM,conn->authName(),conn->authPassword())) {
    if(url.path().toLower()=="/streamcut") {
      StreamCut(conn);
    }
    else {
      conn->sendResponse(404,"404 Not Found\n");
    }
  }
  else {
    hdrs.push_back("WWW-Authenticate");
    values.push_back(QString("Basic realm=\"")+WEBSERVER_REALM+"\"");
    conn->sendResponse(401,hdrs,values,"401 Unauthorized\n");
  }    
}


bool HttpServer::authenticateUser(const QString &realm,const QString &name,
				  const QString &passwd)
{
  /*
  printf("authenticateUser(%s,%s,%s)\n",
	 (const char *)realm.toUtf8(),
	 (const char *)name.toUtf8(),
	 (const char *)passwd.toUtf8());
  */
  if(name.isEmpty()) {
    return false;
  }
  QString sql=QString("select LOGIN_NAME from USERS where ")+
    "LOGIN_NAME='"+SqlQuery::escape(name)+"' && "+
    "PASSWORD='"+SqlQuery::escape(passwd)+"'";
  return SqlQuery::rows(sql)>0;
}


void HttpServer::StreamCut(WHHttpConnection *conn)
{
  bool ok=false;
  QUrl url(conn->uri());
  unsigned cartnum=0;
  int cutnum=-1;
  QStringList hdrs;
  QStringList values;
  QStringList f0;
  QStringList f1;

  //
  // Validate parameters
  //
  switch(conn->method()) {
  case WHHttpConnection::Get:
    cartnum=url.queryItemValue("cart").toUInt(&ok);
    if((!ok)||(cartnum<1)||(cartnum>999999)) {
      conn->sendResponse(400,"[invalid cart number]");
      return;
    }
    cutnum=url.queryItemValue("cut").toUInt(&ok);
    if((!ok)||(cutnum>999)) {
      conn->sendResponse(400,"[invalid cut number]");
      return;
    }
    break;

  case WHHttpConnection::Post:
    f0=QString(conn->body()).split("&");
    for(int i=0;i<f0.size();i++) {
      f1=f0.at(i).split("=");
      if(f1.size()==2) {
	if(f1.at(0)=="cart") {
	  cartnum=f1.at(1).toUInt(&ok);
	  if((!ok)||(cartnum<1)||(cartnum>999999)) {
	    conn->sendResponse(400,"invalid cart number");
	    return;
	  }
	}
	if(f1.at(0)=="cut") {
	  cutnum=f1.at(1).toUInt(&ok);
	  if((!ok)||(cutnum>999)) {
	    conn->sendResponse(400,"invalid cut number");
	    return;
	  }
	}
      }
    }
    break;

  case WHHttpConnection::Delete:
  case WHHttpConnection::Head:
  case WHHttpConnection::Put:
  case WHHttpConnection::None:
    hdrs.push_back("Allow");
    values.push_back("GET, POST");
    conn->sendResponse(405,hdrs,values);
  }
  if(cartnum==0) {
    conn->sendResponse(400,"missing cart number");
  }
  if(cutnum<0) {
    conn->sendResponse(400,"missing cut number");
  }

  //
  // Check user permissions
  //
  QString sql=QString("select CART.NUMBER from CART ")+
    "left join USER_PERMS "+
    "on CART.GROUP_NAME=USER_PERMS.GROUP_NAME where "+
    QString().sprintf("CART.NUMBER=%u && ",cartnum)+
    "USER_PERMS.USER_NAME='"+SqlQuery::escape(conn->authName())+"'";
  if(SqlQuery::rows(sql)==0) {
    conn->sendResponse(404,"Not Found");
    return;
  }

  //
  // Start the stream
  //
  QStringList args;
  args.push_back("--server-type=icestreamer");
  args.push_back("--audio-format=aacp");
  args.push_back("--audio-samplerate=48000");
  args.push_back("--audio-channels=2");
  args.push_back("--audio-bitrate=48");
  args.push_back("--audio-device=FILE");
  args.push_back("--file-name="+QString().sprintf("/var/snd/%06u_%03d.wav",
						  cartnum,cutnum));
  args.push_back("--server-pipe=/home/fredg/socks/pipe");
  args.push_back("--server-exit-on-last");
  http_processes.push_back(new QProcess(this));
  connect(http_processes.back(),SIGNAL(finished(int,QProcess::ExitStatus)),
	  this,SLOT(processFinishedData(int,QProcess::ExitStatus)));
  http_processes.back()->start("glasscoder",args);
  if(!http_processes.back()->waitForStarted()) {
    conn->sendResponse(500,"500 Unable to start stream server\n");
    return;
  }

  //
  // Hand off to client connection
  //
  if(!SendSocket(conn,"/home/fredg/socks/pipe")) {
    //    http_processes.back()->terminate();
    //    conn->sendResponse(500,"500 Unable to hand off to stream server\n");
  }
}


bool HttpServer::SendSocket(WHHttpConnection *conn,const QString &unixpath)
{
  char buf[1]={1};
  struct sockaddr_un sa;
  struct msghdr msg;
  struct iovec iov[1];
  union {
    struct cmsghdr cm;
    char control[CMSG_SPACE(sizeof(int))];
  } control_un;
  struct cmsghdr *cmptr;
  int sendsock=-1;

  //
  // Open UNIX Socket Connection
  //
  memset(&sa,0,sizeof(sa));
  sa.sun_family=AF_UNIX;
  strncpy(sa.sun_path,unixpath.toUtf8(),108);

  if((sendsock=socket(AF_UNIX,SOCK_STREAM,0))<0) {
    syslog(LOG_WARNING,"unable to initialize UNIX socket");
    return false;
  }
  int started=-1;
  for(int i=0;i<200;i++) {  // 2 second timeout
    if((started=::connect(sendsock,(const struct sockaddr *)(&sa),sizeof(sa)))==0) {
      break;
    }
    usleep(10000);
  }
  if(started<0) {
    syslog(LOG_WARNING,"unable to connect to \"%s\" [%s]",
	   (const char *)unixpath.toUtf8(),strerror(errno));
    return false;
  }

  //
  // Send Socket Descriptor
  //
  send(sendsock,buf,1,MSG_NOSIGNAL);

  memset(&msg,0,sizeof(msg));
  memset(iov,0,sizeof(struct iovec));

  msg.msg_control=control_un.control;
  msg.msg_controllen=sizeof(control_un.control);
  cmptr=CMSG_FIRSTHDR(&msg);
  cmptr->cmsg_len=CMSG_LEN(sizeof(int));
  cmptr->cmsg_level=SOL_SOCKET;
  cmptr->cmsg_type=SCM_RIGHTS;
  *((int *) CMSG_DATA(cmptr))=conn->socket()->socketDescriptor();
  
  iov[0].iov_base=buf;
  iov[0].iov_len=1;
  msg.msg_iov=iov;
  msg.msg_iovlen=1;
  
  sendmsg(sendsock,&msg,MSG_NOSIGNAL);
  conn->socket()->disconnectFromHost();

  //
  // Clean Up
  //
  close(sendsock);

  return true;
}
