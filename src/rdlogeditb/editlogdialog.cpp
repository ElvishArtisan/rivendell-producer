// editlogdialog.cpp
//
// Remote log editor for Rivendell
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

#include <QApplication>
#include <QMessageBox>
#include <QPainter>

#include <rivendell/rd_listlogs.h>
#include <rivendell/rd_listservices.h>
#include <rivendell/rd_savelog.h>

#include "cmdswitch.h"
#include "datetime.h"
#include "playerfactory.h"

#include "editlogdialog.h"

EditLogDialog::EditLogDialog(QWidget *parent)
  :QDialog(parent)
{
  //
  // Window Title Bar
  //
  setWindowTitle(tr("RDLogEdit - Edit Log"));

  //
  // Fix the Window Size
  //
  setMinimumSize(sizeHint());

  //
  // Fonts
  //
  QFont bold_font(font().family(),font().pointSize(),QFont::Bold);

  //
  // Dialogs
  //
  edit_date_dialog=new DateDialog(QDate::currentDate().year(),
				  QDate::currentDate().year()+10,this);

  //
  // Stream Player
  //
  edit_stream_player=PlayerFactory(cnf,this);
  connect(edit_stream_player,SIGNAL(stateChanged(StreamPlayer::State)),
	  this,SLOT(playerStateChangedData(StreamPlayer::State)));
  connect(edit_stream_player,SIGNAL(error(const QString &)),
	  this,SLOT(playerErrorData(const QString &)));

  //
  // Log Name
  //
  edit_name_label_label=new QLabel(tr("Log Name")+":",this);
  edit_name_label_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  edit_name_label_label->setFont(bold_font);
  edit_name_label=new QLabel(this);
  edit_name_label->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);

  //
  // Log Tracks
  //
  edit_tracks_label_label=new QLabel(tr("Tracks")+":",this);
  edit_tracks_label_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  edit_tracks_label_label->setFont(bold_font);
  edit_tracks_label=new QLabel(this);
  edit_tracks_label->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);

  //
  // Log Origin
  //
  edit_origin_label_label=new QLabel(tr("Origin")+":",this);
  edit_origin_label_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  edit_origin_label_label->setFont(bold_font);
  edit_origin_label=new QLabel(this);
  edit_origin_label->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);

  //
  // Log Description
  //
  edit_description_label=new QLabel(tr("Description")+":",this);
  edit_description_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  edit_description_label->setFont(bold_font);
  edit_description_edit=new QLineEdit(this);

  //
  // Deletion Date
  //
  edit_logdelete_check=new QCheckBox(this);
  connect(edit_logdelete_check,SIGNAL(toggled(bool)),
	  this,SLOT(deleteDateData(bool)));
  edit_logdelete_label=new QLabel(tr("Delete on"),this);
  edit_logdelete_label->setAlignment(Qt::AlignVCenter|Qt::AlignCenter);
  edit_logdelete_label->setFont(bold_font);
  edit_logdelete_edit=new QDateTimeEdit(this);
  edit_logdelete_edit->setDisplayFormat("MM/dd/yyyy");
  edit_logdelete_button=new QPushButton(tr("Select"),this);
  connect(edit_logdelete_button,SIGNAL(clicked()),
	  this,SLOT(deleteDateSelectData()));

  //
  // Service
  //
  edit_service_label=new QLabel(tr("Service")+":",this);
  edit_service_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  edit_service_label->setFont(bold_font);
  edit_service_box=new ComboBox(this);
  struct rd_service *svcs=NULL;
  unsigned svc_quan=0;
  RD_ListServices(&svcs,cnf->serverHostname(),cnf->serverUsername(),
		  cnf->serverPassword(),false,&svc_quan);
  for(unsigned i=0;i<svc_quan;i++) {
    edit_service_box->insertItem(0,svcs[i].service_name,svcs[i].service_name);
  }
  free(svcs);

  //
  // Start Date
  //
  edit_startdate_label=new QLabel(tr("Start Date")+":",this);
  edit_startdate_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  edit_startdate_label->setFont(bold_font);
  edit_startdate_edit=new QDateTimeEdit(this);
  edit_startdate_edit->setDisplayFormat("MM/dd/yyyy");

  //
  // End Date
  //
  edit_enddate_label=new QLabel(tr("End Date")+":",this);
  edit_enddate_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  edit_enddate_label->setFont(bold_font);
  edit_enddate_edit=new QDateTimeEdit(this);
  edit_enddate_edit->setDisplayFormat("MM/dd/yyyy");

  //
  // Autorefresh
  //
  edit_autorefresh_label=new QLabel(tr("Enable AutoRefresh")+":",this);
  edit_autorefresh_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  edit_autorefresh_label->setFont(bold_font);
  edit_autorefresh_box=new ComboBox(this);
  edit_autorefresh_box->insertItem(0,tr("No"));
  edit_autorefresh_box->insertItem(1,tr("Yes"));

  //
  // Start Date Check
  //
  edit_startdate_check=new QCheckBox(this);
  connect(edit_startdate_check,SIGNAL(toggled(bool)),
  	  this,SLOT(startDateData(bool)));
  edit_startdate_check_label=new QLabel(tr("Start Date Enabled"),this);
  edit_startdate_check_label->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
  edit_startdate_check_label->setFont(bold_font);

  //
  // End Date Check
  //
  edit_enddate_check=new QCheckBox(this);
  connect(edit_enddate_check,SIGNAL(toggled(bool)),
  	  this,SLOT(endDateData(bool)));
  edit_enddate_check_label=new QLabel(tr("End Date Enabled"),this);
  edit_enddate_check_label->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
  edit_enddate_check_label->setFont(bold_font);

  //
  // Log Events
  //
  edit_log_model=new LogModel(this);
  edit_log_model->setBoldFont(bold_font);
  edit_log_view=new TableView(this);
  edit_log_view->setModel(edit_log_model);
  edit_log_view->resizeColumnsToContents();
  connect(edit_log_view,SIGNAL(clicked(const QModelIndex &)),
	  this,SLOT(eventClickedData(const QModelIndex &)));

  //
  // Play Button
  //
  edit_play_button=new TransportButton(TransportButton::Play,this);
  edit_play_button->setDisabled(true);
  connect(edit_play_button,SIGNAL(clicked()),this,SLOT(playData()));

  //
  // Stop Button
  //
  edit_stop_button=new TransportButton(TransportButton::Stop,this);
  edit_stop_button->on();
  edit_stop_button->setDisabled(true);
  connect(edit_stop_button,SIGNAL(clicked()),this,SLOT(stopData()));

  //
  // Ok Button
  //
  edit_ok_button=new QPushButton(tr("OK"),this);
  edit_ok_button->setFont(bold_font);
  connect(edit_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  // Cancel Button
  //
  edit_cancel_button=new QPushButton(tr("Cancel"),this);
  edit_cancel_button->setFont(bold_font);
  connect(edit_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));
}


