// pickcartdialog.h
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

#ifndef PICKCARTDIALOG_H
#define PICKCARTDIALOG_H

#include <QDialog>
#include <QPushButton>

#include "librarymodel.h"
#include "logline.h"
#include "tableview.h"

class PickCartDialog : public QDialog
{
  Q_OBJECT
 public:
  PickCartDialog(QWidget *parent=0);
  QSize sizeHint() const;

 public slots:
  int exec(LogLine *ll);

 private slots:
  void okData();
  void cancelData();

 protected:
  void closeEvent(QCloseEvent *e);
  void resizeEvent(QResizeEvent *e);

 private:
  TableView *cart_library_view;
  LibraryModel *cart_library_model;
  QPushButton *cart_ok_button;
  QPushButton *cart_cancel_button;
  LogLine *cart_logline;
};


#endif  // PICKCARTDIALOG_H
