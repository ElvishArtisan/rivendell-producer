// insertmetadialog.h
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

#ifndef INSERTMETADIALOG_H
#define INSERTMETADIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>

class InsertMetaDialog : public QDialog
{
  Q_OBJECT
 public:
  InsertMetaDialog(QWidget *parent=0);
  QSize sizeHint() const;

 private slots:
  void clickedData(int id);

 protected:
  void closeEvent(QCloseEvent *e);
  void resizeEvent(QResizeEvent *e);

 private:
  QLabel *insert_label;
  QPushButton *insert_marker_button;
  QPushButton *insert_track_button;
  QPushButton *insert_chain_button;
  QPushButton *insert_cancel_button;
};


#endif  // INSERTMETADIALOG_H
