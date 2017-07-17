// runlengthwidget.h
//
// RunLengthWidget widget
//
//   (C) Copyright 2017 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License version 2 as
//   published by the Free Software Foundation.
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

#ifndef RUNLENGTHWIDGET_H
#define RUNLENGTHWIDGET_H

#include <QGroupBox>
#include <QItemSelectionModel>
#include <QLabel>
#include <QLineEdit>

#include "combobox.h"
#include "logmodel.h"

class RunLengthWidget : public QWidget
{
 Q_OBJECT;
 public:
 RunLengthWidget(LogModel *log_model,QItemSelectionModel *sel_model,
		  QWidget *parent=0);

 public slots:
  void updateSelection(const QItemSelection &new_sel,
		       const QItemSelection &old_sel);

 protected:
  void resizeEvent(QResizeEvent *e);

 private:
  QGroupBox *run_group_box;
  QLabel *run_nextstop_label;
  QLineEdit *run_nextstop_edit;
  QLabel *run_logend_label;
  QLineEdit *run_logend_edit;
  LogModel *run_log_model;
  QItemSelectionModel *run_sel_model;
};


#endif  // RUNLENGTHWIDGET_H
