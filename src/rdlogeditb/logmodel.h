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

#include <QAbstractTableModel>
#include <QFont>
#include <QList>
#include <QPixmap>
#include <QStringList>

class LogModel : public QAbstractTableModel
{
  Q_OBJECT
 public:
  enum EventSource {Manual=0,Traffic=1,Music=2,Template=3,Tracker=4};
  enum EventType {Cart=0,Marker=1,Macro=2,OpenBracket=3,CloseBracket=4,Chain=5,
		  Track=6,MusicLink=7,TrafficLink=8};
  enum TimeType {Relative=0,Hard=1,NoTime=255};
  enum TransType {Play=0,Segue=1,Stop=2,NoTrans=255};
  LogModel(QObject *parent=0);
  QString logName() const;
  void setBoldFont(const QFont &font);
  int rowCount(const QModelIndex &parent=QModelIndex()) const;
  int columnCount(const QModelIndex &parent=QModelIndex()) const;
  QVariant data(const QModelIndex &index,int role=Qt::DisplayRole) const;
  QVariant headerData(int section,Qt::Orientation orient,
		      int role=Qt::DisplayRole) const;
  static QString eventSourceText(EventSource src);
  static QString eventTransText(TransType type);

 public slots:
  void setLogName(const QString &str);

 private:
  void Update();
  void InsertCart(struct rd_logline *ll,QTime *current);
  void InsertMarker(struct rd_logline *ll,QTime *current);
  void InsertTrackMarker(struct rd_logline *ll,QTime *current);
  void InsertLink(struct rd_logline *ll,QTime *current);
  void InsertChain(struct rd_logline *ll,QTime *current);
  QTime GetStartTime(struct rd_logline *ll,QTime *current) const;
  int GetLength(struct rd_logline *logline) const;
  QString model_log_name;
  QVariant model_audiocart_map;
  QVariant model_chain_map;
  QVariant model_macrocart_map;
  QVariant model_marker_map;
  QVariant model_musiclink_map;
  QVariant model_trackcart_map;
  QVariant model_trackmarker_map;
  QVariant model_trafficlink_map;
  QList<QVariant> model_column_titles;
  QList<QVariant> model_column_alignments;
  QList<QStringList> model_column_fields;
  QList<QVariant> model_group_colors;
  QList<int> model_log_ids;
  QVariant model_bold_font;
};


#endif  // LOGMODEL_H
