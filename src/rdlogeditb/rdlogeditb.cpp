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

#include <QDir>

#include <QApplication>
#include <QMessageBox>
#include <QSplashScreen>
#include <QStyleFactory>

#include "cmdswitch.h"
#include "config.h"
#include "playerfactory.h"
#include "rdlogeditb.h"

#include "../../icons/rdlogeditb-16x16.xpm"

MainWidget::MainWidget(QWidget *parent)
  :QMainWindow(parent)
{
  int err;

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
#ifdef MME
  cnf=new Config("rdlogeditb",":/rdlogeditb-splashscreen.bmp",this);
#endif  // MME
#ifdef ALSA
  cnf=new Config("rdlogeditb",
		 "/usr/share/rivendell-producer/rdlogeditb-splashscreen.png",
		 this);
#endif  // ALSA
  cnf->load();

  //
  // Initialize Curl
  //
  curl_global_init(CURL_GLOBAL_ALL);

  //
  // Window Title Bar
  //
  setWindowTitle(tr("RDLogEdit Remote")+" v"+VERSION+
		 " [Host: "+cnf->serverHostname()+
		 ",User: "+cnf->serverUsername()+"]");
  setWindowIcon(QPixmap(rdlogeditb_16x16_xpm));

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
  main_addlog_dialog=new AddLogDialog(this);
  main_editlog_dialog=new EditLogDialog(main_addlog_dialog,this);

  //
  // Service Selector
  //
  main_service_label=new QLabel(tr("Service")+":",this);
  main_service_label->setFont(bold_font);
  main_service_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  main_service_box=new ServiceBox(true,this);
  if((err=main_service_box->reload())!=0) {
    QMessageBox::warning(this,tr("RDLogEdit - Error"),
			 tr("Error in rd_listservices() call")+
			 " ["+tr("Error")+QString().sprintf(" %d].",err));
    exit(256);
  }

  //
  // Filter
  //
  main_filter_label=new QLabel(tr("Filter"),this);
  main_filter_label->setFont(bold_font);
  main_filter_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  main_filter_edit=new QLineEdit(this);
  connect(main_filter_edit,SIGNAL(returnPressed()),this,SLOT(searchData()));
  connect(main_filter_edit,SIGNAL(textChanged(const QString &)),
	  this,SLOT(searchModifiedData(const QString &)));
  main_search_button=new QPushButton(tr("Search"),this);
  main_search_button->setFont(bold_font);
  main_search_button->setDisabled(true);
  connect(main_search_button,SIGNAL(clicked()),this,SLOT(searchData()));
  main_clear_button=new QPushButton(tr("Clear"),this);
  main_clear_button->setFont(bold_font);
  main_clear_button->setDisabled(true);
  connect(main_clear_button,SIGNAL(clicked()),this,SLOT(clearData()));
  main_recent_check=new QCheckBox(this);
  connect(main_recent_check,SIGNAL(toggled(bool)),
	  this,SLOT(searchModifiedData(bool)));
  main_recent_label=new QLabel(tr("Show Only Recent Logs"),this);
  main_recent_label->setFont(bold_font);
  main_recent_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  //
  // Log List
  //
  main_loglist_model=new LogListModel(this);
  main_loglist_view=new TableView(this);
  main_loglist_view->setSelectionMode(QAbstractItemView::ExtendedSelection);
  main_loglist_view->setModel(main_loglist_model);
  main_loglist_view->resizeColumnsToContents();
  connect(main_loglist_view->selectionModel(),
	  SIGNAL(selectionChanged(const QItemSelection &,
				  const QItemSelection &)),
	  this,
	  SLOT(selectionChangedData(const QItemSelection &,
				    const QItemSelection &)));
  connect(main_loglist_view,SIGNAL(doubleClicked(const QModelIndex &)),
	  this,SLOT(doubleClickedData(const QModelIndex &)));
  connect(main_service_box,SIGNAL(activated(const QString &)),
	  this,SLOT(searchModifiedData(const QString &)));

  //
  // Add Button
  //
  main_add_button=new QPushButton(tr("Add"),this);
  main_add_button->setFont(bold_font);
  connect(main_add_button,SIGNAL(clicked()),this,SLOT(addData()));

  //
  // Edit Button
  //
  main_edit_button=new QPushButton(tr("Edit"),this);
  main_edit_button->setFont(bold_font);
  main_edit_button->setDisabled(true);
  connect(main_edit_button,SIGNAL(clicked()),this,SLOT(editData()));

  //
  // Delete Button
  //
  main_delete_button=new QPushButton(tr("Delete"),this);
  main_delete_button->setFont(bold_font);
  main_delete_button->setDisabled(true);
  connect(main_delete_button,SIGNAL(clicked()),this,SLOT(deleteData()));

  //
  // Close Button
  //
  main_close_button=new QPushButton(tr("Close"),this);
  main_close_button->setFont(bold_font);
  connect(main_close_button,SIGNAL(clicked()),this,SLOT(close()));

  cnf->splashScreen()->finish(this);
}


QSize MainWidget::sizeHint() const
{
  return QSize(800,600);
}


