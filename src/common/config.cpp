// config.cpp
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

#include <stdio.h>
#include <stdlib.h>

#include <QCoreApplication>
#include <QDir>
#include <QMessageBox>
#include <QSettings>

#include <rivendell/rd_createticket.h>

#include "config.h"
#include "datetime.h"
#include "profile.h"

Config *cnf; 

Config::Config(const QString &module_name,const QString &splash_path,
	       QWidget *parent)
  : QDialog(parent,Qt::WindowStaysOnTopHint)
{
  setWindowTitle(tr("Rivendell Login"));
  QCoreApplication::setOrganizationName("Radio Free Asia");
  QCoreApplication::setOrganizationDomain("rfa.org");
  QCoreApplication::setApplicationName("Rivendell Producer");

  //
  // HTTP User Agent
  //
  conf_user_agent=QString("rivendell-producer/")+VERSION+" ("+module_name+")";

  //
  // Splash Screen
  //
  //  QPixmap pix("/usr/share/rivendell-producer/rdlogeditb-splashscreen.png");
  conf_splash_screen=new QSplashScreen(QPixmap(splash_path));
  conf_splash_screen->show();
  conf_splash_screen->showMessage("Contacting server...",Qt::AlignRight);
  qApp->processEvents(QEventLoop::AllEvents);

  //
  // Fonts
  //
  QFont bold_font(font().family(),font().pointSize(),QFont::Bold);

  //
  // Hostname
  //
  conf_hostname_label=new QLabel(tr("RDXport Hostname")+":",this);
  conf_hostname_label->setFont(bold_font);
  conf_hostname_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  conf_hostname_edit=new QLineEdit(this);

  //
  // Username
  //
  conf_username_label=new QLabel(tr("Username")+":",this);
  conf_username_label->setFont(bold_font);
  conf_username_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  conf_username_edit=new QLineEdit(this);
  conf_username_edit->setMaxLength(255);

  //
  // Password
  //
  conf_password_label=new QLabel(tr("Password")+":",this);
  conf_password_label->setFont(bold_font);
  conf_password_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  conf_password_edit=new QLineEdit(this);
  conf_password_edit->setEchoMode(QLineEdit::Password);

  //
  // OK Button
  //
  conf_ok_button=new QPushButton(tr("OK"),this);
  conf_ok_button->setFont(bold_font);
  connect(conf_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  // Quit Button
  //
  conf_quit_button=new QPushButton(tr("Quit"),this);
  conf_quit_button->setFont(bold_font);
  connect(conf_quit_button,SIGNAL(clicked()),this,SLOT(quitData()));
}


QSize Config::sizeHint() const
{
  return QSize(400,150);
}


QSplashScreen *Config::splashScreen() const
{
  return conf_splash_screen;
}


QString Config::serverHostname() const
{
  return conf_hostname_edit->text();
}


void Config::setServerHostname(const QString &str)
{
  conf_hostname_edit->setText(str);
}


QString Config::serverUsername() const
{
  return conf_username_edit->text();
}


void Config::setServerUsername(const QString &str)
{
  conf_username_edit->setText(str);
}


QString Config::serverPassword() const
{
  return conf_password_edit->text();
}


void Config::setServerPassword(const QString &str)
{
  conf_password_edit->setText(str);
}


QString Config::serverTicket() const
{
  return conf_server_ticket;
}


void Config::setServerTicket(const QString &str)
{
  conf_server_ticket=str;
}


QDateTime Config::serverTicketExpiration() const
{
  return conf_server_ticket_expiration;
}


void Config::setServerTicketExpiration(const QDateTime &dt)
{
  conf_server_ticket_expiration=dt;
}


QString Config::audioDeviceName() const
{
  return conf_audio_device_name;
}


void Config::setAudioDeviceName(const QString &str)
{
  conf_audio_device_name=str;
}


bool Config::uiIncludeAllGroup() const
{
  return conf_ui_include_all_group;
}


void Config::setUiIncludeAllGroup(bool state)
{
  conf_ui_include_all_group=state;
}


QString Config::userAgent() const
{
  return conf_user_agent;
}


int Config::addLog(const QString &logname,const QString &svcname)
{
  int ret;
  do {
    ret=RD_AddLog(serverHostname().toUtf8(),serverUsername().toUtf8(),
		  serverPassword().toUtf8(),serverTicket().toUtf8(),
		  logname.toUtf8(),svcname.toUtf8(),
		  conf_user_agent.toUtf8());
    if(ret==0) {
      LockIdentity();
      return ret;
    }
    if((ret==403)||(ret==-1)) {
      if(!exec()) {
	exit(0);
      }
    }
  } while((ret==403)||(ret==-1));

  return ret;
}


int Config::deleteLog(const QString &logname)
{
  int ret;
  do {
    ret=RD_DeleteLog(serverHostname().toUtf8(),serverUsername().toUtf8(),
		     serverPassword().toUtf8(),serverTicket().toUtf8(),
		     logname.toUtf8(),
		     conf_user_agent.toUtf8());
    if(ret==0) {
      LockIdentity();
      return ret;
    }
    if((ret==403)||(ret==-1)) {
      if(!exec()) {
	exit(0);
      }
    }
  } while((ret==403)||(ret==-1));

  return ret;  
}


bool Config::listCart(struct rd_cart **cart,const unsigned cartnum)
{
  int ret;
  unsigned numrecs=0;
  do {
    ret=RD_ListCart(cart,serverHostname().toUtf8(),serverUsername().toUtf8(),
		    serverPassword().toUtf8(),serverTicket().toUtf8(),
		    cartnum,conf_user_agent.toUtf8(),&numrecs);
    if(ret==0) {
      LockIdentity();
      return true;
    }
    if((ret==403)||(ret==-1)) {
      if(!exec()) {
	exit(0);
      }
    }
  } while((ret==403)||(ret==-1));

  return false;
}


int Config::listCarts(struct rd_cart **carts,unsigned *numrecs,
		      const QString &grp_name,const QString &filter,
		      const QString &type)
{
  int ret;
  do {
    ret=RD_ListCarts(carts,serverHostname().toUtf8(),serverUsername().toUtf8(),
		     serverPassword().toUtf8(),serverTicket().toUtf8(),
		     grp_name.toUtf8(),filter.toUtf8(),type.toUtf8(),
		     conf_user_agent.toUtf8(),numrecs);
    if(ret==0) {
      LockIdentity();
      return ret;
    }
    if((ret==403)||(ret==-1)) {
      if(!exec()) {
	exit(0);
      }
    }
  } while((ret==403)||(ret==-1));

  return ret;
}


int Config::listCuts(struct rd_cut **cuts,const unsigned cartnum,
		     unsigned *numrecs)
{
  int ret;
  do {
    ret=RD_ListCuts(cuts,serverHostname().toUtf8(),serverUsername().toUtf8(),
		    serverPassword().toUtf8(),serverTicket().toUtf8(),
		    cartnum,conf_user_agent.toUtf8(),numrecs);
    if(ret==0) {
      LockIdentity();
      return ret;
    }
    if((ret==403)||(ret==-1)) {
      if(!exec()) {
	exit(0);
      }
    }
  } while((ret==403)||(ret==-1));

  return ret;
}


int Config::listCut(struct rd_cut **cuts,const unsigned cartnum,
		    const unsigned cutnum,unsigned *numrecs)
{
  int ret;
  do {
    ret=RD_ListCut(cuts,serverHostname().toUtf8(),serverUsername().toUtf8(),
		   serverPassword().toUtf8(),serverTicket().toUtf8(),
		   cartnum,cutnum,conf_user_agent.toUtf8(),numrecs);
    if(ret==0) {
      LockIdentity();
      return ret;
    }
    if((ret==403)||(ret==-1)) {
      if(!exec()) {
	exit(0);
      }
    }
  } while((ret==403)||(ret==-1));

  return ret;
}


int Config::listGroups(struct rd_group **grps,unsigned *numrecs)
{
  int ret;
  do {
    ret=RD_ListGroups(grps,serverHostname().toUtf8(),serverUsername().toUtf8(),
		      serverPassword().toUtf8(),serverTicket().toUtf8(),
		      conf_user_agent.toUtf8(),numrecs);
    if(ret==0) {
      LockIdentity();
      return ret;
    }
    if((ret==403)||(ret==-1)) {
      if(!exec()) {
	exit(0);
      }
    }
  } while((ret==403)||(ret==-1));

  return ret;
}


int Config::listLog(struct rd_logline **lines,unsigned *numrecs,
		    const QString &logname)
{
  int ret;
  do {
    ret=RD_ListLog(lines,serverHostname().toUtf8(),serverUsername().toUtf8(),
		   serverPassword().toUtf8(),serverTicket().toUtf8(),
		   logname.toUtf8(),conf_user_agent.toUtf8(),numrecs);
    if(ret==0) {
      LockIdentity();
      return ret;
    }
    if((ret==403)||(ret==-1)) {
      if(!exec()) {
	exit(0);
      }
    }
  } while((ret==403)||(ret==-1));

  return ret;
}


int Config::listLogs(struct rd_log **logs,unsigned *numrecs,
		     const QString &logname,const QString &svcname,
		     bool trackable)
{
  int ret;
  do {
    ret=RD_ListLogs(logs,serverHostname().toUtf8(),serverUsername().toUtf8(),
		    serverPassword().toUtf8(),serverTicket().toUtf8(),
		    logname.toUtf8(),svcname.toUtf8(),trackable,
		    conf_user_agent.toUtf8(),numrecs);
    if(ret==0) {
      LockIdentity();
      return ret;
    }
    if((ret==403)||(ret==-1)) {
      if(!exec()) {
	exit(0);
      }
    }
  } while((ret==403)||(ret==-1));

  return ret;
}


int Config::listServices(struct rd_service **svcs,unsigned *numrecs,
			 bool trackable)
{
  int ret;
  do {
    ret=RD_ListServices(svcs,serverHostname().toUtf8(),
			serverUsername().toUtf8(),
			serverPassword().toUtf8(),serverTicket().toUtf8(),
			trackable,conf_user_agent.toUtf8(),numrecs);
    if(ret==0) {
      LockIdentity();
      return ret;
    }
    if((ret==403)||(ret==-1)) {
      if(!exec()) {
	exit(0);
      }
    }
  } while((ret==403)||(ret==-1));

  return ret;
}


int Config::saveLog(struct save_loghdr_values *hdr,
		    struct save_logline_values *lines,
		    unsigned numrecs,const QString &logname)
{
  int ret;
  do {
    ret=RD_SaveLog(hdr,lines,numrecs,serverHostname().toUtf8(),
		   serverUsername().toUtf8(),serverPassword().toUtf8(),
		   serverTicket().toUtf8(),logname.toUtf8(),
		   conf_user_agent.toUtf8());
    if(ret==0) {
      LockIdentity();
      return ret;
    }
    if((ret==403)||(ret==-1)) {
      if(!exec()) {
	exit(0);
      }
    }
  } while((ret==403)||(ret==-1));

  return ret;
}


bool Config::load(bool use_env)
{
#ifdef WIN32
  QSettings s;
  bool ret=true;
  conf_hostname_edit->
    setText(s.value("Server/Hostname",DEFAULT_SERVER_HOSTNAME).toString());
  conf_username_edit->
    setText(s.value("Server/Username",DEFAULT_SERVER_USERNAME).toString());
  conf_password_edit->
    setText(s.value("Server/Password",DEFAULT_SERVER_PASSWORD).toString());

  conf_audio_device_name=
    s.value("Audio/DeviceName",DEFAULT_AUDIO_DEVICE_NAME).toString();

  conf_ui_include_all_group=
    s.value("Ui/IncludeAllGroup",DEFAULT_UI_INCLUDE_ALL_GROUP).toInt();
#else
  Profile *p=new Profile();
  bool ret=p->setSource(DEFAULT_CONF_FILE);

  conf_hostname_edit->
    setText(p->stringValue("Server","Hostname",DEFAULT_SERVER_HOSTNAME));
  conf_username_edit->
    setText(p->stringValue("Server","Username",DEFAULT_SERVER_USERNAME));
  conf_password_edit->
    setText(p->stringValue("Server","Password",DEFAULT_SERVER_PASSWORD));

  conf_audio_device_name=
    p->stringValue("Audio","DeviceName",DEFAULT_AUDIO_DEVICE_NAME);

  conf_ui_include_all_group=
    p->boolValue("Ui","IncludeAllGroup",DEFAULT_UI_INCLUDE_ALL_GROUP);
#endif  // WIN32

  if(use_env) {
    if(conf_username_edit->text().isEmpty()) {
      if(getenv("USERNAME")!=NULL) {
	conf_username_edit->setText(getenv("USERNAME"));
      }
    }
  }
  if(conf_hostname_edit->text().isEmpty()) {
    conf_hostname_edit->setFocus();
  }
  else {
    if(conf_username_edit->text().isEmpty()) {
      conf_username_edit->setFocus();
    }
    else {
      if(conf_password_edit->text().isEmpty()) {
	conf_password_edit->setFocus();
      }
    }
  }
  LoadTicket();

  return ret;
}


bool Config::save()
{
#ifdef WIN32
  QSettings s;
  s.setValue("Server/Hostname",conf_hostname_edit->text());
  s.setValue("Server/Username",conf_username_edit->text());
  s.setValue("Server/Password",conf_password_edit->text());

  s.setValue("Audio/DeviceName",conf_audio_device_name);

  s.setValue("Ui/IncludeAllGroup",conf_ui_include_all_group);

  return true;
#else
  FILE *f=NULL;

  if((f=fopen((DEFAULT_CONF_FILE+"-new").toUtf8(),"w"))==NULL) {
    return false;
  }
  fprintf(f,"[Server]\n");
  fprintf(f,"Hostname=%s\n",(const char *)conf_hostname_edit->text().toUtf8());
  fprintf(f,"Username=%s\n",(const char *)conf_username_edit->text().toUtf8());
  fprintf(f,"Password=%s\n",(const char *)conf_password_edit->text().toUtf8());
  fprintf(f,"\n");

  fprintf(f,"[Audio]\n");
  fprintf(f,"DeviceName=%s\n",(const char *)conf_audio_device_name.toUtf8());
  fprintf(f,"\n");

  fprintf(f,"[Ui]\n");
  fprintf(f,"IncludeAllGroup=%d\n",conf_ui_include_all_group);
  fprintf(f,"\n");

  fclose(f);
  rename((DEFAULT_CONF_FILE+"-new").toUtf8(),DEFAULT_CONF_FILE.toUtf8());

  return true;
#endif  // WIN32
}


void Config::okData()
{
  struct rd_ticketinfo *tktinfo=NULL;
  unsigned records=0;
  int err=0;
  if((err=RD_CreateTicket(&tktinfo,serverHostname().toUtf8(),
			  serverUsername().toUtf8(),serverPassword().toUtf8(),
			  conf_user_agent.toUtf8(),&records))!=0) {
    if(err==403) {
      QMessageBox::warning(this,tr("Rivendell Login"),
			   tr("Incorrect Username or Password."));
      return;
    }
    QMessageBox::warning(this,tr("Rivendell Login"),
			 tr("Error in RD_CreateTicket() call")+
			 " ["+tr("Error")+QString().sprintf(" %d].",err));
    return;
  }
  setServerTicket(tktinfo->ticket);
  setServerPassword("");
  conf_server_ticket_expiration=
    DateTime::fromTm(tktinfo->tkt_expiration_datetime);
  free(tktinfo);
  SaveTicket();
  LockIdentity();
  conf_splash_screen->showMessage("Building cart list...",Qt::AlignRight);
  done(true);
}


void Config::quitData()
{
  done(false);
}


void Config::closeEvent(QCloseEvent *e)
{
  quitData();
}


void Config::resizeEvent(QResizeEvent *e)
{
  conf_hostname_label->setGeometry(10,10,170,20);
  conf_hostname_edit->setGeometry(185,10,size().width()-195,20);

  conf_username_label->setGeometry(10,32,170,20);
  conf_username_edit->setGeometry(185,32,size().width()-195,20);

  conf_password_label->setGeometry(10,54,170,20);
  conf_password_edit->setGeometry(185,54,size().width()-195,20);

  conf_ok_button->setGeometry(size().width()-180,size().height()-60,80,50);
  conf_quit_button->setGeometry(size().width()-90,size().height()-60,80,50);
}


void Config::LoadTicket()
{
#ifdef MME
  QSettings s("Radio Free Asia","Rivendell");
  conf_server_ticket=s.value("TicketString").toString();
  conf_server_ticket_expiration=
    QDateTime::fromString("yyyyMMddhhmmss",
			  s.value("TicketExpiration").toString());
#endif  // MME

#ifdef ALSA
  Profile *p=new Profile();
  p->setSource(QDir::home().path()+"/.rivendell/ticket");
  conf_server_ticket=p->stringValue("Rivendell","TicketString");
  conf_server_ticket_expiration=
    QDateTime::fromString("yyyyMMddhhmmss",
			  p->stringValue("Rivendell","TicketExpiration"));
  delete p;
#endif  // ALSA
}


void Config::SaveTicket() const
{
#ifdef MME
  QSettings s("Radio Free Asia","Rivendell");
  s.setValue("TicketString",serverTicket());
  s.setValue("TicketExpiration",serverTicketExpiration().
	     toString("yyyyMMddhhmmss"));
#endif  // MME

#ifdef ALSA
  QDir dir(QDir::home());
  FILE *f=NULL;

  if(!dir.exists(".rivendell")) {
    dir.mkdir(".rivendell");
  }
  QString filename=dir.path()+"/.rivendell/ticket-NEW";
  if((f=fopen(filename.toUtf8(),"w"))!=NULL) {
    fprintf(f,"[Rivendell]\n");
    fprintf(f,"TicketString=%s\n",(const char *)serverTicket().toUtf8());
    fprintf(f,"TicketExpiration=%s\n",(const char *)serverTicketExpiration().
	    toString("yyyyMMddhhmmss").toUtf8());
    fclose(f);
    rename(filename.toUtf8(),(dir.path()+"/.rivendell/ticket").toUtf8());
  }
#endif  // ALSA
}


void Config::LockIdentity()
{
  conf_hostname_edit->setReadOnly(true);
  conf_username_edit->setReadOnly(true);
  conf_splash_screen->showMessage("Building cart list...",Qt::AlignRight);
}
