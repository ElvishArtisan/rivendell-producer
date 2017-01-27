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

#include <QCheckBox>
#include <QDateTimeEdit>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "combobox.h"
#include "datedialog.h"
#include "logmodel.h"
#include "streamplayer.h"
#include "tableview.h"
#include "transportbutton.h"

class EditLogDialog : public QDialog
{
  Q_OBJECT
 public:
  EditLogDialog(QWidget *parent=0);
  QSize sizeHint() const;

 public slots:
  int exec(const QString &logname);

 private slots:
  void deleteDateData(bool state);
  void deleteDateSelectData();
  void startDateData(bool state);
  void endDateData(bool state);
  void eventClickedData(const QModelIndex &index);
  void playData();
  void stopData();
  void playerStateChangedData(StreamPlayer::State state);
  void playerErrorData(const QString &msg);
  void okData();
  void cancelData();

 protected:
  void closeEvent(QCloseEvent *e);
  void resizeEvent(QResizeEvent *e);
  void paintEvent(QPaintEvent *e);

 private:
  DateDialog *edit_date_dialog;

  QLabel *edit_name_label_label;
  QLabel *edit_name_label;
  QLabel *edit_tracks_label_label;
  QLabel *edit_tracks_label;
  QLabel *edit_origin_label_label;
  QLabel *edit_origin_label;

  QLabel *edit_description_label;
  QLineEdit *edit_description_edit;
  QCheckBox *edit_logdelete_check;
  QLabel *edit_logdelete_label;
  QDateTimeEdit *edit_logdelete_edit;
  QPushButton *edit_logdelete_button;

  QLabel *edit_service_label;
  ComboBox *edit_service_box;
  QLabel *edit_startdate_label;
  QDateTimeEdit *edit_startdate_edit;
  QLabel *edit_enddate_label;
  QDateTimeEdit *edit_enddate_edit;

  QLabel *edit_autorefresh_label;
  QComboBox *edit_autorefresh_box;
  QCheckBox *edit_startdate_check;
  QLabel *edit_startdate_check_label;
  QCheckBox *edit_enddate_check;
  QLabel *edit_enddate_check_label;

  LogModel *edit_log_model;
  TableView *edit_log_view;
  TransportButton *edit_play_button;
  TransportButton *edit_stop_button;
  StreamPlayer *edit_stream_player;
  QPushButton *edit_ok_button;  
  QPushButton *edit_cancel_button;  
  int edit_selected_logid;
};


#endif  // EDITLOGDIALOG_H
