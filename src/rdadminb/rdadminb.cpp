// rdadminb.cpp
//
// Configuration applet for rivendell-producer
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

#include <QApplication>
#include <QPixmap>

#ifdef MME
#include <windows.h>
#include <mmreg.h>
#include <mmsystem.h>
#endif  // MME

#include "rdadminb.h"

#include "../../icons/rdadminb-16x16.xpm"

MainWidget::MainWidget(QWidget *parent)
  : QMainWindow(parent)
{
  admin_config=new Config("rdadminb","",this);
  admin_config->load(false);

  setMinimumSize(sizeHint());
  setMaximumHeight(sizeHint().height());
  setWindowTitle(tr("RDAdmin Remote")+" v"+VERSION);
  setWindowIcon(QPixmap(rdadminb_16x16_xpm));

  //
  // Fonts
  //
  QFont bold_font(font().family(),font().pointSize(),QFont::Bold);

  admin_server_hostname_label=new QLabel(tr("RDXport Hostname")+":",this);
  admin_server_hostname_label->setFont(bold_font);
  admin_server_hostname_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  admin_server_hostname_edit=new QLineEdit(this);
  admin_server_hostname_edit->setText(admin_config->serverHostname());

  admin_server_username_label=new QLabel(tr("Rivendell Username")+":",this);
  admin_server_username_label->setFont(bold_font);
  admin_server_username_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  admin_server_username_edit=new QLineEdit(this);
  admin_server_username_edit->setText(admin_config->serverUsername());

  admin_server_password_label=new QLabel(tr("Rivendell Password")+":",this);
  admin_server_password_label->setFont(bold_font);
  admin_server_password_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  admin_server_password_edit=new QLineEdit(this);
  admin_server_password_edit->setEchoMode(QLineEdit::Password);
  admin_server_password_edit->setText(admin_config->serverPassword());

  admin_audio_devicename_label=new QLabel(tr("Audio Device")+":",this);
  admin_audio_devicename_label->setFont(bold_font);
  admin_audio_devicename_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  admin_audio_devicename_box=new ComboBox(this);
  admin_audio_devicename_edit=new QLineEdit(this);
#ifdef MME
  admin_audio_devicename_edit->hide();
  int devs=waveOutGetNumDevs();
  for(int i=0;i<devs;i++) {
    WAVEOUTCAPS caps;
    if(waveOutGetDevCaps(i,&caps,sizeof(caps))==0) {
      admin_audio_devicename_box->insertItem(-1,caps.szPname,i);
    }
  }
  admin_audio_devicename_box->
    setCurrentItemData(admin_config->audioDeviceName());
#else
  admin_audio_devicename_box->hide();
  admin_audio_devicename_edit->setText(admin_config->audioDeviceName());
#endif  // MME

  admin_ui_allgroup_label=new QLabel(tr("Include \"ALL\" in Group List"),this);
  admin_ui_allgroup_label->setFont(bold_font);
  admin_ui_allgroup_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
  admin_ui_allgroup_check=new QCheckBox(this);
  admin_ui_allgroup_check->setChecked(admin_config->uiIncludeAllGroup());

  admin_ok_button=new QPushButton(tr("OK"),this);
  admin_ok_button->setFont(bold_font);
  connect(admin_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  admin_cancel_button=new QPushButton(tr("Cancel"),this);
  admin_cancel_button->setFont(bold_font);
  connect(admin_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));
}


QSize MainWidget::sizeHint() const
{
  return QSize(400,180);
}


void MainWidget::okData()
{
  admin_config->setServerHostname(admin_server_hostname_edit->text());
  admin_config->setServerUsername(admin_server_username_edit->text());
  admin_config->setServerPassword(admin_server_password_edit->text());
#ifdef MME
  admin_config->
    setAudioDeviceName(QString().
    sprintf("%d",admin_audio_devicename_box->currentItemData().toInt()));
#else
  admin_config->setAudioDeviceName(admin_audio_devicename_edit->text());
#endif  // MME
  admin_config->setUiIncludeAllGroup(admin_ui_allgroup_check->isChecked());

  admin_config->save();
  exit(0);
}


void MainWidget::cancelData()
{
  exit(0);
}


void MainWidget::closeEvent(QCloseEvent *e)
{
  cancelData();
}


void MainWidget::resizeEvent(QResizeEvent *e)
{
  int w=size().width();
  int h=size().height();

  admin_server_hostname_label->setGeometry(10,10,170,20);
  admin_server_hostname_edit->setGeometry(185,10,w-195,20);

  admin_server_username_label->setGeometry(10,32,170,20);
  admin_server_username_edit->setGeometry(185,32,w-195,20);

  admin_server_password_label->setGeometry(10,54,170,20);
  admin_server_password_edit->setGeometry(185,54,w-195,20);

  admin_audio_devicename_label->setGeometry(10,90,170,20);
  admin_audio_devicename_edit->setGeometry(185,90,w-195,20);
  admin_audio_devicename_box->setGeometry(185,90,w-195,20);

  admin_ui_allgroup_check->setGeometry(20,112,20,20);
  admin_ui_allgroup_label->setGeometry(45,112,300,20);

  admin_ok_button->setGeometry(w-180,h-40,80,30);
  admin_cancel_button->setGeometry(w-90,h-40,80,30);
}


int main(int argc,char *argv[])
{
  QApplication a(argc,argv);

  MainWidget *w=new MainWidget();
  w->show();

  return a.exec();
}
