// httpserver.h
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

#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <vector>

#include <QProcess>

#include <wh/whhttpserver.h>

class HttpServer : public WHHttpServer
{
  Q_OBJECT
 public:
  HttpServer(QObject *parent=0);

 private slots:
  void processFinishedData(int exit_code,QProcess::ExitStatus status);

 protected:
  void requestReceived(WHHttpConnection *conn);
  bool authenticateUser(const QString &realm,const QString &name,
			const QString &passwd);

 private:
  void StreamCut(WHHttpConnection *conn);
  bool SendSocket(WHHttpConnection *conn,const QString &unixpath);
  std::vector<QProcess *> http_processes;
};


#endif  // HTTPSERVER_H
