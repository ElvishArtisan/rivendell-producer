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

#include <QApplication>
#include <QStyleFactory>

#include "cmdswitch.h"
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

  QCoreApplication::setOrganizationName("Paravel Systems");
  QCoreApplication::setOrganizationDomain("paravelsystems.com");
  QCoreApplication::setApplicationName("rdlibraryb");

  //
  // Window Title Bar
  //
  setWindowTitle(tr("RDLibrary Remote"));
  //  setWindowIcon(QPixmap(walltime_16x16_xpm));

  //
  // Fix the Window Size
  //
  setMinimumSize(sizeHint());
}


QSize MainWidget::sizeHint() const
{
  return QSize(150,200);
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
