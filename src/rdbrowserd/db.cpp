// gnsqlquery.cpp
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

#include <stdio.h>
#include <syslog.h>

#include <QSqlDatabase>
#include <QSqlError>
#include <QStringList>

#include "config.h"
#include "db.h"

bool OpenDb(QString *err_msg)
{
  QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL3");
  if(!db.isValid()) {
    *err_msg=QObject::tr("Unable to add MySQL database");
    return false;
  }
  db.setHostName(cnf->mysqlHostname());
  db.setDatabaseName(cnf->mysqlDbname());
  db.setUserName(cnf->mysqlUsername());
  db.setPassword(cnf->mysqlPassword());
  if(!db.open()) {
    *err_msg=db.lastError().text();
    return false;
  }
  DbHeartbeat();
  return true;
}


SqlQuery::SqlQuery (const QString &query,bool log):
  QSqlQuery(query)
{
  sql_columns=0;

  if(log) {
    fprintf(stderr,"SQL: %s\n",(const char *)query.toUtf8());
  }

  if(isActive()) {
    QStringList f0=query.split(" ");
    if(f0[0].toLower()=="select") {
      for(int i=1;i<f0.size();i++) {
	if(f0[i].toLower()=="from") {
	  QString fields;
	  for(int j=1;j<i;j++) {
	    fields+=f0[j];
	  }
	  QStringList f1=fields.split(",");
	  sql_columns=f1.size();
	  continue;
	}
      }
    }
  }
  else {
    QString err=QObject::tr("invalid SQL or failed DB connection")+
      +"["+lastError().text()+"]: "+query;

    fprintf(stderr,"%s\n",(const char *)err.toUtf8());
#ifndef WIN32
    syslog(LOG_ERR,(const char *)err.toUtf8());
#endif  // WIN32
  }
}


int SqlQuery::columns() const
{
  return sql_columns;
}


QVariant SqlQuery::run(const QString &sql,bool *ok)
{
  QVariant ret;

  SqlQuery *q=new SqlQuery(sql);
  if(ok!=NULL) {
    *ok=q->isActive();
  }
  ret=q->lastInsertId();
  delete q;

  return ret;
}


int SqlQuery::rows(const QString &sql)
{
  int ret=0;

  QSqlQuery *q=new SqlQuery(sql);
  ret=q->size();
  delete q;

  return ret;
}


QString SqlQuery::escape(const QString &str)
{
  QString res;

  for(int i=0;i<str.length();i++) {
    bool sub=false;
    if(str.at(i)==QChar('"')) {
      res+=QChar('\\');
      res+=QChar('"');
      sub=true;
    }
    if(str.at(i)==QChar('\'')) {
      res+=QChar('\\');
      res+=QChar('\'');
      sub=true;
    }
    if(str.at(i)==QChar('\\')) {
      res+=QChar('\\');
      res+=QChar('\\');
      sub=true;
    }
    if(!sub) {
      res+=str.at(i);
    }
  }
  return res;
}


DbHeartbeat::DbHeartbeat(QObject *parent,int interval)
  : QObject(parent)
{
  heart_timer=new QTimer(this);
  connect(heart_timer,SIGNAL(timeout()),this,SLOT(timeoutData()));
  heart_timer->start(interval);
}


DbHeartbeat::~DbHeartbeat()
{
  delete heart_timer;
}


void DbHeartbeat::timeoutData()
{
  SqlQuery::run("select DB from VERSION");
}
