// librarymodel.h
//
// Remote data model for the Rivendell Library
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

#ifndef LIBRARYMODEL_H
#define LIBRARYMODEL_H

#include <QAbstractTableModel>
#include <QFont>
#include <QList>
#include <QPixmap>
#include <QStringList>

#include "logline.h"

class LibraryModel : public QAbstractTableModel
{
  Q_OBJECT
 public:
  LibraryModel(QObject *parent=0);
  unsigned cartNumber(int row) const;
  void getLogLine(LogLine *ll,int row) const;
  void setBoldFont(const QFont &font);
  int rowCount(const QModelIndex &parent=QModelIndex()) const;
  int columnCount(const QModelIndex &parent=QModelIndex()) const;
  QVariant data(const QModelIndex &index,int role=Qt::DisplayRole) const;
  QVariant headerData(int section,Qt::Orientation orient,
		      int role=Qt::DisplayRole) const;

 signals:
  void error(const QString &err_msg);

 public slots:
  void update(const QString &filter,const QString &grp_name,bool show_audio,
	      bool show_macro);

 private:
  void LoadColorMap();
  QString GetLength(struct rd_cart *cart) const;
  QPixmap *model_audio_map;
  QPixmap *model_macro_map;
  QList<QVariant> model_column_titles;
  QList<QVariant> model_column_alignments;
  QList<QStringList> model_column_fields;
  QList<unsigned> model_cart_numbers;
  QMap<QString,QVariant> model_group_colors;
  QFont model_bold_font;
};


#endif  // LIBRARYMODEL_H
