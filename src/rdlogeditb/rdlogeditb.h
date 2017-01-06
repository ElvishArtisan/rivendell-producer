// rdlogeditb.h
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

#ifndef RDLOGEDITB_H
#define RDLOGEDITB_H

#include <vector>

#include <QLabel>
#include <QMainWindow>
#include <QPushButton>

#include "combobox.h"
#include "loglistmodel.h"
#include "streamplayer.h"
#include "tableview.h"

#define RDLOGEDITB_USAGE "\n"

class MainWidget : public QMainWindow
{
  Q_OBJECT
 public:
  MainWidget(QWidget *parent=0);
  QSize sizeHint() const;

 protected:
  void closeEvent(QCloseEvent *e);
  void resizeEvent(QResizeEvent *e);

 private:
  QLabel *main_service_label;
  ComboBox *main_service_box;
  LogListModel *main_loglist_model;
  TableView *main_loglist_view;
  QPushButton *main_close_button;  
};


#endif  // RDLOGEDITB_H