QSize EditLogDialog::sizeHint() const
{
  return QSize(800,600);
}


int EditLogDialog::exec(const QString &logname)
{
  struct rd_log *log=NULL;
  unsigned log_quan=0;
  QDate date;

  edit_selected_logid=-1;
  edit_log_model->setLogName(logname);
  edit_log_view->resizeColumnsToContents();
  edit_log_view->clearSelection();
  edit_play_button->setDisabled(true);
  edit_stop_button->setDisabled(true);

  RD_ListLogs(&log,cnf->serverHostname(),cnf->serverUsername(),
	      cnf->serverPassword(),"",logname.toUtf8(),false,&log_quan);
  if(log_quan==1) {
    edit_name_label->setText(logname);
    edit_tracks_label->
      setText(QString().sprintf("%d / %d",log->log_completed_tracks,
				log->log_scheduled_tracks));
    edit_origin_label->setText(QString(log->log_origin_username)+" - "+
			       QString(DateTime::fromTm(log->log_origin_datetime).toString("MM/dd/yyyy - hh:mm:ss")));
    edit_description_edit->setText(log->log_description);
    date=DateTime::fromTm(log->log_purge_date).date();
    if(date.isValid()) {
      edit_logdelete_edit->setDate(date);
      edit_logdelete_label->setEnabled(true);
      edit_logdelete_edit->setEnabled(true);
      edit_logdelete_button->setEnabled(true);
      edit_logdelete_check->setChecked(true);
    }
    else {
      edit_logdelete_edit->setDate(QDate::currentDate());
      edit_logdelete_label->setDisabled(true);
      edit_logdelete_button->setDisabled(true);
      edit_logdelete_edit->setDisabled(true);
      edit_logdelete_check->setChecked(false);
    }
    edit_service_box->setCurrentItemData(log->log_service);
    edit_autorefresh_box->setCurrentItem(log->log_autorefresh);
    date=DateTime::fromTm(log->log_startdate).date();
    if(date.isValid()) {
      edit_startdate_edit->setDate(date);
      edit_startdate_label->setEnabled(true);
      edit_startdate_edit->setEnabled(true);
      edit_startdate_check->setChecked(true);
    }
    else {
      edit_startdate_edit->setDate(QDate::currentDate());
      edit_startdate_label->setDisabled(true);
      edit_startdate_edit->setDisabled(true);
      edit_startdate_check->setChecked(false);
    }
    date=DateTime::fromTm(log->log_enddate).date();
    if(date.isValid()) {
      edit_enddate_edit->setDate(date);
      edit_enddate_label->setEnabled(true);
      edit_enddate_edit->setEnabled(true);
      edit_enddate_check->setChecked(true);
    }
    else {
      edit_enddate_edit->setDate(QDate::currentDate());
      edit_enddate_label->setDisabled(true);
      edit_enddate_edit->setDisabled(true);
      edit_enddate_check->setChecked(false);
    }
    free(log);
  }

  return QDialog::exec();
}


