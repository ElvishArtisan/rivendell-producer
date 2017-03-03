// config.cpp
//
// Rivendell-Browser configuration
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
#include <stdlib.h>

#include <QCoreApplication>
#include <QMessageBox>
#include <QSettings>

#include <rivendell/rd_listgroups.h>

#include "config.h"
#include "profile.h"

Config *cnf; 

Config::Config(QWidget *parent)
  : QDialog(parent)
{
  setWindowTitle(tr("Rivendell Login"));
  QCoreApplication::setOrganizationName("Radio Free Asia");
  QCoreApplication::setOrganizationDomain("rfa.org");
  QCoreApplication::setApplicationName("rivendell-browser");

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
  // Cancel Button
  //
  conf_cancel_button=new QPushButton(tr("Cancel"),this);
  conf_cancel_button->setFont(bold_font);
  connect(conf_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));
}


QSize Config::sizeHint() const
{
  return QSize(400,150);
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
  struct rd_group *groups;
  unsigned records=0;
  int err=0;
  if((err=RD_ListGroups(&groups,serverHostname().toUtf8(),
			serverUsername().toUtf8(),
			serverPassword().toUtf8(),&records))!=0) {
    if(err==403) {
      QMessageBox::warning(this,tr("Rivendell Login"),
			   tr("Incorrect Username or Password."));
      return;
    }
    QMessageBox::warning(this,tr("Rivendell Login"),
			 tr("Error in rd_groups() call")+
			 " ["+tr("Error")+QString().sprintf(" %d].",err));
    return;
  }
  done(true);
}


void Config::cancelData()
{
  done(false);
}


void Config::closeEvent(QCloseEvent *e)
{
  cancelData();
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
  conf_cancel_button->setGeometry(size().width()-90,size().height()-60,80,50);
}
