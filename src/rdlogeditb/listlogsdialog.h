// listlogsdialog.h
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

#ifndef LISTLOGSDIALOG_H
#define LISTLOGSDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>

#include "loglistmodel.h"
#include "servicebox.h"
#include "tableview.h"

class ListLogsDialog : public QDialog
{
  Q_OBJECT
 public:
  ListLogsDialog(QWidget *parent=0);
  QSize sizeHint() const;

 public slots:
   int exec(QString *logname,QString *description,const QString &svcname);

 private slots:
  void doubleClickedData(const QModelIndex &index);
  void okData();
  void cancelData();

 protected:
  void closeEvent(QCloseEvent *e);
  void resizeEvent(QResizeEvent *e);

 private:
  QString *list_log_name;
  QString *list_log_description;
  QLabel *list_service_label;
  ServiceBox *list_service_box;
  LogListModel *list_loglist_model;
  TableView *list_loglist_view;
  QPushButton *list_ok_button;
  QPushButton *list_cancel_button;
};


#endif  // LISTLOGSDIALOG_H
