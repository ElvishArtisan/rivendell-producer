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
  model_column_titles.push_back(QObject::tr("Line ID"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignRight);
  model_column_titles.push_back(QObject::tr("Count"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignRight);
}


QString LogModel::logName() const
{
  return model_log_name;
}


void LogModel::setBoldFont(const QFont &font)
{
  model_bold_font=QVariant(font);
}


int LogModel::rowCount(const QModelIndex &parent) const
{
  return model_column_fields.size();
}


int LogModel::columnCount(const QModelIndex &parent) const
{
  return model_column_titles.size();
}


QVariant LogModel::data(const QModelIndex &index,int role) const
{
  QStringList f0;

  switch((Qt::ItemDataRole)role) {
  case Qt::TextAlignmentRole:
    return model_column_alignments.at(index.column());

  case Qt::TextColorRole:
    if(index.column()==4) {
      return model_group_colors.at(index.row());
    }
    break;

  case Qt::FontRole:
    if(index.column()==4) {
      return model_bold_font;
    }
    break;

  case Qt::DisplayRole:
    if(index.column()!=0) {
      /*
      printf("[%d]: %s\n",index.column(),
	     (const char *)model_column_fields.at(index.row()).at(index.column()).toUtf8());
      */
      return model_column_fields.at(index.row()).at(index.column());
    }
    break;

  case Qt::DecorationRole:
    if(index.column()==0) {
      switch((LogModel::EventType)model_column_fields.at(index.row()).
	     at(index.column()).toInt()) {
      case LogModel::Cart:
	return model_audiocart_map;

      case LogModel::Marker:
	return model_marker_map;

      case LogModel::Macro:
	return model_macrocart_map;

      case LogModel::Chain:
	return model_chain_map;

      case LogModel::Track:
	return model_trackmarker_map;

      case LogModel::MusicLink:
	return model_musiclink_map;

      case LogModel::TrafficLink:
	return model_trafficlink_map;

      case LogModel::OpenBracket:
      case LogModel::CloseBracket:
	break;
      }
      break;
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


QString LogModel::eventSourceText(LogModel::EventSource src)
{
  QString ret=tr("Unknown");
  switch(src) {
  case LogModel::Manual:
    ret=tr("Manual");
  break;

  case LogModel::Traffic:
    ret=tr("Traffic");
    break;

  case LogModel::Music:
    ret=tr("Music");
    break;

  case LogModel::Template:
    ret=tr("RDLogManager");
    break;

 case LogModel::Tracker:
   ret=tr("Voice Tracker");
   break;
  }
  return ret;
}


QString LogModel::eventTransText(TransType type)
{
  QString ret=tr("UNKNOWN");

  switch(type) {
  case LogModel::Play:
    ret=tr("PLAY");
    break;

  case LogModel::Segue:
    ret=tr("SEGUE");
    break;

  case LogModel::Stop:
    ret=tr("STOP");
    break;

  case LogModel::NoTrans:
    break;
  }

  return ret;
}


void LogModel::setLogName(const QString &str)
{
  if(str!=model_log_name) {
    model_log_name=str;
    Update();
  }
}


void LogModel::Update()
{
  struct rd_logline *loglines=NULL;
  unsigned numrecs=0;
  int err=0;
  QTime current_time;

  if((err=RD_ListLog(&loglines,cnf->serverHostname().toUtf8(),
		      cnf->serverUsername().toUtf8(),
		      cnf->serverPassword().toUtf8(),
		     model_log_name.toUtf8(),&numrecs))==0) {
    if(model_log_ids.size()>0) {
      beginRemoveRows(QModelIndex(),0,model_column_fields.size()-1);
      model_log_ids.clear();
      model_column_fields.clear();
      model_group_colors.clear();
      endRemoveRows();
    }
    if(numrecs>0) {
      beginInsertRows(QModelIndex(),0,numrecs-1);
      for(unsigned i=0;i<numrecs;i++) {
	switch((LogModel::EventType)loglines[i].logline_type) {
	case LogModel::Cart:
	case LogModel::Macro:
	  InsertCart(&(loglines[i]),&current_time);
	  break;

	case LogModel::Marker:
	  InsertMarker(&(loglines[i]),&current_time);
	  break;

	case LogModel::OpenBracket:
	  break;

	case LogModel::CloseBracket:
	  break;

	case LogModel::Chain:
	  InsertChain(&(loglines[i]),&current_time);
	  break;

	case LogModel::Track:
	  InsertTrackMarker(&(loglines[i]),&current_time);
	  break;

	case LogModel::MusicLink:
	case LogModel::TrafficLink:
	  InsertLink(&(loglines[i]),&current_time);
	  break;
	}
      }
      endInsertRows();
    }
  }
  else {
    fprintf(stderr,"LogModel: RD_ListLog returned error %d\n",err);
  }
}


void LogModel::InsertCart(struct rd_logline *ll,QTime *current)
{
  model_log_ids.push_back(ll->logline_id);
  model_column_fields.push_back(QStringList());
  model_column_fields.back().
    push_back(QString().sprintf("%d",ll->logline_type));
  model_column_fields.back().
    push_back(GetStartTime(ll,current).toString("hh:mm:ss"));
  model_column_fields.back().
    push_back(LogModel::eventTransText((LogModel::TransType)ll->
				       logline_transition_type));
  model_column_fields.back().
    push_back(QString().sprintf("%06u",ll->logline_cart_number));
  model_column_fields.back().push_back(ll->logline_group_name);
  model_group_colors.push_back(QVariant(QColor(ll->logline_group_color)));
  model_column_fields.back().
    push_back(QTime().addMSecs(GetLength(ll)).toString("mm:ss"));
  model_column_fields.back().push_back(ll->logline_title);
  model_column_fields.back().push_back(ll->logline_artist);
  model_column_fields.back().push_back(ll->logline_client);
  model_column_fields.back().push_back(ll->logline_agency);
  model_column_fields.back().push_back(ll->logline_label);
  model_column_fields.back().
    push_back(LogModel::eventSourceText((LogModel::EventSource)ll->logline_source));
  model_column_fields.back().
    push_back(QString().sprintf("%d",ll->logline_id));
  model_column_fields.back().
    push_back(QString().sprintf("%d",ll->logline_line));
}


void LogModel::InsertMarker(struct rd_logline *ll,QTime *current)
{
  model_log_ids.push_back(ll->logline_id);
  model_column_fields.push_back(QStringList());
  model_column_fields.back().
    push_back(QString().sprintf("%d",ll->logline_type));
  model_column_fields.back().
    push_back(GetStartTime(ll,current).toString("hh:mm:ss"));
  model_column_fields.back().
    push_back(LogModel::eventTransText((LogModel::TransType)ll->
				       logline_transition_type));
  model_column_fields.back().push_back(tr("MARKER"));
  model_column_fields.back().push_back(QString());
  model_group_colors.push_back(QVariant(QColor(ll->logline_group_color)));
  model_column_fields.back().push_back(QString());
  model_column_fields.back().push_back(ll->logline_marker_comment);
  model_column_fields.back().push_back(QString());
  model_column_fields.back().push_back(QString());
  model_column_fields.back().push_back(QString());
  model_column_fields.back().push_back(QString());
  model_column_fields.back().
    push_back(LogModel::eventSourceText((LogModel::EventSource)ll->logline_source));
  model_column_fields.back().
    push_back(QString().sprintf("%d",ll->logline_id));
  model_column_fields.back().
    push_back(QString().sprintf("%d",ll->logline_line));
}


void LogModel::InsertTrackMarker(struct rd_logline *ll,QTime *current)
{
  model_log_ids.push_back(ll->logline_id);
  model_column_fields.push_back(QStringList());
  model_column_fields.back().
    push_back(QString().sprintf("%d",ll->logline_type));
  model_column_fields.back().
    push_back(GetStartTime(ll,current).toString("hh:mm:ss"));
  model_column_fields.back().
    push_back(LogModel::eventTransText((LogModel::TransType)ll->
				       logline_transition_type));
  model_column_fields.back().push_back(tr("TRACK"));
  model_column_fields.back().push_back(QString());
  model_group_colors.push_back(QVariant(QColor(ll->logline_group_color)));
  model_column_fields.back().push_back(QString());
  model_column_fields.back().push_back(ll->logline_marker_comment);
  model_column_fields.back().push_back(QString());
  model_column_fields.back().push_back(QString());
  model_column_fields.back().push_back(QString());
  model_column_fields.back().push_back(QString());
  model_column_fields.back().
    push_back(LogModel::eventSourceText((LogModel::EventSource)ll->logline_source));
  model_column_fields.back().
    push_back(QString().sprintf("%d",ll->logline_id));
  model_column_fields.back().
    push_back(QString().sprintf("%d",ll->logline_line));
}


void LogModel::InsertLink(struct rd_logline *ll,QTime *current)
{
  model_log_ids.push_back(ll->logline_id);
  model_column_fields.push_back(QStringList());
  model_column_fields.back().
    push_back(QString().sprintf("%d",ll->logline_type));
  model_column_fields.back().
    push_back(GetStartTime(ll,current).toString("hh:mm:ss"));
  model_column_fields.back().
    push_back(LogModel::eventTransText((LogModel::TransType)ll->
				       logline_transition_type));
  model_column_fields.back().push_back(tr("LINK"));
  model_column_fields.back().push_back(QString());
  model_group_colors.push_back(QVariant(QColor(ll->logline_group_color)));
  model_column_fields.back().push_back(QString());
  if(ll->logline_type==LogModel::TrafficLink) {
    model_column_fields.back().push_back("["+tr("traffic import")+"]");
  }
  else {
    model_column_fields.back().push_back("["+tr("music import")+"]");
  }
  model_column_fields.back().push_back(QString());
  model_column_fields.back().push_back(QString());
  model_column_fields.back().push_back(QString());
  model_column_fields.back().push_back(QString());
  model_column_fields.back().
    push_back(LogModel::eventSourceText((LogModel::EventSource)ll->logline_source));
  model_column_fields.back().
    push_back(QString().sprintf("%d",ll->logline_id));
  model_column_fields.back().
    push_back(QString().sprintf("%d",ll->logline_line));
}


void LogModel::InsertChain(struct rd_logline *ll,QTime *current)
{
  model_log_ids.push_back(ll->logline_id);
  model_column_fields.push_back(QStringList());
  model_column_fields.back().
    push_back(QString().sprintf("%d",ll->logline_type));
  model_column_fields.back().
    push_back(GetStartTime(ll,current).toString("hh:mm:ss"));
  model_column_fields.back().
    push_back(LogModel::eventTransText((LogModel::TransType)ll->
				       logline_transition_type));
  model_column_fields.back().push_back(tr("LOG CHAIN"));
  model_column_fields.back().push_back(QString());
  model_group_colors.push_back(QVariant(QColor(ll->logline_group_color)));
  model_column_fields.back().push_back(QString());
  model_column_fields.back().push_back(ll->logline_marker_label);
  model_column_fields.back().push_back(QString());
  model_column_fields.back().push_back(QString());
  model_column_fields.back().push_back(QString());
  model_column_fields.back().push_back(QString());
  model_column_fields.back().
    push_back(LogModel::eventSourceText((LogModel::EventSource)ll->logline_source));
  model_column_fields.back().
    push_back(QString().sprintf("%d",ll->logline_id));
  model_column_fields.back().
    push_back(QString().sprintf("%d",ll->logline_line));
}


QTime LogModel::GetStartTime(struct rd_logline *ll,QTime *current) const
{
  QTime ret=*current;

  ll->logline_starttime[12]=0; // FIXME: Needed to work around a bug in rivcapi
  if(ll->logline_time_type==LogModel::Hard) {
    ret=QTime::fromString(ll->logline_starttime,"hh:mm:ss.zzz");
  }
  *current=current->addMSecs(GetLength(ll));

  return ret;
}


int LogModel::GetLength(struct rd_logline *logline) const
{
  if((logline->logline_start_point_cart<0)||
     (logline->logline_end_point_cart<0)) {
    return 0;
  }
  return logline->logline_end_point_cart-logline->logline_start_point_cart;
}
