// pickcartdialog.cpp
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

#include "pickcartdialog.h"

PickCartDialog::PickCartDialog(QWidget *parent)
  : QDialog(parent)
{
  setWindowTitle(tr("RDLogEdit - Pick Cart"));

  QFont bold_font(font().family(),font().pointSize(),QFont::Bold);

  cart_filter_widget=new CartFilterWidget(this);

  cart_library_model=new LibraryModel(this);
  connect(cart_filter_widget,
	  SIGNAL(updateRequested(const QString &,const QString &,bool,bool)),
	  cart_library_model,
	  SLOT(update(const QString &,const QString &,bool,bool)));
  cart_library_view=new TableView(this);
  cart_library_view->setModel(cart_library_model);
  cart_library_view->resizeColumnsToContents();
  connect(cart_library_view,SIGNAL(doubleClicked(const QModelIndex &)),
	  this,SLOT(doubleClickedData(const QModelIndex)));

  cart_ok_button=new QPushButton(tr("OK"),this);
  cart_ok_button->setFont(bold_font);
  connect(cart_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  cart_cancel_button=new QPushButton(tr("Cancel"),this);
  cart_cancel_button->setFont(bold_font);
  connect(cart_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));
}


QSize PickCartDialog::sizeHint() const
{
  return QSize(800,600);
}


int PickCartDialog::exec(LogLine *ll)
{
  cart_logline=ll;
  cart_library_model->update("",tr("ALL"),true,true);
  cart_library_view->resizeColumnsToContents();
  return QDialog::exec();
}


void PickCartDialog::doubleClickedData(const QModelIndex &index)
{
  okData();
}


void PickCartDialog::okData()
{
  QItemSelectionModel *s=cart_library_view->selectionModel();
  if(s->hasSelection()) {
    cart_library_model->getLogLine(cart_logline,s->selectedRows()[0].row());
    done(true);
  }
}


void PickCartDialog::cancelData()
{
  done(false);
}


void PickCartDialog::closeEvent(QCloseEvent *e)
{
  cancelData();
}


void PickCartDialog::resizeEvent(QResizeEvent *e)
{
  cart_filter_widget->setGeometry(10,10,size().width()-20,84);

  cart_library_view->setGeometry(10,90,size().width()-20,size().height()-165);

  cart_ok_button->setGeometry(size().width()-180,size().height()-60,80,50);
  cart_cancel_button->setGeometry(size().width()-90,size().height()-60,80,50);
}
