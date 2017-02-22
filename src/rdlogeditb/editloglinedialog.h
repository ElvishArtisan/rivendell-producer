// editloglinedialog.h
//
// Log Event Editing Dialog
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

#ifndef EDITLOGLINEDIALOG_H
#define EDITLOGLINEDIALOG_H

#include <QButtonGroup>
#include <QCheckBox>
#include <QDateTimeEdit>
#include <QDialog>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include <QPushButton>
#include <QRadioButton>

#include "combobox.h"
#include "logline.h"
#include "pickcartdialog.h"

class EditLogLineDialog : public QDialog
{
  Q_OBJECT
 public:
  EditLogLineDialog(QWidget *parent=0);
  ~EditLogLineDialog();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 public slots:
   int exec(LogLine *ll,const QString &service,const QList<QTime> &start_times);

 private slots:
  void selectCartData();
  void timeChangedData(const QTime &);
  void timeToggledData(bool state);
  void graceClickedData(int id);
  void okData();
  void cancelData();

 protected:
  void closeEvent(QCloseEvent *e);
  
 private:
  PickCartDialog *edit_cart_dialog;
  QDateTimeEdit *edit_time_edit;
  QLabel *edit_time_label;
  QCheckBox *edit_timetype_box;
  QLabel *edit_timetype_label;
  ComboBox *edit_transtype_box;
  QCheckBox *edit_overlap_box;
  QLabel *edit_overlap_label;

  QLabel *edit_cart_label;
  QLineEdit *edit_cart_edit;
  QLabel *edit_title_label;
  QLineEdit *edit_title_edit;
  QLabel *edit_artist_label;
  QLineEdit *edit_artist_edit;
  QPushButton *edit_cart_button;

  QLabel *edit_comment_label;
  QLineEdit *edit_comment_edit;
  QLabel *edit_label_label;
  QLineEdit *edit_label_edit;

  QLabel *edit_logname_label;
  QLineEdit *edit_logname_edit;
  QLabel *edit_logcomment_label;
  QLineEdit *edit_logcomment_edit;
  QPushButton *edit_selectlog_button;

  QGroupBox *edit_grace_box;
  QButtonGroup *edit_grace_group;
  QDateTimeEdit *edit_grace_edit;
  QString edit_service;
  LogLine *edit_logline;
  LogLine edit_new_logline;
  QPushButton *edit_ok_button;
  QPushButton *edit_cancel_button;
  QList<QTime> edit_start_times;
};


#endif  // EDITLOGLINEDIALOG_H
