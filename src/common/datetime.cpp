// datetime.cpp
//
// DateTime values for Rivendell Producer
//
//   (C) Copyright 2017 Fred Gleason <fredg@paravelsystems.com>
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

#include <string.h>

#include "datetime.h"

DateTime::DateTime()
  : QDateTime()
{
}


DateTime::DateTime(const QDate &date,const QTime &time,Qt::TimeSpec spec)
  : QDateTime(date,time,spec)
{
}


DateTime::DateTime(const QDateTime &other)
  : QDateTime(other)
{
}


struct tm DateTime::toTm() const
{
  struct tm ret;

  memset(&ret,0,sizeof(tm));
  ret.tm_sec=time().second();
  ret.tm_min=time().minute();
  ret.tm_hour=time().hour();
  ret.tm_mday=date().day();
  ret.tm_mon=date().month()-1;
  ret.tm_year=date().year()-1900;
  ret.tm_wday=date().dayOfWeek();
  if(ret.tm_wday==7) {
    ret.tm_wday=0;
  }
  ret.tm_yday=date().dayOfYear()-1;
  ret.tm_isdst=-1;

  return ret;
}


QDateTime DateTime::fromTm(struct tm dt)
{
  return QDateTime(QDate(dt.tm_year+1900,dt.tm_mon+1,dt.tm_mday),
		   QTime(dt.tm_hour,dt.tm_min,dt.tm_sec));
}
