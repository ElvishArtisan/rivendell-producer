// config.h
//
// Rivendell-Producer configuration
//
//   (C) Copyright 2016-2017 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

#include <QDateTime>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSplashScreen>
#include <QString>

#include <rivendell/rd_addlog.h>
#include <rivendell/rd_deletelog.h>
#include <rivendell/rd_listcart.h>
#include <rivendell/rd_listcarts.h>
#include <rivendell/rd_listcut.h>
#include <rivendell/rd_listcuts.h>
#include <rivendell/rd_listgroups.h>
#include <rivendell/rd_listlog.h>
#include <rivendell/rd_listlogs.h>
#include <rivendell/rd_listservices.h>
#include <rivendell/rd_savelog.h>

#define DEFAULT_CONF_FILE QString("/etc/rivendell-producer.conf")
#define DEFAULT_SERVER_HOSTNAME "172.30.5.133"
#define DEFAULT_SERVER_USERNAME ""
#define DEFAULT_SERVER_PASSWORD ""

#define DEFAULT_AUDIO_DEVICE_NAME QString("plughw:0")

#define DEFAULT_UI_INCLUDE_ALL_GROUP 0

#define RD_FADE_DEPTH -3000

class Config : public QDialog
{
  Q_OBJECT;
 public:
  Config(const QString &splash_path,QWidget *parent=0);
  QSize sizeHint() const;
  QSplashScreen *splashScreen() const;
  QString serverHostname() const;
  void setServerHostname(const QString &str);
  QString serverUsername() const;
  void setServerUsername(const QString &str);
  QString serverPassword() const;
  void setServerPassword(const QString &str);
  QString serverTicket() const;
  void setServerTicket(const QString &str);
  QDateTime serverTicketExpiration() const;
  void setServerTicketExpiration(const QDateTime &dt);
  QString audioDeviceName() const;
  void setAudioDeviceName(const QString &str);
  bool uiIncludeAllGroup() const;
  void setUiIncludeAllGroup(bool state);
  int addLog(const QString &logname,const QString &svcname);
  int deleteLog(const QString &logname);
  bool listCart(struct rd_cart **cart,const unsigned cartnum);
  int listCarts(struct rd_cart **carts,unsigned *numrecs,
		const QString &grp_name,const QString &filter,
		const QString &type);
  int listCuts(struct rd_cut **cuts,const unsigned cartnum,unsigned *numrecs);
  int listCut(struct rd_cut **cuts,const unsigned cartnum,
	      const unsigned cutnum,unsigned *numrecs);
  int listGroups(struct rd_group **grps,unsigned *numrecs);
  int listLog(struct rd_logline **lines,unsigned *numrecs,
	      const QString &logname);
  int listLogs(struct rd_log **logs,unsigned *numrecs,
	       const QString &logname,const QString &svcname,
	       bool trackable);
  int listServices(struct rd_service **svcs,unsigned *numrecs,
		   bool trackable);
  int saveLog(struct save_loghdr_values *hdr,struct save_logline_values *lines,
	      unsigned numrecs,const QString &logname);
  bool load(bool use_env=true);
  bool save();

 private slots:
  void okData();
  void quitData();

 protected:
  void closeEvent(QCloseEvent *e);
  void resizeEvent(QResizeEvent *e);

 private:
  void LoadTicket();
  void SaveTicket() const;
  void LockIdentity();
  QSplashScreen *conf_splash_screen;
  QLabel *conf_hostname_label;
  QLineEdit *conf_hostname_edit;
  QLabel *conf_username_label;
  QLineEdit *conf_username_edit;
  QLabel *conf_password_label;
  QLineEdit *conf_password_edit;
  QString conf_server_ticket;
  QDateTime conf_server_ticket_expiration;
  QPushButton *conf_ok_button;
  QPushButton *conf_quit_button;
  QString conf_audio_device_name;
  bool conf_ui_include_all_group;
};

extern Config *cnf; 

#endif  // CONFIG_H