void EditLogDialog::deleteDateData(bool state)
{
  edit_logdelete_label->setEnabled(state);
  edit_logdelete_edit->setEnabled(state);
  edit_logdelete_button->setEnabled(state);
}


void EditLogDialog::deleteDateSelectData()
{
  QDate date=edit_logdelete_edit->date();

  if(edit_date_dialog->exec(&date)) {
    edit_logdelete_edit->setDate(date);
  }
}


void EditLogDialog::startDateData(bool state)
{
  edit_startdate_label->setEnabled(state);
  edit_startdate_edit->setEnabled(state);
}


void EditLogDialog::endDateData(bool state)
{
  edit_enddate_label->setEnabled(state);
  edit_enddate_edit->setEnabled(state);
}


void EditLogDialog::eventClickedData(const QModelIndex &index)
{
  edit_play_button->
    setEnabled(edit_log_model->eventType(index.row())==LogModel::Cart);
  edit_stop_button->
    setEnabled(edit_log_model->eventType(index.row())==LogModel::Cart);

  if(edit_stream_player->state()==StreamPlayer::Playing) {
    if(edit_log_model->eventType(index.row())==LogModel::Cart) {
      if(edit_log_model->logId(index.row())!=edit_selected_logid) {
	edit_stream_player->
	  play(edit_log_model->cartNumber(index.row()),1,-1,-1);
	edit_selected_logid=edit_log_model->logId(index.row());
      }
    }
    else {
      edit_stream_player->stop();
    }
  }
}


void EditLogDialog::playData()
{
  QItemSelectionModel *s=edit_log_view->selectionModel();
  if(s->hasSelection()) {
    unsigned cartnum=edit_log_model->cartNumber(s->selectedRows()[0].row());
    edit_stream_player->play(cartnum,1,-1,-1);
  }
}


void EditLogDialog::stopData()
{
  edit_stream_player->stop();
}


void EditLogDialog::playerStateChangedData(StreamPlayer::State state)
{
  switch(state) {
  case StreamPlayer::Playing:
    edit_play_button->on();
    edit_stop_button->off();
    break;

  case StreamPlayer::Stopped:
    /*
    if(edit_is_closing) {
      exit(0);
    }
    */
    edit_play_button->off();
    edit_stop_button->on();
    break;
  }
}


