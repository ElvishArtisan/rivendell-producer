// rdlibraryb.cpp
//
// Remote cart browser for Rivendell
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

#include <curl/curl.h>

#include <QApplication>
#include <QMessageBox>
#include <QStyleFactory>

#include <rivendell/rd_listgroups.h>

#include "cmdswitch.h"
#include "config.h"
#include "playerfactory.h"
#include "rdlibraryb.h"

#include "../../icons/rdlibraryb-16x16.xpm"

MainWidget::MainWidget(QWidget *parent)
  :QMainWindow(parent)
{
  main_selected_cart=0;
  main_is_closing=false;

  //
  // Read Command Options
  //
  CmdSwitch *cmd=new CmdSwitch("walltime-panel",VERSION,RDLIBRARYB_USAGE);
  for(unsigned i=0;i<cmd->keys();i++) {
  }
  delete cmd;

  //
  // Configuration
  //
  cnf=new Config(this);
  cnf->load();
  if(!cnf->exec()) {
    exit(0);
  }

  //
  // Initialize Curl
  //
  curl_global_init(CURL_GLOBAL_ALL);

  //
  // Stream Player
  //
  main_stream_player=PlayerFactory(cnf,this);
  connect(main_stream_player,SIGNAL(stateChanged(StreamPlayer::State)),
	  this,SLOT(playerStateChangedData(StreamPlayer::State)));
  connect(main_stream_player,SIGNAL(error(const QString &)),
	  this,SLOT(playerErrorData(const QString &)));

  //
  // Window Title Bar
  //
  setWindowTitle(tr("RDLibrary Remote")+" v"+VERSION+
		 " [Host: "+cnf->serverHostname()+
		 ",User: "+cnf->serverUsername()+"]");
  setWindowIcon(QPixmap(rdlibraryb_16x16_xpm));

  //
  // Fix the Window Size
  //
  setMinimumSize(sizeHint());

  //
  // Fonts
  //
  QFont bold_font(font().family(),font().pointSize(),QFont::Bold);

  //
  // Filter
  //
  main_filter_widget=new CartFilterWidget(this);

  //
  // Carts List
  //
  main_library_model=new LibraryModel(this);
  main_library_model->setBoldFont(bold_font);
  connect(main_library_model,SIGNAL(capiError(int,const QString &)),
	  this,SLOT(capiErrorData(int,const QString &)));
  connect(main_filter_widget,
	  SIGNAL(updateRequested(const QString &,const QString &,bool,bool)),
	  main_library_model,
	  SLOT(update(const QString &,const QString &,bool,bool)));
  main_library_view=new TableView(this);
  main_library_view->setModel(main_library_model);
  connect(main_library_view,SIGNAL(clicked(const QModelIndex &)),
	  this,SLOT(cartClickedData(const QModelIndex &)));
  main_library_model->update("",main_filter_widget->currentGroup(),true,true);
  main_library_view->resizeColumnsToContents();

  //
  // Play Button
  //
  main_play_button=new TransportButton(TransportButton::Play,this);
  connect(main_play_button,SIGNAL(clicked()),this,SLOT(playData()));

  //
  // Stop Button
  //
  main_stop_button=new TransportButton(TransportButton::Stop,this);
  main_stop_button->on();
  connect(main_stop_button,SIGNAL(clicked()),this,SLOT(stopData()));

  //
  // Close Button
  //
  main_close_button=new QPushButton(tr("Close"),this);
  main_close_button->setFont(bold_font);
  connect(main_close_button,SIGNAL(clicked()),this,SLOT(close()));
}


QSize MainWidget::sizeHint() const
{
  return QSize(800,600);
}


void MainWidget::cartClickedData(const QModelIndex &index)
{
#ifdef ALSA
  if(main_stream_player->state()==StreamPlayer::Playing) {
    main_stream_player->stop();
  }
#else
  if((main_stream_player->state()==StreamPlayer::Playing)&&
     (main_library_model->cartNumber(index.row())!=main_selected_cart)) {
    main_stream_player->
      play(main_library_model->cartNumber(index.row()),1,-1,-1);
    main_selected_cart=main_library_model->cartNumber(index.row());
  }
#endif  // ALSA
}


void MainWidget::playData()
{
  QItemSelectionModel *s=main_library_view->selectionModel();
  if(s->hasSelection()) {
    unsigned cartnum=main_library_model->cartNumber(s->selectedRows()[0].row());
    main_stream_player->play(cartnum,1,-1,-1);
  }
}


void MainWidget::stopData()
{
  main_stream_player->stop();
}


void MainWidget::playerStateChangedData(StreamPlayer::State state)
{
  switch(state) {
  case StreamPlayer::Playing:
    main_play_button->on();
    main_stop_button->off();
    break;

  case StreamPlayer::Stopped:
    if(main_is_closing) {
      exit(0);
    }
    main_play_button->off();
    main_stop_button->on();
    break;
  }

}


void MainWidget::playerErrorData(const QString &msg)
{
  QMessageBox::information(this,"RDLibrary - "+tr("Player Error"),msg);
}


void MainWidget::capiErrorData(int err,const QString &err_msg)
{
  QMessageBox::warning(this,tr("RDLibrary - C API Error"),
		       err_msg+QString().sprintf(" [Error: %d]",err));
}


void MainWidget::closeEvent(QCloseEvent *e)
{
  if(main_stream_player->state()==StreamPlayer::Playing) {
    main_stream_player->stop();
    main_is_closing=true;
    e->ignore();
  }
}


void MainWidget::resizeEvent(QResizeEvent *e)
{
  main_filter_widget->setGeometry(10,10,size().width()-20,84);

  main_library_view->setGeometry(10,90,size().width()-20,size().height()-165);

  main_play_button->setGeometry(10,size().height()-60,80,50);
  main_stop_button->setGeometry(100,size().height()-60,80,50);
  main_close_button->setGeometry(size().width()-90,size().height()-60,80,50);
}


int main(int argc,char *argv[])
{
  QApplication a(argc,argv);
  a.setStyle(QStyleFactory::create("Windows"));

  //
  // Start Event Loop
  //
  MainWidget *w=new MainWidget();
  w->show();
  return a.exec();
}
