// rdlogeditb.h
//
// Remote log editor for Rivendell
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

#ifndef RDLOGEDITB_H
#define RDLOGEDITB_H

#include <vector>

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>

#include "addlogdialog.h"
#include "editlogdialog.h"
#include "loglistmodel.h"
#include "servicebox.h"
#include "streamplayer.h"
#include "tableview.h"

#define RDLOGEDITB_USAGE "\n"

class MainWidget : public QMainWindow
{
  Q_OBJECT
 public:
  MainWidget(QWidget *parent=0);
  QSize sizeHint() const;

 private slots:
  void serverLoginUpdatedData();
  void addData();
  void editData();
  void selectionChangedData(const QItemSelection &selected,
			    const QItemSelection &dselected);
  void doubleClickedData(const QModelIndex &index);
  void deleteData();
  void searchData();
  void clearData();
  void searchModifiedData(const QString &str);
  void searchModifiedData(bool state);

 protected:
  void closeEvent(QCloseEvent *e);
  void resizeEvent(QResizeEvent *e);

 private:
  QLabel *main_service_label;
  ServiceBox *main_service_box;
  QLabel *main_filter_label;
  QLineEdit *main_filter_edit;
  QPushButton *main_search_button;
  QPushButton *main_clear_button;
  QCheckBox *main_recent_check;
  QLabel *main_recent_label;
  LogListModel *main_loglist_model;
  TableView *main_loglist_view;
  AddLogDialog *main_addlog_dialog;
  EditLogDialog *main_editlog_dialog;
  QPushButton *main_add_button;  
  QPushButton *main_edit_button;  
  QPushButton *main_delete_button;  
  QPushButton *main_close_button;  
};


#endif  // RDLOGEDITB_H
