// logmodel.h
//
// Remote data model for a Rivendell Log
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

#ifndef LOGMODEL_H
#define LOGMODEL_H

#include <rivendell/rd_listlogs.h>
#include <rivendell/rd_savelog.h>

#include <QAbstractTableModel>
#include <QFont>
#include <QList>
#include <QMap>
#include <QPixmap>
#include <QStringList>

#include "log.h"

class LogModel : public QAbstractTableModel
{
  Q_OBJECT
 public:
  LogModel(QObject *parent=0);
  QString logName() const;
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
  void updateRow(int row);
  bool load(const QString &name,QString *err_msg);
  bool save(const QString &name,QString *err_msg);
  LogLine *logLine(const QModelIndex &index);
  void setBoldFont(const QFont &font);
  int rowCount(const QModelIndex &parent=QModelIndex()) const;
  int columnCount(const QModelIndex &parent=QModelIndex()) const;
  QVariant data(const QModelIndex &index,int role=Qt::DisplayRole) const;
  QVariant headerData(int section,Qt::Orientation orient,
		      int role=Qt::DisplayRole) const;

 signals:
  void error(const QString &err_msg);

 private:
  void LoadColorMap();
  QVariant GetIcon(const LogLine &ll) const;
  QVariant GetStartTime(const LogLine &ll) const;
  QString GetLength(const LogLine &ll) const;
  Log *model_log;
  QVariant model_audiocart_map;
  QVariant model_chain_map;
  QVariant model_macrocart_map;
  QVariant model_marker_map;
  QVariant model_musiclink_map;
  QVariant model_trackcart_map;
  QVariant model_trackmarker_map;
  QVariant model_trafficlink_map;
  QVariant model_bold_font;
  QList<QVariant> model_column_titles;
  QList<QVariant> model_column_alignments;
  QMap<QString,QVariant> model_group_colors;
};


#endif  // LOGMODEL_H