void MainWidget::addData()
{
  QString svcname=main_service_box->currentText();
  QString logname;
  int err;

  if(main_addlog_dialog->exec(&svcname,&logname)) {
    if((err=cnf->addLog(logname,svcname))!=0) {
      QMessageBox::critical(this,"RDLogEdit - Add Log",
			    tr("Unable to add new log")+" ["+tr("error code")+
			    QString().sprintf(": %d]",err));
      return;
    }
    if(main_editlog_dialog->exec(logname)) {
      main_loglist_model->
	update(main_filter_edit->text(),main_recent_check->isChecked());
      QModelIndex index=main_loglist_model->index(logname);
      main_loglist_view->selectionModel()->
	select(main_loglist_model->index(logname),QItemSelectionModel::Rows|
	       QItemSelectionModel::ClearAndSelect);
    }
    else {
      cnf->deleteLog(logname);
    }
  }
}


void MainWidget::editData()
{
  QItemSelectionModel *s=main_loglist_view->selectionModel();
  if(s->hasSelection()) {
    QString logname=main_loglist_model->logName(s->selectedRows()[0].row());
    main_editlog_dialog->exec(logname);
    main_loglist_model->
      update(main_filter_edit->text(),main_recent_check->isChecked());
  }
}


void MainWidget::selectionChangedData(const QItemSelection &selected,
				      const QItemSelection &dselected)
{
  QItemSelectionModel *s=main_loglist_view->selectionModel();
  main_edit_button->setEnabled(s->selectedRows().size()==1);
  main_delete_button->setEnabled(s->selectedRows().size()>0);
}


void MainWidget::doubleClickedData(const QModelIndex &index)
{
  editData();
}


void MainWidget::deleteData()
{
  int err;
  QItemSelectionModel *s=main_loglist_view->selectionModel();
  if(s->selectedRows().size()==1) {
    QString logname=main_loglist_model->logName(s->selectedRows()[0].row());
    if(QMessageBox::question(this,"RDLogEdit - Delete Log",
			     tr("Are you sure you want to delete the")+
			     " \""+logname+"\" "+tr("log?"),
			     QMessageBox::Yes,QMessageBox::No)!=QMessageBox::Yes) {
      return;
    }
  }
  else {
    if(QMessageBox::question(this,"RDLogEdit - Delete Log",
			     tr("Are you sure you want to delete these logs?"),
			     QMessageBox::Yes,QMessageBox::No)!=QMessageBox::Yes) {
      return;
    }
  }
  QModelIndexList indices=s->selectedRows();
  for(int i=0;i<indices.size();i++) {
    QString logname=main_loglist_model->logName(indices.at(i).row());
    if((err=cnf->deleteLog(logname))!=0) {
      QMessageBox::critical(this,"RDLogEdit - Delete Log",
			    tr("Unable to delete log")+"\""+logname+
			    "\" ["+tr("error code")+
			    QString().sprintf(": %d",err));
    }
  }
  main_loglist_model->
    update(main_filter_edit->text(),main_recent_check->isChecked());
}


void MainWidget::searchData()
{
  QString service_name=main_service_box->currentText();
  if(service_name==tr("ALL")) {
    service_name="";
  }
  main_loglist_model->setServiceName(service_name);
  main_loglist_model->
    update(main_filter_edit->text(),main_recent_check->isChecked());
  main_search_button->setDisabled(true);
}


void MainWidget::clearData()
{
  main_filter_edit->clear();
  searchModifiedData("");
}


void MainWidget::searchModifiedData(const QString &str)
{
  searchModifiedData(false);
  main_clear_button->setDisabled(str.isEmpty());
}


void MainWidget::searchModifiedData(bool state)
{
  main_search_button->setEnabled(true);
}


void MainWidget::closeEvent(QCloseEvent *e)
{
  exit(0);
}


void MainWidget::resizeEvent(QResizeEvent *e)
{
  main_service_label->setGeometry(10,7,60,20);
  main_service_box->setGeometry(75,7,120,20);

  main_filter_label->setGeometry(210,7,60,20);
  main_filter_edit->setGeometry(275,7,size().width()-450,20);
  main_search_button->setGeometry(size().width()-140,5,60,25);
  main_clear_button->setGeometry(size().width()-70,5,60,25);
  main_recent_check->setGeometry(275,31,15,15);
  main_recent_label->setGeometry(295,29,size().width()-305,20);

  main_loglist_view->setGeometry(10,54,size().width()-20,size().height()-134);

  main_add_button->setGeometry(10,size().height()-60,80,50);
  main_edit_button->setGeometry(100,size().height()-60,80,50);
  main_delete_button->setGeometry(190,size().height()-60,80,50);

  main_close_button->setGeometry(size().width()-90,size().height()-60,80,50);
}


int main(int argc,char *argv[])
{
  QApplication a(argc,argv);
  a.setStyle(QStyleFactory::create("Windows"));
  /*
  QPixmap pix("/usr/share/rivendell-producer/rdlogeditb-splashscreen.png");
  QSplashScreen *splash=new QSplashScreen(pix);
  splash->show();
  splash->showMessage("Contacting server...");
  a.processEvents(QEventLoop::AllEvents);
  */

  //
  // Start Event Loop
  //
  MainWidget *w=new MainWidget();
  w->show();
  return a.exec();
}
