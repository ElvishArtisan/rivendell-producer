// config.h
//
// Rivendell-Browser configuration
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

#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>

#define DEFAULT_CONF_FILE QString("/etc/rivendell-browser.conf")
#define DEFAULT_SERVER_HOSTNAME "localhost"
#define DEFAULT_SERVER_USERNAME "user"
#define DEFAULT_SERVER_PASSWORD ""

#define DEFAULT_AUDIO_DEVICE_NAME QString("plughw:0")

#define RD_FADE_DEPTH -3000

class Config : public QDialog
{
  Q_OBJECT;
 public:
  Config(QWidget *parent=0);
  QSize sizeHint() const;
  QString serverHostname() const;
  void setServerHostname(const QString &str);
  QString serverUsername() const;
  void setServerUsername(const QString &str);
  QString serverPassword() const;
  void setServerPassword(const QString &str);
  QString audioDeviceName() const;
  void setAudioDeviceName(const QString &str);
  bool load();
  bool save();

 private slots:
  void okData();
  void cancelData();

 protected:
  void closeEvent(QCloseEvent *e);
  void resizeEvent(QResizeEvent *e);

 private:
  QLabel *conf_hostname_label;
  QLineEdit *conf_hostname_edit;
  QLabel *conf_username_label;
  QLineEdit *conf_username_edit;
  QLabel *conf_password_label;
  QLineEdit *conf_password_edit;
  QPushButton *conf_ok_button;
  QPushButton *conf_cancel_button;
  //  QString conf_server_hostname;
  //  QString conf_server_username;
  //  QString conf_server_password;
  QString conf_audio_device_name;
};

extern Config *cnf; 

#endif  // CONFIG_H
