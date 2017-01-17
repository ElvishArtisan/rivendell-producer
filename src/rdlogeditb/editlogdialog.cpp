// rdlogeditb.cpp
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

#include "cmdswitch.h"
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
  // Stream Player
  //
  edit_stream_player=PlayerFactory(cnf,this);
  connect(edit_stream_player,SIGNAL(stateChanged(StreamPlayer::State)),
	  this,SLOT(playerStateChangedData(StreamPlayer::State)));
  connect(edit_stream_player,SIGNAL(error(const QString &)),
	  this,SLOT(playerErrorData(const QString &)));

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
  edit_selected_logid=-1;
  edit_log_model->setLogName(logname);
  edit_log_view->resizeColumnsToContents();
  edit_log_view->clearSelection();
  edit_play_button->setDisabled(true);
  edit_stop_button->setDisabled(true);

  return QDialog::exec();
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
  edit_log_view->setGeometry(10,32,size().width()-20,size().height()-112);

  edit_play_button->setGeometry(size().width()-390,size().height()-60,80,50);
  edit_stop_button->setGeometry(size().width()-300,size().height()-60,80,50);

  edit_ok_button->setGeometry(size().width()-180,size().height()-60,80,50);
  edit_cancel_button->setGeometry(size().width()-90,size().height()-60,80,50);
}