void EditLogDialog::playerErrorData(const QString &msg)
{
  QMessageBox::information(this,"RDLogEdit - "+tr("Player Error"),msg);
}


void EditLogDialog::okData()
{
  edit_stream_player->stop();
  Save();
  done(true);
}


void EditLogDialog::cancelData()
{
  edit_stream_player->stop();
  done(false);
}


void EditLogDialog::closeEvent(QCloseEvent *e)
{
  cancelData();
}


void EditLogDialog::resizeEvent(QResizeEvent *e)
{
  edit_name_label_label->setGeometry(50,11,100,18);
  edit_name_label->setGeometry(155,11,size().width()-500,18);
  //  edit_modified_label->setGeometry(60,14,20,18);
  edit_tracks_label_label->setGeometry(size().width()-510,11,80,18);
  edit_tracks_label->setGeometry(size().width()-425,11,40,18);

  edit_origin_label_label->setGeometry(size().width()-355,11,50,18);
  edit_origin_label->setGeometry(size().width()-300,11,200,18);

  edit_description_label->setGeometry(10,40,95,20);
  edit_description_edit->setGeometry(110,40,size().width()-410,20);

  edit_logdelete_check->setGeometry(size().width()-275,42,15,15);
  edit_logdelete_label->setGeometry(size().width()-260,40,80,20);
  edit_logdelete_edit->setGeometry(size().width()-170,40,100,20);
  edit_logdelete_button->setGeometry(size().width()-60,37,50,26);

  edit_service_box->setGeometry(110,68,120,22);
  //  edit_service_edit->setGeometry(110,68,120,22);
  edit_service_label->setGeometry(10,68,95,22);

  edit_autorefresh_box->setGeometry(180,94,50,22);
  //  edit_autorefresh_edit->setGeometry(180,94,50,22);
  edit_autorefresh_label->setGeometry(10,94,165,22);

  edit_startdate_label->setGeometry(240,68,90,22);
  edit_startdate_edit->setGeometry(335,68,100,22);
  edit_startdate_check->setGeometry(250,98,15,15);
  edit_startdate_check_label->setGeometry(270,96,175,20);

  edit_enddate_label->setGeometry(435,68,90,22);
  edit_enddate_edit->setGeometry(530,68,100,22);
  edit_enddate_check->setGeometry(450,98,15,15);
  edit_enddate_check_label->setGeometry(470,96,175,20);

  edit_log_view->setGeometry(10,120,size().width()-20,size().height()-250);

  edit_play_button->setGeometry(size().width()-390,size().height()-60,80,50);
  edit_stop_button->setGeometry(size().width()-300,size().height()-60,80,50);

  edit_ok_button->setGeometry(size().width()-180,size().height()-60,80,50);
  edit_cancel_button->setGeometry(size().width()-90,size().height()-60,80,50);
}


void EditLogDialog::paintEvent(QPaintEvent *e)
{
  QColor system_mid_color = colorGroup().mid();
  QPainter *p=new QPainter(this);
  p->fillRect(60,8,size().width()-120,24,QColor(system_mid_color));
  p->fillRect(9,size().height()-130,size().width()-20,60,
	      QColor(system_mid_color));
  p->end();
  delete p;
}


void EditLogDialog::Save()
{
  struct save_loghdr_values hdr;
  int err;

  memset(&hdr,0,sizeof(hdr));
  strncpy(hdr.loghdr_service,edit_service_box->currentText().toUtf8(),64);
  strncpy(hdr.loghdr_description,edit_description_edit->text().toUtf8(),64);
  hdr.loghdr_autorefresh=edit_autorefresh_box->currentIndex();
  if((err=RD_SaveLog(&hdr,NULL,0,
		     cnf->serverHostname().toUtf8(),
		     cnf->serverUsername().toUtf8(),
		     cnf->serverPassword().toUtf8(),
		     edit_log_model->logName().toUtf8()))!=0) {
    QMessageBox::critical(this,"RDLogEdit - Error",
			  tr("Unable to save log")+
			  QString().sprintf(" [Err: %d]",err));
  }
}
