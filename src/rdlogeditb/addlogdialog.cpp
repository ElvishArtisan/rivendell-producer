// addlogdialog.cpp
//
// Log Editing Dialog
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

#include <rivendell/rd_listservices.h>

#include <QMessageBox>

#include "addlogdialog.h"
#include "config.h"

AddLogDialog::AddLogDialog(QWidget *parent)
  : QDialog(parent)
{
  setWindowTitle(tr("RDLogEdit - Add Log"));
  setMinimumSize(sizeHint());
  setMaximumHeight(sizeHint().height());

  edit_servicename_label=new QLabel(tr("Service Name")+":",this);
  edit_servicename_label->setAlignment(Qt::AlignRight);
  edit_servicename_box=new ComboBox(this);

  edit_logname_label=new QLabel(tr("Log Name")+":",this);
  edit_logname_label->setAlignment(Qt::AlignRight);
  edit_logname_edit=new QLineEdit(this);
  edit_logname_edit->setMaxLength(64);

  edit_ok_button=new QPushButton(tr("OK"),this);
  connect(edit_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  edit_cancel_button=new QPushButton(tr("Cancel"),this);
  connect(edit_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));
}


QSize AddLogDialog::sizeHint() const
{
  return QSize(400,130);
}



int AddLogDialog::exec(QString *svcname,QString *logname)
{
  struct rd_service *svc=NULL;
  unsigned svc_quan=0;

  edit_service_name=svcname;
  edit_log_name=logname;

  if(RD_ListServices(&svc,cnf->serverHostname().toUtf8(),
		     cnf->serverUsername().toUtf8(),
		     cnf->serverPassword().toUtf8(),
		     0,&svc_quan)!=0) {
    QMessageBox::critical(this,"RDLogEdit - Add Log",
			   tr("Unable to get Services list."));
  }
  edit_servicename_box->clear();
  for(unsigned i=0;i<svc_quan;i++) {
    edit_servicename_box->insertItem(-1,svc[i].service_name);
  }
  edit_servicename_box->setCurrentItemData(*svcname);
  edit_logname_edit->setText(*logname);

  return QDialog::exec();
}


void AddLogDialog::okData()
{
  *edit_service_name=edit_servicename_box->currentText();
  *edit_log_name=edit_logname_edit->text();

  done(true);
}


void AddLogDialog::cancelData()
{
  done(false);
}


void AddLogDialog::closeEvent(QCloseEvent *e)
{
  cancelData();
}


void AddLogDialog::resizeEvent(QResizeEvent *e)
{
  edit_logname_label->setGeometry(10,10,110,20);
  edit_logname_edit->setGeometry(125,10,size().width()-135,20);

  edit_servicename_label->setGeometry(10,32,110,20);
  edit_servicename_box->setGeometry(125,32,120,20);

  edit_ok_button->setGeometry(size().width()-180,size().height()-60,80,50);
  edit_cancel_button->setGeometry(size().width()-90,size().height()-60,80,50);
}
