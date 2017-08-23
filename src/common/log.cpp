// log.cpp
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

#include <string.h>

#include "config.h"
#include "datetime.h"
#include "log.h"

Log::Log(const QString &servername,const QString &username,
	 const QString &passwd)
  : QList<LogLine>()
{
  log_servername=servername;
  log_username=username;
  log_password=passwd;
}


QString Log::name() const
{
  return log_name;
}


QString Log::serviceName() const
{
  return log_service_name;
}


void Log::setServiceName(const QString &str)
{
  log_service_name=str;
}


QString Log::description() const
{
  return log_description;
}


void Log::setDescription(const QString &str)
{
  log_description=str;
}


QString Log::originUsername() const
{
  return log_origin_username;
}


QDateTime Log::originDateTime() const
{
  return log_origin_date_time;
}


QDateTime Log::linkDateTime() const
{
  return log_origin_date_time;
}


QDateTime Log::modifiedDateTime() const
{
  return log_modified_date_time;
}


QDate Log::startDate() const
{
  return log_start_date.date();
}


void Log::setStartDate(const QDate &date)
{
  log_start_date.setDate(date);
}


QDate Log::endDate() const
{
  return log_end_date.date();
}


void Log::setEndDate(const QDate &date)
{
  log_end_date.setDate(date);
}


QDate Log::purgeDate() const
{
  return log_purge_date.date();
}


void Log::setPurgeDate(const QDate &date)
{
  log_purge_date.setDate(date);
}


bool Log::autorefresh() const
{
  return log_autorefresh;
}


void Log::setAutorefresh(bool state)
{
  log_autorefresh=state;
}


int Log::scheduledTracks() const
{
  return log_scheduled_tracks;
}


int Log::completedTracks() const
{
  return log_completed_tracks;
}


int Log::musicLinks() const
{
  return log_music_links;
}


bool Log::musicLinked() const
{
  return log_music_linked;
}


int Log::trafficLinks() const
{
  return log_traffic_links;
}


bool Log::trafficLinked() const
{
  return log_traffic_linked;
}


void Log::insert(int row,unsigned cartnum)
{
  struct rd_cart *cart=NULL;
  LogLine ll;

  if(cnf->listCart(&cart,cartnum)) {
    ll.setCartType((LogLine::CartType)cart->cart_type);
    ll.setType(LogLine::Cart);
    if(ll.cartType()==LogLine::MacroCart) {
      ll.setType(LogLine::Macro);
    }
    ll.setCartNumber(cart->cart_number);
    ll.setGroupName(cart->cart_grp_name);
    ll.setTitle(cart->cart_title);
    ll.setArtist(cart->cart_artist);
    ll.setAlbum(cart->cart_album);
    ll.setYear(cart->cart_year);
    ll.setLabel(cart->cart_label);
    ll.setClient(cart->cart_client);
    ll.setAgency(cart->cart_agency);
    ll.setPublisher(cart->cart_publisher);
    ll.setComposer(cart->cart_composer);
    ll.setConductor(cart->cart_conductor);
    ll.setUserDefined(cart->cart_user_defined);
    ll.setUsageCode((LogLine::UsageCode)cart->cart_usage_code);
    ll.setEnforceLength(cart->cart_enforce_length);
    ll.setForcedLength(cart->cart_forced_length);
    ll.setSource(LogLine::Manual);
    ll.setTimeType(LogLine::Relative);
    ll.setTransType(LogLine::Play);
    ll.setCutQuantity(cart->cart_cut_quantity);
    ll.setLastCutPlayed(cart->cart_last_cut_played);
    ::free(cart);
    ll.setId(GetNextId());
  }
  insert(row,ll);
}


