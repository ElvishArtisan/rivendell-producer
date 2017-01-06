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

#include <curl/curl.h>

#include <QApplication>
#include <QMessageBox>
#include <QStyleFactory>

#include <rivendell/rd_listservices.h>

#include "cmdswitch.h"
#include "config.h"
#include "playerfactory.h"
#include "rdlogeditb.h"

#include "../../icons/rdlogedit-16x16.xpm"

MainWidget::MainWidget(QWidget *parent)
  :QMainWindow(parent)
{
  //
  // Read Command Options
  //
  CmdSwitch *cmd=new CmdSwitch("walltime-panel",VERSION,RDLOGEDITB_USAGE);
  for(unsigned i=0;i<cmd->keys();i++) {
  }
  delete cmd;

  //
  // Configuration
  //
  cnf=new Config();
  cnf->load();

  //
  // Initialize Curl
  //
  curl_global_init(CURL_GLOBAL_ALL);

  //
  // Window Title Bar
  //
  setWindowTitle(tr("RDLogEdit Browser"));
  setWindowIcon(QPixmap(rdlogedit_16x16_xpm));

  //
  // Fix the Window Size
  //
  setMinimumSize(sizeHint());

  //
  // Fonts
  //
  QFont bold_font(font().family(),font().pointSize(),QFont::Bold);

  //
  // Service Selector
  //
  main_service_label=new QLabel(tr("Service")+":",this);
  main_service_label->setFont(bold_font);
  main_service_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  main_service_box=new ComboBox(this);
  struct rd_service *services;
  unsigned records=0;
  int err=0;
  if((err=RD_ListServices(&services,cnf->serverHostname().toUtf8(),
			  cnf->serverUsername().toUtf8(),
			  cnf->serverPassword().toUtf8(),false,&records))!=0) {
    QMessageBox::warning(this,tr("RDLogEdit Browser - Error"),
			 tr("Unable to connect to Rivendell server")+
			 " ["+tr("Error")+QString().sprintf(" %d].",err));
    exit(256);
  }
  main_service_box->insertItem(0,tr("ALL"),tr("ALL"));
  for(unsigned i=0;i<records;i++) {
    main_service_box->
      insertItem(main_service_box->count(),services[i].service_name,services[i].service_name);
  }

  //
  // Carts List
  //
  main_loglist_model=new LogListModel(this);
  main_loglist_view=new TableView(this);
  main_loglist_view->setModel(main_loglist_model);
  main_loglist_view->resizeColumnsToContents();
  connect(main_service_box,SIGNAL(activated(const QString &)),
	  main_loglist_model,SLOT(setServiceName(const QString &)));

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


void MainWidget::closeEvent(QCloseEvent *e)
{
  exit(0);
}


void MainWidget::resizeEvent(QResizeEvent *e)
{
  main_service_label->setGeometry(10,5,60,20);
  main_service_box->setGeometry(75,5,100,20);

  main_loglist_view->setGeometry(10,32,size().width()-20,size().height()-112);

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
