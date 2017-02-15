// loglistmodel.cpp
//
// Remote data model for the Rivendell Library
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

#include <stdio.h>

#include "config.h"
#include "datetime.h"
#include "loglistmodel.h"

#include "../../icons/greencheckmark.xpm"
#include "../../icons/redx.xpm"
#include "../../icons/greenball.xpm"
#include "../../icons/redball.xpm"
#include "../../icons/whiteball.xpm"

LogListModel::LogListModel(QObject *parent)
  : QAbstractTableModel(parent)
{
  //
  // Icons
  //
  model_greencheckmark_map=QVariant(QPixmap(greencheckmark_xpm));
  model_redx_map=QVariant(QPixmap(redx_xpm));
  model_greenball_map=QVariant(QPixmap(greenball_xpm));
  model_redball_map=QVariant(QPixmap(redball_xpm));
  model_whiteball_map=QVariant(QPixmap(whiteball_xpm));

  //
  // Column Header Titles
  //
  model_column_titles.push_back("");
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignCenter);
  model_column_titles.push_back(QObject::tr("Title"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignCenter);
  model_column_titles.push_back(QObject::tr("Description"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignLeft);
  model_column_titles.push_back(QObject::tr("Service"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignCenter);
  model_column_titles.push_back(QObject::tr("Music"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignCenter);
  model_column_titles.push_back(QObject::tr("Traffic"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignCenter);
  model_column_titles.push_back(QObject::tr("Tracks"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignCenter);
  model_column_titles.push_back(QObject::tr("Valid From"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignCenter);
  model_column_titles.push_back(QObject::tr("Valid To"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignCenter);
  model_column_titles.push_back(QObject::tr("Origin"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignLeft);
  model_column_titles.push_back(QObject::tr("Last Linked"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignLeft);
  model_column_titles.push_back(QObject::tr("Last Modified"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignLeft);

  model_service_name=QObject::tr("ALL");
  update();
}


QString LogListModel::serviceName() const
{
  return model_service_name;
}


QString LogListModel::logName(int row) const
{
  return model_log_names.at(row);
}


QModelIndex LogListModel::index(const QString &logname) const
{
  for(int i=0;i<model_log_names.size();i++) {
    if(model_log_names.at(i)==logname) {
      return createIndex(i,0);
    }
  }
  return QModelIndex();
}


void LogListModel::update()
{
  struct rd_log *logs=NULL;
  QString service_name="";
  unsigned numrecs=0;
  int err=0;

  if(model_service_name!=QObject::tr("ALL")) {
    service_name=model_service_name;
  }
  if((err=RD_ListLogs(&logs,cnf->serverHostname().toUtf8(),
		      cnf->serverUsername().toUtf8(),
		      cnf->serverPassword().toUtf8(),
		      service_name.toUtf8(),"",false,&numrecs))==0) {
    if(model_log_names.size()>0) {
      beginRemoveRows(QModelIndex(),0,model_column_fields.size()-1);
      model_log_names.clear();
      model_column_fields.clear();
      endRemoveRows();
    }
    if(numrecs>0) {
      beginInsertRows(QModelIndex(),0,numrecs-1);
      for(unsigned i=0;i<numrecs;i++) {
	model_log_names.push_back(logs[i].log_name);
	model_column_fields.push_back(QStringList());
	model_column_fields.back().
	  push_back(QString().sprintf("%u",LogReady(&(logs[i]))));
	model_column_fields.back().push_back(logs[i].log_name);
	model_column_fields.back().push_back(logs[i].log_description);
	model_column_fields.back().push_back(logs[i].log_service);
	model_column_fields.back().
	  push_back(QString().sprintf("%d",MusicLinked(&(logs[i]))));
	model_column_fields.back().
	  push_back(QString().sprintf("%d",TrafficLinked(&(logs[i]))));
	model_column_fields.back().
	  push_back(QString().sprintf("%d / %d",logs[i].log_completed_tracks,
				      logs[i].log_scheduled_tracks));
	/*
	if(strlen(logs[i].log_startdate)==0) {
	  model_column_fields.back().push_back(tr("Always"));
	}
	else {
	*/
	model_column_fields.back().push_back(DateTime::fromTm(logs[i].log_startdate).toString("MM-dd-yyyy"));
	  //	}
	/*
	if(strlen(logs[i].log_enddate)==0) {
	  model_column_fields.back().push_back(tr("TFN"));
	}
	else {
	*/
	model_column_fields.back().push_back(DateTime::fromTm(logs[i].log_enddate).toString("MM/dd/yyyy"));
	  //	}
	model_column_fields.back().
	  push_back(QString(logs[i].log_origin_username)+" - "+
		    DateTime::fromTm(logs[i].log_origin_datetime).toString("MM/dd/yyyy - hh:mm:ss"));
	/*
	if(strlen(logs[i].log_link_datetime)==0) {
	  model_column_fields.back().push_back(tr("Never"));
	}
	else {
	*/
	model_column_fields.back().push_back(DateTime::fromTm(logs[i].log_link_datetime).toString("MM/dd/yyyy - hh:mm:ss"));
	  //	}
	model_column_fields.back().push_back(DateTime::fromTm(logs[i].log_modified_datetime).toString("MM/dd/yyyy -- hh:mm:ss"));
      }
      endInsertRows();
      free(logs);
    }
  }
  else {
    fprintf(stderr,"LogListModel: RD_ListCarts returned error %d\n",err);
  }
}


int LogListModel::rowCount(const QModelIndex &parent) const
{
  return model_column_fields.size();
}


int LogListModel::columnCount(const QModelIndex &parent) const
{
  return model_column_titles.size();
}


QVariant LogListModel::data(const QModelIndex &index,int role) const
{
  QStringList f0;

  switch((Qt::ItemDataRole)role) {
  case Qt::TextAlignmentRole:
    return model_column_alignments.at(index.column());

  case Qt::DisplayRole:
    if((index.column()!=0)&&(index.column()!=4)&&(index.column()!=5)) {
      return model_column_fields.at(index.row()).at(index.column());
    }
    break;

  case Qt::DecorationRole:
    switch(index.column()) {
    case 0:  // Log Ready
      if(model_column_fields.at(index.row()).at(index.column()).toInt()) {
	return model_greencheckmark_map;
      }
      return model_redx_map;

    case 4:  // Music Linked
    case 5:  // Traffic Linked
      switch(model_column_fields.at(index.row()).at(index.column()).toInt()) {
      case 0:
	return model_redball_map;

      case 1:
	return model_greenball_map;
      }
      return model_whiteball_map;
    }
    break;

    case 6:  // Track Count
      f0=model_column_fields.at(index.row()).at(index.column()).split("/");
      if(f0.size()==2) {
	if(f0.at(1).toInt()>0) {
	  if(f0.at(0).toInt()==f0.at(1).toInt()) {
	    return model_greenball_map;
	  }	
          return model_redball_map;
	}
      }
      return model_whiteball_map;

  default:
    break;
  }
  return QVariant();
}


QVariant LogListModel::headerData(int section,Qt::Orientation orient,
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


void LogListModel::setServiceName(const QString &str)
{
  if(str!=model_service_name) {
    model_service_name=str;
    update();
  }
}


bool LogListModel::LogReady(struct rd_log *log) const
{
  return (log->log_scheduled_tracks==log->log_completed_tracks)&&
    ((log->log_music_links==0)||log->log_music_linked)&&
    ((log->log_traffic_links==0)||log->log_traffic_linked);
}


int LogListModel::MusicLinked(struct rd_log *log) const
{
  if(log->log_music_links==0) {
    return 2;
  }
  return log->log_music_linked;
}


int LogListModel::TrafficLinked(struct rd_log *log) const
{
  if(log->log_traffic_links==0) {
    return 2;
  }
  return log->log_traffic_linked;
}