void Log::insert(int row,LogLine ll)
{
  struct rd_cut *cuts=NULL;
  unsigned numrecs=0;
  if((cnf->listCuts(&cuts,ll.cartNumber(),&numrecs)==0)&&(numrecs>0)) {
    ll.setStartPoint(LogLine::CartPointer,cuts->cut_start_point);
    ll.setEndPoint(LogLine::CartPointer,cuts->cut_end_point);
    ll.setCutNumber(cuts->cut_cut_number);
    ll.setEvergreen(cuts->cut_evergreen);
    //    ll.setStartTime(cuts->cut_start_daypart);
    ll.setStartPoint(LogLine::CartPointer,cuts->cut_start_point);
    ll.setEndPoint(LogLine::CartPointer,cuts->cut_end_point);
    ll.setSegueStartPoint(LogLine::CartPointer,cuts->cut_segue_start_point);
    ll.setSegueEndPoint(LogLine::CartPointer,cuts->cut_segue_end_point);
    ll.setSegueGain(cuts->cut_segue_gain);
    ll.setFadeupPoint(LogLine::CartPointer,cuts->cut_fadeup_point);
    ll.setFadedownPoint(LogLine::CartPointer,cuts->cut_fadedown_point);
    ll.setTalkStartPoint(cuts->cut_talk_start_point);
    ll.setTalkEndPoint(cuts->cut_talk_end_point);
    ll.setHookStartPoint(cuts->cut_hook_start_point);
    ll.setHookEndPoint(cuts->cut_hook_end_point);
    ::free(cuts);
  }
  QList<LogLine>::insert(row,ll);
  SetStartTimes();
}


