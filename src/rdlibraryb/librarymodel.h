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
#include <QList>
#include <QPixmap>
#include <QStringList>

class LibraryModel : public QAbstractTableModel
{
  Q_OBJECT
 public:
  LibraryModel(QObject *parent=0);
  QString groupName() const;
  int rowCount(const QModelIndex &parent=QModelIndex()) const;
  int columnCount(const QModelIndex &parent=QModelIndex()) const;
  QVariant data(const QModelIndex &index,int role=Qt::DisplayRole) const;
  QVariant headerData(int section,Qt::Orientation orient,
		      int role=Qt::DisplayRole) const;

 public slots:
  void setGroupName(const QString &str);

 private:
  void Update();
  QPixmap *model_audio_map;
  QPixmap *model_macro_map;
  QString model_group_name;
  QList<QVariant> model_column_titles;
  QList<QStringList> model_column_fields;
};


#endif  // LIBRARYMODEL_H
