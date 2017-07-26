// logtableview.h
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

#ifndef LOGTABLEVIEW_H
#define LOGTABLEVIEW_H

#include "tableview.h"

class LogTableView : public TableView
{
  Q_OBJECT
 public:
  LogTableView(QWidget *parent=0);

 signals:
  void changed();

 protected:
  virtual void dragEnterEvent(QDragEnterEvent *e);
  virtual void dragMoveEvent(QDragMoveEvent *e);
  virtual void dropEvent(QDropEvent *e);
};


#endif  //  LOGTABLEVIEW_H
