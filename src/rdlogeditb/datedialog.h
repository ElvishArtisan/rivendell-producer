// datedialog.h
//
// A Dialog Box for using an RDatePicker widget.
//
//   (C) Copyright 2002-2003,2016 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU Library General Public License 
//   version 2 as published by the Free Software Foundation.
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


#ifndef DATEDIALOG_H
#define DATEDIALOG_H

#include <QDialog>
#include <QSize>
#include <QSizePolicy>
#include <QPushButton>
#include <QColor>
#include <QString>
#include <QDateTime>
#include <QLabel>

#include "datepicker.h"

class DateDialog : public QDialog
{
 Q_OBJECT
 public:
  DateDialog(int low_year,int high_year,QWidget *parent=0);
  ~DateDialog();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 public slots:
  int exec(QDate *date);

 private slots:
  void okData();
  void cancelData();

 private:
  DatePicker *date_picker;
  QDate *date_date;
};


#endif  // DATEDIALOG_H