bool Log::load(const QString &name,QString *err_msg)
{
  //
  // Header Data
  //
  struct rd_log *loghdr=NULL;
  unsigned log_quan=0;
  int err=0;

  if((err=cnf->listLogs(&loghdr,&log_quan,"",name,false))!=0) {
    *err_msg=QString().sprintf("RD_ListLogs() failed [code: %d]",err);
    return false;
  }
  if(log_quan==0) {
    *err_msg="no such log";
    return false;
  }
  if(log_quan>1) {
    *err_msg=
      QString().sprintf("RD_ListLogs() found %u logs matching that name!",
			log_quan);
    return false;
  }
  log_name=name;
  log_service_name=loghdr->log_service;
  log_description=loghdr->log_description;
  log_origin_username=loghdr->log_origin_username;
  log_origin_date_time=DateTime::fromTm(loghdr->log_origin_datetime);
  log_link_date_time=DateTime::fromTm(loghdr->log_link_datetime);
  log_modified_date_time=DateTime::fromTm(loghdr->log_modified_datetime);
  log_purge_date=DateTime::fromTm(loghdr->log_purge_date);
  log_start_date=DateTime::fromTm(loghdr->log_startdate);
  log_end_date=DateTime::fromTm(loghdr->log_enddate);
  log_autorefresh=loghdr->log_autorefresh;
  log_scheduled_tracks=loghdr->log_scheduled_tracks;
  log_completed_tracks=loghdr->log_completed_tracks;
  log_music_links=loghdr->log_music_links;
  log_music_linked=loghdr->log_music_linked;
  log_traffic_links=loghdr->log_traffic_links;
  log_traffic_linked=loghdr->log_traffic_linked;
  ::free(loghdr);
  
  //
  // Event Data
  //
  struct rd_logline *ll=NULL;
  unsigned ll_quan=0;

  if((err=cnf->listLog(&ll,&ll_quan,log_name)!=0)) {
    *err_msg=QString().sprintf("RD_ListLog() failed [code: %d]",err);
    return false;
  }
  clear();
  for(unsigned i=0;i<ll_quan;i++) {
    append(LogLine());
    back().setId(ll[i].logline_id);
    back().setType((LogLine::Type)ll[i].logline_type);
    back().setCartType((LogLine::CartType)ll[i].logline_cart_type);
    back().setCartNumber(ll[i].logline_cart_number);
    back().setCutNumber(ll[i].logline_cut_number);
    back().setGroupName(ll[i].logline_group_name);
    back().setGroupColor(ll[i].logline_group_color);
    back().setTitle(ll[i].logline_title);
    back().setArtist(ll[i].logline_artist);
    back().setAlbum(ll[i].logline_album);
    back().setYear(ll[i].logline_year);
    back().setLabel(ll[i].logline_label);
    back().setClient(ll[i].logline_client);
    back().setAgency(ll[i].logline_agency);
    back().setPublisher(ll[i].logline_publisher);
    back().setComposer(ll[i].logline_composer);
    back().setConductor(ll[i].logline_conductor);
    back().setUserDefined(ll[i].logline_user_defined);
    back().setUsageCode((LogLine::UsageCode)ll[i].logline_usage_code);
    back().setEnforceLength(ll[i].logline_enforce_length);
    back().setForcedLength(ll[i].logline_forced_length);
    back().setEvergreen(ll[i].logline_evergreen);
    back().setSource((LogLine::Source)ll[i].logline_source);
    back().setTimeType((LogLine::TimeType)ll[i].logline_time_type);
    back().setStartTime(QTime().addMSecs(ll[i].logline_starttime));
    back().setTransType((LogLine::TransType)ll[i].logline_transition_type);
    back().setCutQuantity(ll[i].logline_cut_quantity);
    back().setLastCutPlayed(ll[i].logline_last_cut_played);
    back().setMarkerComment(ll[i].logline_marker_comment);
    back().setMarkerLabel(ll[i].logline_marker_label);
    back().setOriginUser(ll[i].logline_origin_user);
    back().setOriginDateTime(DateTime::fromTm(ll[i].logline_origin_datetime));
    back().setStartPoint(LogLine::CartPointer,ll[i].logline_start_point_cart);
    back().setStartPoint(LogLine::LogPointer,ll[i].logline_start_point_log);
    back().setEndPoint(LogLine::CartPointer,ll[i].logline_end_point_cart);
    back().setEndPoint(LogLine::LogPointer,ll[i].logline_end_point_log);
    back().setSegueStartPoint(LogLine::CartPointer,
			      ll[i].logline_segue_start_point_cart);
    back().setSegueStartPoint(LogLine::LogPointer,
			      ll[i].logline_segue_start_point_log);
    back().setSegueEndPoint(LogLine::CartPointer,
			    ll[i].logline_segue_end_point_cart);
    back().setSegueEndPoint(LogLine::LogPointer,
			    ll[i].logline_segue_end_point_log);
    back().setSegueGain(ll[i].logline_segue_gain);
    back().setFadeupPoint(LogLine::CartPointer,ll[i].logline_fadeup_point_cart);
    back().setFadeupPoint(LogLine::LogPointer,ll[i].logline_fadeup_point_log);
    back().setFadedownPoint(LogLine::CartPointer,
			    ll[i].logline_fadedown_point_cart);
    back().setFadedownPoint(LogLine::LogPointer,ll[i].logline_fadedown_point_log);
    back().setDuckupGain(ll[i].logline_duckup_gain);
    back().setDuckdownGain(ll[i].logline_duckdown_gain);
    back().setTalkStartPoint(ll[i].logline_talk_start_point);
    back().setTalkEndPoint(ll[i].logline_talk_end_point);
    back().setHookMode(ll[i].logline_hook_mode);
    back().setHookStartPoint(ll[i].logline_hook_start_point);
    back().setHookEndPoint(ll[i].logline_hook_end_point);
    back().setEventLength(ll[i].logline_event_length);
    back().setLinkEventName(ll[i].logline_link_event_name);
    back().setLinkStartTime(QTime().addMSecs(ll[i].logline_link_starttime));
    back().setLinkStartSlop(ll[i].logline_link_start_slop);
    back().setLinkEndSlop(ll[i].logline_link_end_slop);
    back().setLinkId(ll[i].logline_link_id);
    back().setLinkEmbedded(ll[i].logline_link_embedded);
    back().setExtStartTime(QTime().addMSecs(ll[i].logline_ext_starttime));
    back().setExtLength(ll[i].logline_ext_length);
    back().setExtCartName(ll[i].logline_ext_cart_name);
    back().setExtData(ll[i].logline_ext_data);
    back().setExtEventId(ll[i].logline_ext_event_id);
    back().setExtAnncType(ll[i].logline_ext_annc_type);
  }
  ::free(ll);
  *err_msg="OK";

  SetStartTimes();

  return true;
}


