// db.h
//
// Database methods.
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

#ifndef DB_H
#define DB_H

#include <QString>
#include <QSqlQuery>
#include <QTimer>
#include <QVariant>

#define DB_HEARTBEAT_INTERVAL 300000

extern bool OpenDb(QString *err_msg);

class SqlQuery : public QSqlQuery
{
 public:
  SqlQuery(const QString &query,bool log=false);
  int columns() const;
  static QVariant run(const QString &sql,bool *ok=NULL);
  static int rows(const QString &sql);
  static QString escape(const QString &str);

 private:
  int sql_columns;
};


class DbHeartbeat : public QObject
{
  Q_OBJECT;
 public:
  DbHeartbeat(QObject *parent=0,int interval=DB_HEARTBEAT_INTERVAL);
  ~DbHeartbeat();

 private slots:
  void timeoutData();

 private:
  QTimer *heart_timer;
};


#endif  // DB_H
