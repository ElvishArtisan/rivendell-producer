// editlogdialog.h
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

#ifndef EDITLOGDIALOG_H
#define EDITLOGDIALOG_H

#include <vector>

#include <QDialog>
#include <QLabel>
#include <QPushButton>

#include "logmodel.h"
#include "streamplayer.h"
#include "tableview.h"

class EditLogDialog : public QDialog
{
  Q_OBJECT
 public:
  EditLogDialog(QWidget *parent=0);
  QSize sizeHint() const;

 public slots:
  int exec(const QString &logname);

 private slots:
  void okData();
  void cancelData();

 protected:
  void closeEvent(QCloseEvent *e);
  void resizeEvent(QResizeEvent *e);

 private:
  LogModel *main_log_model;
  TableView *main_log_view;
  QPushButton *main_ok_button;  
  QPushButton *main_cancel_button;  
};


#endif  // EDITLOGDIALOG_H