bool Log::save(const QString &name,QString *err_msg)
{
  int err=0;

  //
  // Header Data
  //
  struct save_loghdr_values loghdr;

  memset(&loghdr,0,sizeof(loghdr));
  strncpy(loghdr.loghdr_service,log_service_name.toUtf8(),11);
  strncpy(loghdr.loghdr_description,log_description.toUtf8(),65);
  loghdr.loghdr_autorefresh=log_autorefresh;
  loghdr.loghdr_purge_date=log_purge_date.toTm();
  loghdr.loghdr_start_date=log_start_date.toTm();
  loghdr.loghdr_end_date=log_end_date.toTm();

  //
  // Event Data
  //
  struct save_logline_values *ll=new struct save_logline_values[size()];
  for(int i=0;i<size();i++) {
    ll[i].logline_id=at(i).id();
    ll[i].logline_type=at(i).type();
    ll[i].logline_source=at(i).source();
    ll[i].logline_cart_number=at(i).cartNumber();
    ll[i].logline_starttime=QTime().msecsTo(at(i).startTime());
    ll[i].logline_gracetime=at(i).graceTime();
    ll[i].logline_time_type=at(i).timeType();
    ll[i].logline_transition_type=at(i).transType();
    ll[i].logline_start_point_log=at(i).startPoint(LogLine::LogPointer);
    ll[i].logline_end_point_log=at(i).endPoint(LogLine::LogPointer);
    ll[i].logline_segue_start_point_log=
      at(i).segueStartPoint(LogLine::LogPointer);
    ll[i].logline_segue_end_point_log=at(i).segueEndPoint(LogLine::LogPointer);
    ll[i].logline_fadeup_point_log=at(i).fadeupPoint(LogLine::LogPointer);
    ll[i].logline_fadeup_gain=at(i).fadeupGain();
    ll[i].logline_fadedown_point_log=at(i).fadedownPoint(LogLine::LogPointer);
    ll[i].logline_fadedown_gain=at(i).fadedownGain();
    ll[i].logline_duckup_gain=at(i).duckupGain();
    ll[i].logline_duckdown_gain=at(i).duckdownGain();
    strncpy(ll[i].logline_marker_label,at(i).markerLabel().toUtf8(),64);
    strncpy(ll[i].logline_marker_comment,at(i).markerComment().toUtf8(),255);
    ll[i].logline_origin_datetime=DateTime(at(i).originDateTime()).toTm();
    strncpy(ll[i].logline_origin_user,at(i).originUser().toUtf8(),255);
    ll[i].logline_event_length=at(i).eventLength();
    strncpy(ll[i].logline_link_event_name,at(i).linkEventName().toUtf8(),64);
    ll[i].logline_link_starttime=DateTime(QDate(),at(i).linkStartTime()).toTm();
    ll[i].logline_link_length=at(i).linkLength();
    ll[i].logline_link_start_slop=at(i).linkStartSlop();
    ll[i].logline_link_end_slop=at(i).linkEndSlop();
    ll[i].logline_link_id=at(i).linkId();
    ll[i].logline_link_embedded=at(i).linkEmbedded();
    ll[i].logline_ext_starttime=DateTime(QDate(),at(i).extStartTime()).toTm();
    ll[i].logline_ext_length=at(i).extLength();
    strncpy(ll[i].logline_ext_cart_name,at(i).extCartName().toUtf8(),32);
    strncpy(ll[i].logline_ext_data,at(i).extData().toUtf8(),32);
    strncpy(ll[i].logline_ext_event_id,at(i).extEventId().toUtf8(),32);
    strncpy(ll[i].logline_ext_annc_type,at(i).extAnncType().toUtf8(),8);
  }
  if((err=cnf->saveLog(&loghdr,ll,size(),name.toUtf8()))!=0) {
    *err_msg=QString().sprintf("RD_SaveLog() failed [code: %d]",err);
    return false;
  }
  delete ll;

  return true;
}


int Log::GetNextId()
{
  int id=0;

  for(int i=0;i<size();i++) {
    if(at(i).id()>=id) {
      id=at(i).id()+1;
    }
  }
  return id;
}


void Log::update()
{
  SetStartTimes();
}


void Log::SetStartTimes()
{
  QTime current;

  for(int i=0;i<size();i++) {
    if(at(i).timeType()==LogLine::Hard) {
      current=at(i).startTime();
    }
    else {
      (*this)[i].setStartTime(current);
    }
    if((at(i).transType()==LogLine::Segue)&&
       (at(i).segueStartPoint(LogLine::AutoPointer))>=0) {
      current=current.addMSecs(at(i).segueStartPoint(LogLine::AutoPointer)-
			       at(i).startPoint(LogLine::AutoPointer));
    }
    else {
      current=current.addMSecs(at(i).length(LogLine::AutoPointer));
    }
  }
}
