// logline.cpp
//
// Container class for log events
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

#include "logline.h"

#include <QObject>

LogLine::LogLine()
{
  logline_id=0;
  logline_type=LogLine::UnknownType;
  logline_cart_type=LogLine::AudioCart;
  logline_cart_number=0;
  logline_cut_number=0;
  logline_year=0;
  logline_usage_code=LogLine::UsageFeature;
  logline_enforce_length=false;
  logline_forced_length=0;
  logline_evergreen=false;
  logline_source=LogLine::Manual;
  logline_time_type=LogLine::Relative;
  logline_grace_time=0;
  logline_trans_type=LogLine::Play;
  logline_cut_quantity=0;
  logline_last_cut_played=0;
  for(int i=0;i<2;i++) {
    logline_start_point[i]=-1;
    logline_end_point[i]=-1;
    logline_segue_start_point[i]=-1;
    logline_segue_end_point[i]=-1;
    logline_fadeup_point[i]=-1;
    logline_fadedown_point[i]=-1;
  }
  logline_segue_gain=-3000;
  logline_duckup_gain=0;
  logline_duckdown_gain=0;
  logline_talk_start_point=-1;
  logline_talk_end_point=-1;
  logline_hook_start_point=-1;
  logline_hook_end_point=-1;
  logline_hook_mode=false;
  logline_event_length=0;
  logline_link_length=0;
  logline_link_start_slop=0;
  logline_link_end_slop=0;
  logline_link_id=-1;
  logline_link_embedded=false;
  logline_ext_length=0;
}


int LogLine::id() const
{
  return logline_id;
}


void LogLine::setId(int id)
{
  logline_id=id;
}


LogLine::Type LogLine::type() const
{
  return logline_type;
}


void LogLine::setType(LogLine::Type type)
{
  logline_type=type;
}


LogLine::CartType LogLine::cartType() const
{
  return logline_cart_type;
}


void LogLine::setCartType(LogLine::CartType type)
{
  logline_cart_type=type;
}


unsigned LogLine::cartNumber() const
{
  return logline_cart_number;
}


void LogLine::setCartNumber(unsigned cartnum)
{
  logline_cart_number=cartnum;
}


int LogLine::cutNumber() const
{
  return logline_cut_number;
}


void LogLine::setCutNumber(int cutnum)
{
  logline_cut_number=cutnum;
}


QString LogLine::groupName() const
{
  return logline_group_name;
}


void LogLine::setGroupName(const QString &str)
{
  logline_group_name=str;
}


QColor LogLine::groupColor() const
{
  return logline_group_color;
}


void LogLine::setGroupColor(const QColor &color)
{
  logline_group_color=color;
}


QString LogLine::title() const
{
  return logline_title;
}


void LogLine::setTitle(const QString &str)
{
  logline_title=str;
}


QString LogLine::artist() const
{
  return logline_artist;
}


void LogLine::setArtist(const QString &str)
{
  logline_artist=str;
}


QString LogLine::album() const
{
  return logline_album;
}


void LogLine::setAlbum(const QString &str)
{
  logline_album=str;
}


int LogLine::year() const
{
  return logline_year;
}


void LogLine::setYear(int year)
{
  logline_year=year;
}


QString LogLine::label() const
{
  return logline_label;
}


void LogLine::setLabel(const QString &str)
{
  logline_label=str;
}


QString LogLine::client() const
{
  return logline_client;
}


void LogLine::setClient(const QString &str)
{
  logline_client=str;
}


QString LogLine::agency() const
{
  return logline_agency;
}


void LogLine::setAgency(const QString &str)
{
  logline_agency=str;
}


QString LogLine::publisher() const
{
  return logline_publisher;
}


void LogLine::setPublisher(const QString &str)
{
  logline_publisher=str;
}


QString LogLine::composer() const
{
  return logline_composer;
}


void LogLine::setComposer(const QString &str)
{
  logline_composer=str;
}


QString LogLine::conductor() const
{
  return logline_conductor;
}


void LogLine::setConductor(const QString &str)
{
  logline_conductor=str;
}


QString LogLine::userDefined() const
{
  return logline_user_defined;
}


void LogLine::setUserDefined(const QString &str)
{
  logline_user_defined=str;
}


LogLine::UsageCode LogLine::usageCode() const
{
  return logline_usage_code;
}


void LogLine::setUsageCode(LogLine::UsageCode code)
{
  logline_usage_code=code;
}


bool LogLine::enforceLength() const
{
  return logline_enforce_length;
}


void LogLine::setEnforceLength(bool state)
{
  logline_enforce_length=state;
}


int LogLine::forcedLength() const
{
  return logline_forced_length;
}


void LogLine::setForcedLength(int msec)
{
  logline_forced_length=msec;
}


bool LogLine::evergreen() const
{
  return logline_evergreen;
}


void LogLine::setEvergreen(bool state)
{
  logline_evergreen=state;
}


