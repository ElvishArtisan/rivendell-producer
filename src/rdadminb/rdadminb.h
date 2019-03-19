// rdadminb.h
//
// Configuration applet for rivendell-producer
//
//   (C) Copyright 2016-2019 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef RDADMINB_H
#define RDADMINB_H

#include <vector>

#include <QCheckBox>
#include <QLabel>
#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>

#include "combobox.h"
#include "config.h"

#define RDADMINB_USAGE "\n"

class MainWidget : public QMainWindow
{
  Q_OBJECT
 public:
  MainWidget(QWidget *parent=0);
  QSize sizeHint() const;

 private slots:
  void okData();
  void cancelData();

 protected:
  void closeEvent(QCloseEvent *e);
  void resizeEvent(QResizeEvent *e);

 private:
  QLabel *admin_server_hostname_label;
  QLineEdit *admin_server_hostname_edit;
  QLabel *admin_server_username_label;
  QLineEdit *admin_server_username_edit;
  QLabel *admin_server_password_label;
  QLineEdit *admin_server_password_edit;
  QLabel *admin_audio_devicename_label;
  ComboBox *admin_audio_devicename_box;
  QLineEdit *admin_audio_devicename_edit;
  QCheckBox *admin_ui_allgroup_check;
  QLabel *admin_ui_allgroup_label;

  QPushButton *admin_ok_button;
  QPushButton *admin_cancel_button;
  Config *admin_config;
};


#endif  // RDADMINB_H
