// rdlibraryb.h
//
// Remote cart producer for Rivendell
//
//   (C) Copyright 2016-2019 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef RDLIBRARYB_H
#define RDLIBRARYB_H

#include <vector>

#include <QLabel>
#include <QMainWindow>
#include <QPushButton>

#include "cartfilterwidget.h"
#include "librarymodel.h"
#include "libtableview.h"
#include "streamplayer.h"
#include "transportbutton.h"

#define RDLIBRARYB_USAGE "\n"

class MainWidget : public QMainWindow
{
  Q_OBJECT
 public:
  MainWidget(QWidget *parent=0);
  QSize sizeHint() const;

 private slots:
  void serverLoginUpdatedData(const QString &username);
  void cartClickedData(const QModelIndex &index);
  void playData();
  void stopData();
  void playerStateChangedData(StreamPlayer::State state);
  void playerErrorData(const QString &msg);
  void capiErrorData(int err,const QString &err_msg);

 protected:
  void closeEvent(QCloseEvent *e);
  void resizeEvent(QResizeEvent *e);

 private:
  LibraryModel *main_library_model;
  LibTableView *main_library_view;
  TransportButton *main_play_button;
  TransportButton *main_stop_button;
  QPushButton *main_close_button;  
  StreamPlayer *main_stream_player;
  unsigned main_selected_cart;
  bool main_is_closing;
  CartFilterWidget *main_filter_widget;
};


#endif  // RDLIBRARYB_H
