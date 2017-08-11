// listlogsdialog.cpp
//
// Log Listing Dialog
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

#include <QMessageBox>

#include "listlogsdialog.h"

ListLogsDialog::ListLogsDialog(QWidget *parent)
  : QDialog(parent)
{
  setWindowTitle(tr("RDLogEdit - List Logs"));
  setMinimumSize(sizeHint());

  //
  // Fonts
  //
  QFont bold_font(font().family(),font().pointSize(),QFont::Bold);

  //
  // Service
  //
  list_service_box=new ServiceBox(true,this);
  list_service_label=new QLabel(tr("Service")+":",this);
  list_service_label->setFont(bold_font);
  list_service_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

  //
  // Log List
  //
  list_loglist_model=new LogListModel(this);
  connect(list_loglist_model,SIGNAL(capiError(int,const QString &)),
	  this,SLOT(capiErrorData(int,const QString &)));
  list_loglist_view=new TableView(this);
  list_loglist_view->setModel(list_loglist_model);
  list_loglist_view->resizeColumnsToContents();
  connect(list_loglist_view,SIGNAL(doubleClicked(const QModelIndex &)),
	  this,SLOT(doubleClickedData(const QModelIndex &)));
  connect(list_service_box,SIGNAL(activated(const QString &)),
  	  list_loglist_model,SLOT(setServiceName(const QString &)));

  //
  // OK Button
  //
  list_ok_button=new QPushButton(tr("OK"),this);
  list_ok_button->setFont(bold_font);
  connect(list_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  // Cancel Button
  //
  list_cancel_button=new QPushButton(tr("Cancel"),this);
  list_cancel_button->setFont(bold_font);
  connect(list_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));
}


QSize ListLogsDialog::sizeHint() const
{
  return QSize(400,300);
}


int ListLogsDialog::exec(QString *logname,QString *description,
			 const QString &svcname)
{
  int err;

  list_log_name=logname;
  list_log_description=description;

  if((err=list_service_box->reload())!=0) {
    QMessageBox::warning(this,tr("RDLogEdit - Error"),
			 tr("Error in RD_ListServices() call")+
			 " ["+tr("Error")+QString().sprintf(" %d].",err));
  }
  list_service_box->setCurrentItemData(svcname);
  list_loglist_model->setServiceName(svcname);

  return QDialog::exec();
}


void ListLogsDialog::doubleClickedData(const QModelIndex &index)
{
  okData();
}

void ListLogsDialog::okData()
{
  QItemSelectionModel *s=list_loglist_view->selectionModel();
  if(s->hasSelection()) {
    *list_log_name=list_loglist_model->logName(s->selectedRows().at(0).row());
    *list_log_description=
      list_loglist_model->description(s->selectedRows().at(0).row());
    done(true);
  }
}


void ListLogsDialog::cancelData()
{
  done(false);
}


void ListLogsDialog::capiErrorData(int err,const QString &err_msg)
{
  QMessageBox::warning(this,tr("RDLogEdit - C API Error"),
		       err_msg+QString().sprintf(" [Error: %d]",err));
}


void ListLogsDialog::closeEvent(QCloseEvent *e)
{
  cancelData();
}


void ListLogsDialog::resizeEvent(QResizeEvent *e)
{
  list_service_label->setGeometry(10,10,80,20);
  list_service_box->setGeometry(95,10,120,20);

  list_loglist_view->setGeometry(10,35,size().width()-20,size().height()-105);

  list_ok_button->setGeometry(size().width()-180,size().height()-60,80,50);

  list_cancel_button->setGeometry(size().width()-90,size().height()-60,80,50);
}
