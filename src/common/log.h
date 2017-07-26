// log.h
//
// Container class for Rivendell logs
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

#ifndef LOG_H
#define LOG_H

#include <QDateTime>
#include <QList>
#include <QString>

#include "logline.h"

class Log : public QList<LogLine>
{
 public:
  Log(const QString &servername,const QString &username,const QString &passwd);
  QString name() const;
  QString serviceName() const;
  void setServiceName(const QString &str);
  QString description() const;
  void setDescription(const QString &str);
  QString originUsername() const;
  QDateTime originDateTime() const;
  QDateTime linkDateTime() const;
  QDateTime modifiedDateTime() const;
  QDate startDate() const;
  void setStartDate(const QDate &date);
  QDate endDate() const;
  void setEndDate(const QDate &date);
  QDate purgeDate() const;
  void setPurgeDate(const QDate &date);
  bool autorefresh() const;
  void setAutorefresh(bool state);
  int scheduledTracks() const;
  int completedTracks() const;
  int musicLinks() const;
  bool musicLinked() const;
  int trafficLinks() const;
  bool trafficLinked() const;
  void insert(int row,unsigned cartnum);
  void insert(int row,LogLine ll);
  bool load(const QString &name,QString *err_msg);
  bool save(const QString &name,QString *err_msg);
  void update();

 private:
  int GetNextId();
  void SetStartTimes();
  QString log_name;
  QString log_service_name;
  QString log_description;
  QString log_origin_username;
  QDateTime log_origin_date_time;
  QDateTime log_link_date_time;
  QDateTime log_modified_date_time;
  DateTime log_start_date;
  DateTime log_end_date;
  DateTime log_purge_date;
  bool log_autorefresh;
  int log_scheduled_tracks;
  int log_completed_tracks;
  int log_music_links;
  bool log_music_linked;
  int log_traffic_links;
  bool log_traffic_linked;
  QString log_servername;
  QString log_username;
  QString log_password;
};


#endif  // LOG_H
