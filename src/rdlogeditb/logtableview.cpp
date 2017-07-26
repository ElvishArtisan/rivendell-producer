// logtableview.cpp
//
// TableView class for logs.
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

#include <QDragEnterEvent>

#include "config.h"
#include "logmodel.h"
#include "logtableview.h"
#include "profile.h"

LogTableView::LogTableView(QWidget *parent)
  : TableView(parent)
{
  setAcceptDrops(true);
}


void LogTableView::dragEnterEvent(QDragEnterEvent *e)
{
  QStringList fmts=e->mimeData()->formats();
  if(e->mimeData()->hasFormat(RDMIMETYPE_CART)) {
    e->acceptProposedAction();
  }
}


void LogTableView::dragMoveEvent(QDragMoveEvent *e)
{
  e->acceptProposedAction();
}


void LogTableView::dropEvent(QDropEvent *e)
{
  Profile *p=new Profile();
  p->setSourceText(e->mimeData()->data(RDMIMETYPE_CART));
  int cartnum=p->intValue("Rivendell-Cart","Number");
  int row=indexAt(e->pos()).row();
  LogModel *m=(LogModel *)model();
  if(row<0) {
    row=m->rowCount()-1;
  }
  m->insert(row,cartnum);
  e->acceptProposedAction();
  delete p;
  emit changed();
}
