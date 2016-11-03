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

#include <QApplication>
#include <QMessageBox>
#include <QStyleFactory>

#include <rivendell/rd_listgroups.h>

#include "cmdswitch.h"
#include "config.h"
#include "rdlibraryb.h"

MainWidget::MainWidget(QWidget *parent)
  :QMainWindow(parent)
{
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
  cnf=new Config();
  cnf->load();

  //
  // Window Title Bar
  //
  setWindowTitle(tr("RDLibrary Browser"));
  //  setWindowIcon(QPixmap(walltime_16x16_xpm));

  //
  // Fix the Window Size
  //
  setMinimumSize(sizeHint());

  //
  // Fonts
  //
  QFont bold_font(font().family(),font().pointSize(),QFont::Bold);

  //
  // Group Selector
  //
  main_group_label=new QLabel(tr("Group")+":",this);
  main_group_label->setFont(bold_font);
  main_group_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  main_group_box=new ComboBox(this);
  struct rd_group *groups;
  unsigned records=0;
  int err=0;
  if((err=RD_ListGroups(&groups,cnf->serverHostname().toUtf8(),
			cnf->serverUsername().toUtf8(),
			cnf->serverPassword().toUtf8(),&records))!=0) {
    QMessageBox::warning(this,tr("RDLibrary Browser - Error"),
			 tr("Unable to connect to Rivendell server")+
			 " ["+tr("Error")+QString().sprintf(" %d].",err));
    exit(256);
  }
  main_group_box->insertItem(0,tr("ALL"),tr("ALL"));
  for(unsigned i=0;i<records;i++) {
    main_group_box->
      insertItem(main_group_box->count(),groups[i].grp_name,groups[i].grp_name);
  }

  //
  // Carts List
  //
  main_library_model=new LibraryModel(this);
  main_library_view=new TableView(this);
  main_library_view->setModel(main_library_model);
  main_library_view->resizeColumnsToContents();
  connect(main_group_box,SIGNAL(activated(const QString &)),
	  main_library_model,SLOT(setGroupName(const QString &)));
}


QSize MainWidget::sizeHint() const
{
  return QSize(800,600);
}


void MainWidget::resizeEvent(QResizeEvent *e)
{
  main_group_label->setGeometry(10,10,60,20);
  main_group_box->setGeometry(75,10,100,20);

  main_library_view->setGeometry(10,32,size().width()-20,size().height()-42);
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