// librarymodel.cpp
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

#include <stdio.h>

#include <rivendell/rd_listcarts.h>

#include "config.h"
#include "librarymodel.h"

#include "../../icons/audio.xpm"
#include "../../icons/macro.xpm"

LibraryModel::LibraryModel(QObject *parent)
  : QAbstractTableModel(parent)
{
  //
  // Icons
  //
  model_audio_map=new QPixmap(audio_xpm);
  model_macro_map=new QPixmap(macro_xpm);

  //
  // Column Header Titles
  //
  model_column_titles.push_back("");
  model_column_titles.push_back(QObject::tr("Number"));
  model_column_titles.push_back(QObject::tr("Title"));
  model_column_titles.push_back(QObject::tr("Artist"));

  model_group_name=QObject::tr("ALL");
  Update();
}


QString LibraryModel::groupName() const
{
  return model_group_name;
}


unsigned LibraryModel::cartNumber(int row) const
{
  return model_cart_numbers.at(row);
}


int LibraryModel::rowCount(const QModelIndex &parent) const
{
  return model_column_fields.size();
}


int LibraryModel::columnCount(const QModelIndex &parent) const
{
  return model_column_titles.size();
}


QVariant LibraryModel::data(const QModelIndex &index,int role) const
{
  switch((Qt::ItemDataRole)role) {
  case Qt::DisplayRole:
    if(index.column()!=0) {
      return model_column_fields.at(index.row()).at(index.column());
    }
    break;

  case Qt::DecorationRole:
    if(index.column()==0) {
      if(model_column_fields.at(index.row()).at(index.column())=="2") {
	return QVariant(*model_macro_map);
      }
      return QVariant(*model_audio_map);
    }
    break;

  default:
    break;
  }
  return QVariant();
}


QVariant LibraryModel::headerData(int section,Qt::Orientation orient,
				  int role) const
{
  if(orient==Qt::Vertical) {
    return QVariant();
  }

  switch((Qt::ItemDataRole)role) {
  case Qt::DisplayRole:
    return model_column_titles.at(section);

  default:
    break;
  }
  return QVariant();
}


void LibraryModel::setGroupName(const QString &str)
{
  if(str!=model_group_name) {
    model_group_name=str;
    Update();
  }
}


void LibraryModel::Update()
{
  struct rd_cart *carts=NULL;
  QString group_name="ALL";
  unsigned numrecs=0;
  int err=0;

  if(model_group_name!=QObject::tr("ALL")) {
    group_name=model_group_name;
  }
  if((err=RD_ListCarts(&carts,cnf->serverHostname().toUtf8(),
		       cnf->serverUsername().toUtf8(),
		       cnf->serverPassword().toUtf8(),
		       group_name.toUtf8(),"","",&numrecs))==0) {
    if(model_cart_numbers.size()>0) {
      beginRemoveRows(QModelIndex(),0,model_column_fields.size()-1);
      model_cart_numbers.clear();
      model_column_fields.clear();
      endRemoveRows();
    }
    if(numrecs>0) {
      beginInsertRows(QModelIndex(),0,numrecs-1);
      for(unsigned i=0;i<numrecs;i++) {
	model_cart_numbers.push_back(carts[i].cart_number);
	model_column_fields.push_back(QStringList());
	model_column_fields.back().
	  push_back(QString().sprintf("%u",carts[i].cart_type));
	model_column_fields.back().
	  push_back(QString().sprintf("%06u",carts[i].cart_number));
	model_column_fields.back().push_back(carts[i].cart_title);
	model_column_fields.back().push_back(carts[i].cart_artist);
      }
      endInsertRows();
    }
  }
  else {
    fprintf(stderr,"LibraryModel: RD_ListCarts returned error %d\n",err);
  }
}
