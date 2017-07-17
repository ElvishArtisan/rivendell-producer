// runlengthwidget.cpp
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

#include <QDateTime>

#include "runlengthwidget.h"

RunLengthWidget::RunLengthWidget(LogModel *log_model,
				 QItemSelectionModel *sel_model,QWidget *parent)
  : QWidget(parent)
{
  run_log_model=log_model;
  run_sel_model=sel_model;

  QFont edit_font(font().family(),font().pointSize(),QFont::Normal);

  run_group_box=new QGroupBox(this);
  run_group_box->setTitle(tr("Run Length"));

  run_nextstop_label=new QLabel(tr("Next Stop")+":",run_group_box);
  run_nextstop_label->setAlignment(Qt::AlignRight|Qt::AlignCenter);

  run_nextstop_edit=new QLineEdit(run_group_box);
  run_nextstop_edit->setFont(edit_font);
  run_nextstop_edit->setReadOnly(true);

  run_logend_label=new QLabel(tr("Log End")+":",run_group_box);
  run_logend_label->setAlignment(Qt::AlignRight|Qt::AlignCenter);

  run_logend_edit=new QLineEdit(run_group_box);
  run_logend_edit->setFont(edit_font);
  run_logend_edit->setReadOnly(true);
}


void RunLengthWidget::updateSelection(const QItemSelection &new_sel,
				      const QItemSelection &old_sel)
{
  int len=0;

  if(run_sel_model->selectedRows().size()==1) {
    run_nextstop_label->setText(tr("Next Stop")+":");
    run_nextstop_edit->clear();
    if((len=run_log_model->lengthToStop(run_sel_model->selectedRows()[0].row()))<0) {
      run_nextstop_edit->clear();
    }
    else {
      run_nextstop_edit->setText(QTime().addMSecs(len).toString("h:mm:ss"));
    }

    if((len=run_log_model->length(run_sel_model->selectedRows()[0].row(),run_log_model->rowCount()-1))==0) {
      run_logend_edit->clear();
    }
    else {
      run_logend_edit->setText(QTime().addMSecs(len).toString("h:mm:ss"));
    }
  }
  else {
    run_nextstop_label->setText(tr("Selected")+":");
    run_logend_edit->clear();
    len=run_log_model->length(run_sel_model->selectedRows()[0].row(),run_sel_model->selectedRows()[run_sel_model->selectedRows().size()-1].row()+1);
    run_nextstop_edit->setText(QTime().addMSecs(len).toString("h:mm:ss"));
  }
}


void RunLengthWidget::resizeEvent(QResizeEvent *e)
{
  run_nextstop_label->setGeometry(5,20,80,20);
  run_nextstop_edit->setGeometry(90,20,65,20);

  run_logend_label->setGeometry(5,42,80,20);
  run_logend_edit->setGeometry(90,42,65,20);
}