LogLine::Source LogLine::source() const
{
  return logline_source;
}


void LogLine::setSource(LogLine::Source src)
{
  logline_source=src;
}


LogLine::TimeType LogLine::timeType() const
{
  return logline_time_type;
}


void LogLine::setTimeType(LogLine::TimeType type)
{
  logline_time_type=type;
}


QTime LogLine::startTime() const
{
  return logline_start_time;
}


void LogLine::setStartTime(const QTime &time)
{
  logline_start_time=time;
}


int LogLine::graceTime() const
{
  return logline_grace_time;
}


void LogLine::setGraceTime(int msec)
{
  logline_grace_time=msec;
}


LogLine::TransType LogLine::transType() const
{
  return logline_trans_type;
}


void LogLine::setTransType(LogLine::TransType type)
{
  logline_trans_type=type;
}


int LogLine::cutQuantity() const
{
  return logline_cut_quantity;
}


void LogLine::setCutQuantity(int quan)
{
  logline_cut_quantity=quan;
}


int LogLine::lastCutPlayed() const
{
  return logline_last_cut_played;
}


void LogLine::setLastCutPlayed(int cutnum)
{
  logline_last_cut_played=cutnum;
}


QString LogLine::markerComment() const
{
  return logline_marker_comment;
}


void LogLine::setMarkerComment(const QString &str)
{
  logline_marker_comment=str;
}


QString LogLine::markerLabel() const
{
  return logline_marker_label;
}


void LogLine::setMarkerLabel(const QString &str)
{
  logline_marker_label=str;
}


QString LogLine::originUser() const
{
  return logline_marker_label;
}


void LogLine::setOriginUser(const QString &str)
{
  logline_marker_label=str;
}


QDateTime LogLine::originDateTime() const
{
  return logline_origin_date_time;
}


void LogLine::setOriginDateTime(const QDateTime &datetime)
{
  logline_origin_date_time=datetime;
}


int LogLine::startPoint(LogLine::PointerSource src) const
{
  if(src==LogLine::AutoPointer) {
    if(logline_start_point[LogLine::LogPointer]>=0) {
      src=LogLine::LogPointer;
    }
    else {
      src=LogLine::CartPointer;
    }
  }
  return logline_start_point[src];
}


void LogLine::setStartPoint(LogLine::PointerSource src,int msec)
{
  logline_start_point[src]=msec;
}


int LogLine::endPoint(LogLine::PointerSource src) const
{
  if(src==LogLine::AutoPointer) {
    if(logline_end_point[LogLine::LogPointer]>=0) {
      src=LogLine::LogPointer;
    }
    else {
      src=LogLine::CartPointer;
    }
  }
  return logline_end_point[src];
}


void LogLine::setEndPoint(LogLine::PointerSource src,int msec)
{
  logline_end_point[src]=msec;
}


int LogLine::segueStartPoint(LogLine::PointerSource src) const
{
  if(src==LogLine::AutoPointer) {
    if(logline_segue_start_point[LogLine::LogPointer]>=0) {
      src=LogLine::LogPointer;
    }
    else {
      src=LogLine::CartPointer;
    }
  }
  return logline_segue_start_point[src];
}


void LogLine::setSegueStartPoint(LogLine::PointerSource src,int msec)
{
  logline_segue_start_point[src]=msec;
}


int LogLine::segueEndPoint(LogLine::PointerSource src) const
{
  if(src==LogLine::AutoPointer) {
    if(logline_segue_end_point[LogLine::LogPointer]>=0) {
      src=LogLine::LogPointer;
    }
    else {
      src=LogLine::CartPointer;
    }
  }
  return logline_segue_end_point[src];
}


void LogLine::setSegueEndPoint(LogLine::PointerSource src,int msec)
{
  logline_segue_end_point[src]=msec;
}


int LogLine::segueGain() const
{
  return logline_segue_gain;
}


void LogLine::setSegueGain(int gain)
{
  logline_segue_gain=gain;
}


int LogLine::fadeupPoint(LogLine::PointerSource src) const
{
  if(src==LogLine::AutoPointer) {
    if(logline_fadeup_point[LogLine::LogPointer]>=0) {
      src=LogLine::LogPointer;
    }
    else {
      src=LogLine::CartPointer;
    }
  }
  return logline_fadeup_point[src];
}


void LogLine::setFadeupPoint(LogLine::PointerSource src,int msec)
{
  logline_fadeup_point[src]=msec;
}


int LogLine::fadedownPoint(LogLine::PointerSource src) const
{
  if(src==LogLine::AutoPointer) {
    if(logline_fadedown_point[LogLine::LogPointer]>=0) {
      src=LogLine::LogPointer;
    }
    else {
      src=LogLine::CartPointer;
    }
  }
  return logline_fadedown_point[src];
}


void LogLine::setFadedownPoint(LogLine::PointerSource src,int msec)
{
  logline_fadedown_point[src]=msec;
}


