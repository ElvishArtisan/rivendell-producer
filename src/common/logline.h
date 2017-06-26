// logline.h
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

#ifndef LOGLINE_H
#define LOGLINE_H

#include <QColor>
#include <QDateTime>
#include <QString>

#include "datetime.h"

class LogLine
{
 public:
  enum TimeType {Relative=0,Hard=1,NoTime=255};
  enum TransType {Play=0,Segue=1,Stop=2,NoTrans=255};
  enum Type {Cart=0,Marker=1,Macro=2,OpenBracket=3,CloseBracket=4,Chain=5,
	     Track=6,MusicLink=7,TrafficLink=8,UnknownType=9};
  enum CartType {All=0,AudioCart=1,MacroCart=2};
  enum PointerSource {CartPointer=0,LogPointer=1,AutoPointer=2};
  enum Source {Manual=0,Traffic=1,Music=2,Template=3,Tracker=4};
  enum UsageCode {UsageFeature=0,UsageOpen=1,UsageClose=2,UsageTheme=3,
		  UsageBackground=4,UsagePromo=5,UsageLast=6};
  LogLine();
  int id() const;
  void setId(int id);
  Type type() const;
  void setType(Type type);
  CartType cartType() const;
  void setCartType(CartType type);
  unsigned cartNumber() const;
  void setCartNumber(unsigned cartnum);
  int cutNumber() const;
  void setCutNumber(int cutnum);
  QString groupName() const;
  void setGroupName(const QString &str);
  QColor groupColor() const;
  void setGroupColor(const QColor &color);
  QString title() const;
  void setTitle(const QString &str);
  QString artist() const;
  void setArtist(const QString &str);
  QString album() const;
  void setAlbum(const QString &str);
  int year() const;
  void setYear(int year);
  QString label() const;
  void setLabel(const QString &str);
  QString client() const;
  void setClient(const QString &str);
  QString agency() const;
  void setAgency(const QString &str);
  QString publisher() const;
  void setPublisher(const QString &str);
  QString composer() const;
  void setComposer(const QString &str);
  QString conductor() const;
  void setConductor(const QString &str);
  QString userDefined() const;
  void setUserDefined(const QString &str);
  UsageCode usageCode() const;
  void setUsageCode(UsageCode code);
  bool enforceLength() const;
  void setEnforceLength(bool state);
  int forcedLength() const;
  void setForcedLength(int msec);
  bool evergreen() const;
  void setEvergreen(bool state);
  Source source() const;
  void setSource(Source src);
  TimeType timeType() const;
  void setTimeType(TimeType type);
  QTime startTime() const;
  void setStartTime(const QTime &time);
  int graceTime() const;
  void setGraceTime(int msec);
  TransType transType() const;
  void setTransType(TransType type);
  int cutQuantity() const;
  void setCutQuantity(int quan);
  int lastCutPlayed() const;
  void setLastCutPlayed(int cutnum);
  QString markerComment() const;
  void setMarkerComment(const QString &str);
  QString markerLabel() const;
  void setMarkerLabel(const QString &str);
  QString originUser() const;
  void setOriginUser(const QString &str);
  QDateTime originDateTime() const;
  void setOriginDateTime(const QDateTime &datetime);
  int startPoint(PointerSource src) const;
  void setStartPoint(PointerSource src,int msec);
  int endPoint(PointerSource src) const;
  void setEndPoint(PointerSource src,int msec);
  int segueStartPoint(PointerSource src) const;
  void setSegueStartPoint(PointerSource src,int msec);
  int segueEndPoint(PointerSource src) const;
  void setSegueEndPoint(PointerSource src,int msec);
  int segueGain() const;
  void setSegueGain(int gain);
  int fadeupPoint(PointerSource src) const;
  void setFadeupPoint(PointerSource src,int msec);
  int fadeupGain() const;
  void setFadeupGain(int gain);
  int fadedownPoint(PointerSource src) const;
  void setFadedownPoint(PointerSource src,int msec);
  int fadedownGain() const;
  void setFadedownGain(int gain);
  int duckupGain() const;
  void setDuckupGain(int gain);
  int duckdownGain() const;
  void setDuckdownGain(int gain);
  int talkStartPoint() const;
  void setTalkStartPoint(int msec);
  int talkEndPoint() const;
  void setTalkEndPoint(int msec);
  int hookStartPoint() const;
  void setHookStartPoint(int msec);
  int hookEndPoint() const;
  void setHookEndPoint(int msec);
  bool hookMode() const;
  void setHookMode(bool state);
  int eventLength() const;
  void setEventLength(int msec);
  QString linkEventName() const;
  void setLinkEventName(const QString &str);
  QTime linkStartTime() const;
  void setLinkStartTime(const QTime &time);
  int linkLength() const;
  void setLinkLength(int msec);
  int linkStartSlop() const;
  void setLinkStartSlop(int msec);
  int linkEndSlop() const;
  void setLinkEndSlop(int msec);
  int linkId() const;
  void setLinkId(int id);
  bool linkEmbedded() const;
  void setLinkEmbedded(bool state);
  QTime extStartTime() const;
  void setExtStartTime(const QTime &time);
  int extLength() const;
  void setExtLength(int msec);
  QString extCartName() const;
  void setExtCartName(const QString &str);
  QString extData() const;
  void setExtData(const QString &str);
  QString extEventId() const;
  void setExtEventId(const QString &str);
  QString extAnncType() const;
  void setExtAnncType(const QString &str);
  int length(PointerSource src) const;
  void clear();
  static QString sourceText(Source src);
  static QString transText(TransType type);

 private:
  int logline_id;
  Type logline_type;
  CartType logline_cart_type;
  unsigned logline_cart_number;
  int logline_cut_number;
  QString logline_group_name;
  QColor logline_group_color;
  QString logline_title;
  QString logline_artist;
  QString logline_album;
  int logline_year;
  QString logline_label;
  QString logline_client;
  QString logline_agency;
  QString logline_publisher;
  QString logline_composer;
  QString logline_conductor;
  QString logline_user_defined;
  UsageCode logline_usage_code;
  bool logline_enforce_length;
  int logline_forced_length;
  bool logline_evergreen;
  Source logline_source;
  TimeType logline_time_type;
  QTime logline_start_time;
  int logline_grace_time;
  TransType logline_trans_type;
  int logline_cut_quantity;
  int logline_last_cut_played;
  QString logline_marker_label;
  QString logline_marker_comment;
  QString logline_origin_user;
  DateTime logline_origin_date_time;
  int logline_start_point[2];
  int logline_end_point[2];
  int logline_segue_start_point[2];
  int logline_segue_end_point[2];
  int logline_segue_gain;
  int logline_fadeup_point[2];
  int logline_fadeup_gain;
  int logline_fadedown_point[2];
  int logline_fadedown_gain;
  int logline_duckup_gain;
  int logline_duckdown_gain;
  int logline_talk_start_point;
  int logline_talk_end_point;
  int logline_hook_start_point;
  int logline_hook_end_point;
  bool logline_hook_mode;
  int logline_event_length;
  QString logline_link_event_name;
  QTime logline_link_start_time;
  int logline_link_length;
  int logline_link_start_slop;
  int logline_link_end_slop;
  int logline_link_id;
  bool logline_link_embedded;
  QTime logline_ext_start_time;
  int logline_ext_length;
  QString logline_ext_cart_name;
  QString logline_ext_data;
  QString logline_ext_event_id;
  QString logline_ext_annc_type;
};


#endif  // LOGLINE_H
