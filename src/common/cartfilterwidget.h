// cartfilterwidget.h
//
// Cart Filter Widget
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

#ifndef CARTFILTERWIDGET_H
#define CARTFILTERWIDGET_H

#include <vector>

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "combobox.h"

class CartFilterWidget : public QWidget
{
  Q_OBJECT
 public:
  CartFilterWidget(QWidget *parent=0);
  QSize sizeHint() const;

 signals:
  void updateRequested(const QString &filter,const QString &grp_name,
		       bool show_audio,bool show_macro);

 private slots:
  void filterChangedData(const QString &str);
  void groupChangedData(int index);
  void typeChangedData(bool state);
  void searchData();
  void clearData();

 protected:
  void resizeEvent(QResizeEvent *e);

 private:
  QLabel *main_group_label;
  ComboBox *main_group_box;
  QLabel *main_filter_label;
  QLineEdit *main_filter_edit;
  QPushButton *main_search_button;
  QPushButton *main_clear_button;
  QCheckBox *main_audio_check;
  QLabel *main_audio_label;
  QCheckBox *main_macro_check;
  QLabel *main_macro_label;
};


#endif  // CARTFILTERWIDGET_H