int LogLine::duckupGain() const
{
  return logline_duckup_gain;
}


void LogLine::setDuckupGain(int gain)
{
  logline_duckup_gain=gain;
}


int LogLine::duckdownGain() const
{
  return logline_duckdown_gain;
}


void LogLine::setDuckdownGain(int gain)
{
  logline_duckdown_gain=gain;
}


int LogLine::talkStartPoint() const
{
  return logline_talk_start_point;
}


void LogLine::setTalkStartPoint(int msec)
{
  logline_talk_start_point=msec;
}


int LogLine::talkEndPoint() const
{
  return logline_talk_end_point;
}


void LogLine::setTalkEndPoint(int msec)
{
  logline_talk_end_point=msec;
}


int LogLine::hookStartPoint() const
{
  return logline_hook_start_point;
}


void LogLine::setHookStartPoint(int msec)
{
  logline_hook_start_point=msec;
}


bool LogLine::hookMode() const
{
  return logline_hook_mode;
}


void LogLine::setHookMode(bool state)
{
  logline_hook_mode=state;
}


int LogLine::hookEndPoint() const
{
  return logline_hook_end_point;
}


void LogLine::setHookEndPoint(int msec)
{
  logline_hook_end_point=msec;
}


int LogLine::eventLength() const
{
  return logline_event_length;
}


void LogLine::setEventLength(int msec)
{
  logline_event_length=msec;
}


QString LogLine::linkEventName() const
{
  return logline_link_event_name;
}


void LogLine::setLinkEventName(const QString &str)
{
  logline_link_event_name=str;
}


QTime LogLine::linkStartTime() const
{
  return logline_link_start_time;
}


void LogLine::setLinkStartTime(const QTime &time)
{
  logline_link_start_time=time;
}


int LogLine::linkLength() const
{
  return logline_link_length;
}


void LogLine::setLinkLength(int msec)
{
  logline_link_length=msec;
}


int LogLine::linkStartSlop() const
{
  return logline_link_start_slop;
}


void LogLine::setLinkStartSlop(int msec)
{
  logline_link_start_slop=msec;
}


int LogLine::linkEndSlop() const
{
  return logline_link_end_slop;
}


void LogLine::setLinkEndSlop(int msec)
{
  logline_link_end_slop=msec;
}


int LogLine::linkId() const
{
  return logline_link_id;
}


void LogLine::setLinkId(int id)
{
  logline_link_id=id;
}


bool LogLine::linkEmbedded() const
{
  return logline_link_embedded;
}


void LogLine::setLinkEmbedded(bool state)
{
  logline_link_embedded=state;
}


QTime LogLine::extStartTime() const
{
  return logline_ext_start_time;
}


void LogLine::setExtStartTime(const QTime &time)
{
  logline_ext_start_time=time;
}


int LogLine::extLength() const
{
  return logline_ext_length;
}


void LogLine::setExtLength(int msec)
{
  logline_ext_length=msec;
}


QString LogLine::extCartName() const
{
  return logline_ext_cart_name;
}


void LogLine::setExtCartName(const QString &str)
{
  logline_ext_cart_name=str;
}


QString LogLine::extData() const
{
  return logline_ext_data;
}


void LogLine::setExtData(const QString &str)
{
  logline_ext_data=str;
}


QString LogLine::extEventId() const
{
  return logline_ext_event_id;
}


void LogLine::setExtEventId(const QString &str)
{
  logline_ext_event_id=str;
}


QString LogLine::extAnncType() const
{
  return logline_ext_annc_type;
}


void LogLine::setExtAnncType(const QString &str)
{
  logline_ext_annc_type=str;
}


int LogLine::length(LogLine::PointerSource src) const
{
  if((startPoint(src)>=0)&&(endPoint(src)>=0)) {
    return endPoint(src)-startPoint(src);
  }
  return 0;
}


QString LogLine::sourceText(LogLine::Source src)
{
  QString ret=QObject::tr("Unknown");
  switch(src) {
  case LogLine::Manual:
    ret=QObject::tr("Manual");
  break;

  case LogLine::Traffic:
    ret=QObject::tr("Traffic");
    break;

  case LogLine::Music:
    ret=QObject::tr("Music");
    break;

  case LogLine::Template:
    ret=QObject::tr("RDLogManager");
    break;

 case LogLine::Tracker:
   ret=QObject::tr("Voice Tracker");
   break;
  }
  return ret;
}


QString LogLine::transText(LogLine::TransType type)
{
  QString ret=QObject::tr("UNKNOWN");

  switch(type) {
  case LogLine::Play:
    ret=QObject::tr("PLAY");
    break;

  case LogLine::Segue:
    ret=QObject::tr("SEGUE");
    break;

  case LogLine::Stop:
    ret=QObject::tr("STOP");
    break;

  case LogLine::NoTrans:
    break;
  }

  return ret;
}
