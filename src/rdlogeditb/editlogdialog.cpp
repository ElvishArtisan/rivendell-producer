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
  // Log Events
  //
  main_log_model=new LogModel(this);
  main_log_model->setBoldFont(bold_font);
  main_log_view=new TableView(this);
  main_log_view->setModel(main_log_model);
  main_log_view->resizeColumnsToContents();

  //
  // Ok Button
  //
  main_ok_button=new QPushButton(tr("OK"),this);
  main_ok_button->setFont(bold_font);
  connect(main_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  // Cancel Button
  //
  main_cancel_button=new QPushButton(tr("Cancel"),this);
  main_cancel_button->setFont(bold_font);
  connect(main_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));
}


QSize EditLogDialog::sizeHint() const
{
  return QSize(800,600);
}


int EditLogDialog::exec(const QString &logname)
{
  main_log_model->setLogName(logname);
  main_log_view->resizeColumnsToContents();

  return QDialog::exec();
}


void EditLogDialog::okData()
{
  done(true);
}


void EditLogDialog::cancelData()
{
  done(false);
}


void EditLogDialog::closeEvent(QCloseEvent *e)
{
  cancelData();
}


void EditLogDialog::resizeEvent(QResizeEvent *e)
{
  main_log_view->setGeometry(10,32,size().width()-20,size().height()-112);

  main_ok_button->setGeometry(size().width()-180,size().height()-60,80,50);
  main_cancel_button->setGeometry(size().width()-90,size().height()-60,80,50);
}
