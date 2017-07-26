//   libtableview.cpp
//
//   A drag & drop QTableView widget.
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

#include <stdio.h>

#include <QDrag>
#include <QMimeData>

#include "config.h"
#include "librarymodel.h"
#include "libtableview.h"

LibTableView::LibTableView(QWidget *parent)
  : TableView(parent)
{
  list_move_count=-1;
}


void LibTableView::mousePressEvent(QMouseEvent *e)
{
  list_move_count=3;
  TableView::mousePressEvent(e);
}


void LibTableView::mouseMoveEvent(QMouseEvent *e)
{
  TableView::mouseMoveEvent(e);
  list_move_count--;
  if(list_move_count==0) {
    QItemSelectionModel *s=selectionModel();
    LibraryModel *m=(LibraryModel *)model();
    if(s->hasSelection()) {
      int row=s->selectedRows()[0].row();
      QDrag *d=new QDrag(this);
      QMimeData *mime=new QMimeData();
      QString str="[Rivendell-Cart]\n";
      str+=QString().sprintf("Number=%u\n",m->cartNumber(row));
      str+="Color="+
	m->data(m->index(row,2),Qt::TextColorRole).value<QColor>().name()+"\n";
      str+="ButtonText="+
	m->data(m->index(row,4),Qt::DisplayRole).toString()+"\n";
      mime->setData(RDMIMETYPE_CART,str.toUtf8());
      d->setMimeData(mime);
      d->setPixmap(m->data(s->selectedIndexes()[0],Qt::DecorationRole).
		   value<QPixmap>());
      d->exec();
    }
  }
}


void LibTableView::mouseReleaseEvent(QMouseEvent *e)
{
  list_move_count=-1;
  TableView::mouseReleaseEvent(e);
}
