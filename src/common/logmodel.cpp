// logmodel.cpp
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

#include <stdio.h>

#include <rivendell/rd_listgroups.h>
#include <rivendell/rd_listlog.h>

#include <QDateTime>

#include "config.h"
#include "logmodel.h"

#include "../../icons/audiocart.xpm"
#include "../../icons/chain.xpm"
#include "../../icons/macrocart.xpm"
#include "../../icons/marker.xpm"
#include "../../icons/musiclink.xpm"
#include "../../icons/trackcart.xpm"
#include "../../icons/trackmarker.xpm"
#include "../../icons/trafficlink.xpm"

LogModel::LogModel(QObject *parent)
  : QAbstractTableModel(parent)
{
  //
  // Log
  //
  model_log=
    new Log(cnf->serverHostname(),cnf->serverUsername(),cnf->serverPassword());

  //
  // Icons
  //
  model_audiocart_map=QVariant(QPixmap(audiocart_xpm));
  model_chain_map=QVariant(QPixmap(chain_xpm));
  model_macrocart_map=QVariant(QPixmap(macrocart_xpm));
  model_marker_map=QVariant(QPixmap(marker_xpm));
  model_musiclink_map=QVariant(QPixmap(musiclink_xpm));
  model_trackcart_map=QVariant(QPixmap(trackcart_xpm));
  model_trackmarker_map=QVariant(QPixmap(trackmarker_xpm));
  model_trafficlink_map=QVariant(QPixmap(trafficlink_xpm));

  //
  // Column Header Titles
  //
  model_column_titles.push_back("");
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignCenter);
  model_column_titles.push_back(QObject::tr("Time"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignRight);
  model_column_titles.push_back(QObject::tr("Trans"));
  model_column_alignments.push_back((int)Qt::AlignCenter);
  model_column_titles.push_back(QObject::tr("Cart"));
  model_column_alignments.push_back((int)Qt::AlignCenter);
  model_column_titles.push_back(QObject::tr("Group"));
  model_column_alignments.push_back((int)Qt::AlignCenter);
  model_column_titles.push_back(QObject::tr("Length"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignRight);
  model_column_titles.push_back(QObject::tr("Title"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignLeft);
  model_column_titles.push_back(QObject::tr("Artist"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignLeft);
  model_column_titles.push_back(QObject::tr("Client"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignLeft);
  model_column_titles.push_back(QObject::tr("Agency"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignLeft);
  model_column_titles.push_back(QObject::tr("Label"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignLeft);
  model_column_titles.push_back(QObject::tr("Source"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignLeft);
  model_column_titles.push_back(QObject::tr("Ext Data"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignLeft);
  model_column_titles.push_back(QObject::tr("Line ID"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignRight);
  model_column_titles.push_back(QObject::tr("Count"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignRight);
}


QString LogModel::logName() const
{
  return model_log->name();
}


QString LogModel::serviceName() const
{
  return model_log->serviceName();
}


void LogModel::setServiceName(const QString &str)
{
  model_log->setServiceName(str);
}


QString LogModel::description() const
{
  return model_log->description();
}


void LogModel::setDescription(const QString &str)
{
  model_log->setDescription(str);
}


QString LogModel::originUsername() const
{
  return model_log->originUsername();
}


QDateTime LogModel::originDateTime() const
{
  return model_log->originDateTime();
}


QDateTime LogModel::linkDateTime() const
{
  return model_log->linkDateTime();
}


QDateTime LogModel::modifiedDateTime() const
{
  return model_log->modifiedDateTime();
}


QDate LogModel::startDate() const
{
  return model_log->startDate();
}


void LogModel::setStartDate(const QDate &date)
{
  model_log->setStartDate(date);
}


QDate LogModel::endDate() const
{
  return model_log->endDate();
}


void LogModel::setEndDate(const QDate &date)
{
  model_log->setEndDate(date);
}


QDate LogModel::purgeDate() const
{
  return model_log->purgeDate();
}


void LogModel::setPurgeDate(const QDate &date)
{
  model_log->setPurgeDate(date);
}


bool LogModel::autorefresh() const
{
  return model_log->autorefresh();
}


void LogModel::setAutorefresh(bool state)
{
  model_log->setAutorefresh(state);
}


int LogModel::scheduledTracks() const
{
  return model_log->scheduledTracks();
}


int LogModel::completedTracks() const
{
  return model_log->completedTracks();
}


int LogModel::musicLinks() const
{
  return model_log->musicLinks();
}


bool LogModel::musicLinked() const
{
  return model_log->musicLinked();
}


int LogModel::trafficLinks() const
{
  return model_log->trafficLinks();
}


bool LogModel::trafficLinked() const
{
  return model_log->trafficLinked();
}


void LogModel::updateRow(int row)
{
  model_log->update();
  emit dataChanged(createIndex(row,0),createIndex(row,14));
}


void LogModel::move(int from_row,int to_row)
{
  model_log->move(from_row,to_row);
  emit dataChanged(createIndex(from_row,0),
		   createIndex(from_row,columnCount()-1));
  emit dataChanged(createIndex(to_row,0),
		   createIndex(to_row,columnCount()-1));
}


void LogModel::insert(int row,LogLine *ll)
{
  beginInsertRows(QModelIndex(),row,row);
  model_log->insert(row,*ll);
  endInsertRows();
}


void LogModel::removeAt(int row,int num_rows)
{
  beginRemoveRows(QModelIndex(),row,row+num_rows-1);
  for(int i=0;i<num_rows;i++) {
    model_log->removeAt(row);
  }
  endRemoveRows();
}


bool LogModel::load(const QString &name,QString *err_msg)
{
  LoadColorMap();
  if(model_log->size()>0) {
    beginRemoveRows(QModelIndex(),0,model_log->size()-1);
    model_log->clear();
    endRemoveRows();
  }
  if(model_log->load(name,err_msg)) {
    beginInsertRows(QModelIndex(),0,model_log->size()-1);
    endInsertRows();
    return true;
  }
  return false;
}


bool LogModel::save(const QString &name,QString *err_msg)
{
  return model_log->save(name,err_msg);
}


LogLine *LogModel::logLine(const QModelIndex &index)
{
  return logLine(index.row());
}


LogLine *LogModel::logLine(int row)
{
  if(row>=model_log->size()) {
    return NULL;
  }
  return &(*model_log)[row];
}


void LogModel::setBoldFont(const QFont &font)
{
  model_bold_font=QVariant(font);
}


int LogModel::rowCount(const QModelIndex &parent) const
{
  return model_log->size()+1;
}


int LogModel::columnCount(const QModelIndex &parent) const
{
  return model_column_titles.size();
}


QVariant LogModel::data(const QModelIndex &index,int role) const
{
  QStringList f0;
  LogLine ll;

  //
  // End of log marker
  //
  if(index.row()>=model_log->size()) {
    if((role==Qt::DisplayRole)&&(index.column()==6)) {
      return QVariant(tr("--- end of log ---"));
    }
    return QVariant();
  }

  switch((Qt::ItemDataRole)role) {
  case Qt::TextAlignmentRole:
    return model_column_alignments.at(index.column());

  case Qt::TextColorRole:
    if(index.column()==4) {
      return model_group_colors[model_log->at(index.row()).groupName()];
    }
    break;

  case Qt::FontRole:
    if(index.column()==4) {
      return model_bold_font;
    }
    break;

  case Qt::DisplayRole:
    ll=model_log->at(index.row());
    switch(index.column()) {
    case 0:  // Icon
      break;

    case 1:  // Start Time
      return GetStartTime(ll);

    case 2:  // Transition Type
      return QVariant(LogLine::transText(ll.transType()));

    case 3:  // Cart Number
      return QVariant(QString().sprintf("%06u",ll.cartNumber()));

    case 4:  // Group Name
      return QVariant(ll.groupName());

    case 5:  // Length
      return QVariant(GetLength(ll));

    case 6:  // Title
      return QVariant(ll.title());

    case 7:  // Artist
      return QVariant(ll.artist());

    case 8:  // Client
      return QVariant(ll.client());

    case 9:  // Agency
      return QVariant(ll.agency());

    case 10: // Label
      return QVariant(ll.label());

    case 11: // Source
      return QVariant(LogLine::sourceText(ll.source()));

    case 12: // Ext Data
      return QVariant(ll.extData());

    case 13: // Line Id
      return QVariant(QString().sprintf("%d",ll.id()));

    case 14: // Count
      return QVariant(index.row());
    }
    break;

  case Qt::DecorationRole:
    if(index.column()==0) {
      return GetIcon(model_log->at(index.row()));
    }
    break;

  default:
    break;
  }
  return QVariant();
}


QVariant LogModel::headerData(int section,Qt::Orientation orient,
				  int role) const
{
  if(orient==Qt::Vertical) {
    return QVariant();
  }

  switch((Qt::ItemDataRole)role) {
  case Qt::DisplayRole:
    return model_column_titles.at(section);

  default:
    break;
  }
  return QVariant();
}


void LogModel::LoadColorMap()
{
  struct rd_group *grps=NULL;
  unsigned grp_quan=0;
  int err=0;

  if((err=RD_ListGroups(&grps,cnf->serverHostname(),cnf->serverUsername(),
			cnf->serverPassword(),&grp_quan))!=0) {
    emit error(QString().sprintf("RD_ListGroups() failed [error: %d]",err));
    return;
  }
  model_group_colors.clear();
  for(unsigned i=0;i<grp_quan;i++) {
    model_group_colors[grps[i].grp_name]=QVariant(QColor(grps[i].grp_color));
  }
}


QVariant LogModel::GetIcon(const LogLine &ll) const
{
  switch(ll.type()) {
  case LogLine::Cart:
    return model_audiocart_map;

  case LogLine::Marker:
    return model_marker_map;

  case LogLine::Macro:
    return model_macrocart_map;

  case LogLine::Chain:
    return model_chain_map;

  case LogLine::Track:
    return model_trackmarker_map;

  case LogLine::MusicLink:
    return model_musiclink_map;

  case LogLine::TrafficLink:
    return model_trafficlink_map;

  case LogLine::OpenBracket:
  case LogLine::CloseBracket:
  case LogLine::UnknownType:
    break;
  }
  return QVariant();
}


QVariant LogModel::GetStartTime(const LogLine &ll) const
{
  if(ll.timeType()==LogLine::Hard) {
    return QVariant("T"+ll.startTime().toString("hh:mm:ss.zzz").left(10));
  }
  return QVariant(ll.startTime().toString("hh:mm:ss.zzz").left(10));
}


QString LogModel::GetLength(const LogLine &ll) const
{
  int len=ll.length(LogLine::CartPointer);
  if(len>=3600000) {
    return QTime().addMSecs(len).toString("hh:mm:ss");
  }
  return QTime().addMSecs(len).toString("mm:ss");
}

