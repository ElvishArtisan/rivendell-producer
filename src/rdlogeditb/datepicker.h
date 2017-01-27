// datepicker.h
//
// A Calendar Widget.
//
//   (C) Copyright 2002-2004,2016 Fred Gleason <fredg@paravelsystems.com>
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


#ifndef DATEPICKER_H
#define DATEPICKER_H

#include <QWidget>
#include <QSize>
#include <QSizePolicy>
#include <QPushButton>
#include <QColor>
#include <QDateTime>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QMouseEvent>

//
// Display Settings
//
#define DATEPICKER_X_ORIGIN 20
#define DATEPICKER_X_INTERVAL 25
#define DATEPICKER_Y_ORIGIN 30
#define DATEPICKER_Y_INTERVAL 20

class DatePicker : public QWidget
{
  Q_OBJECT
 public:
  DatePicker(int low_year,int high_year,QWidget *parent=0);
  ~DatePicker();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;
  QDate date() const;
  bool setDate(QDate date);

 private slots:
  void monthActivatedData(int id);
  void yearActivatedData(int id);
  void yearChangedData(int year);

 protected:
  void mousePressEvent(QMouseEvent *e);

 private:
  void PrintDays();
  void PrintDay(int day,int dow_offset);
  void SelectDay(int day,int dow_offset,bool state);
  QComboBox *pick_month_box;
  QComboBox *pick_year_box;
  QSpinBox *pick_year_spin;
  QLabel *pick_date_label[6][7];
  QDate pick_date;
  int pick_low_year;
  int pick_high_year;
};


#endif  // DATEPICKER_H
