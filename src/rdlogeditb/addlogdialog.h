// addlogdialog.h
//
// Log Editing Dialog
//
//   (C) Copyright 2017-2019 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef ADDLOGDIALOG_H
#define ADDLOGDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "servicebox.h"

class AddLogDialog : public QDialog
{
  Q_OBJECT
 public:
  AddLogDialog(QWidget *parent=0);
  QSize sizeHint() const;

 public slots:
  int exec(QString *svcname,QString *logname);

 private slots:
  void logNameChanged(const QString &str);
  void okData();
  void cancelData();

 protected:
  void closeEvent(QCloseEvent *e);
  void resizeEvent(QResizeEvent *e);

 private:
  QLabel *edit_servicename_label;
  ServiceBox *edit_servicename_box;
  QLabel *edit_logname_label;
  QLineEdit *edit_logname_edit;
  QPushButton *edit_ok_button;
  QPushButton *edit_cancel_button;
  QString *edit_service_name;
  QString *edit_log_name;
};


#endif  // ADDLOGDIALOG_H
