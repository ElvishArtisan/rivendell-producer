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
#include <rivendell/rd_listgroups.h>

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
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignCenter);
  model_column_titles.push_back(QObject::tr("Cart"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignCenter);
  model_column_titles.push_back(QObject::tr("Group"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignCenter);
  model_column_titles.push_back(QObject::tr("Length"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignRight);
  model_column_titles.push_back(QObject::tr("Title"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignLeft);
  model_column_titles.push_back(QObject::tr("Artist"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignLeft);
  model_column_titles.push_back(QObject::tr("Album"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignLeft);
  model_column_titles.push_back(QObject::tr("Label"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignLeft);
  model_column_titles.push_back(QObject::tr("Composer"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignLeft);
  model_column_titles.push_back(QObject::tr("Conductor"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignLeft);
  model_column_titles.push_back(QObject::tr("Publisher"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignLeft);
  model_column_titles.push_back(QObject::tr("Client"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignLeft);
  model_column_titles.push_back(QObject::tr("Agency"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignLeft);
  model_column_titles.push_back(QObject::tr("User Defined"));
  model_column_alignments.push_back((int)Qt::AlignVCenter|Qt::AlignLeft);
}


unsigned LibraryModel::cartNumber(int row) const
{
  return model_cart_numbers.at(row);
}


void LibraryModel::getLogLine(LogLine *ll,int row) const
{
  QStringList fields=model_column_fields.at(row);

  ll->clear();
  switch((LogLine::Type)model_column_fields.at(row).at(0).toInt()) {
  case LogLine::Cart:
    ll->setType(LogLine::Cart);
    break;

  case LogLine::Macro:
    ll->setType(LogLine::Macro);
    break;

  default:
    break;
  }
  ll->setCartNumber(fields.at(1).toUInt());
  ll->setGroupName(fields.at(2));
  ll->setTitle(fields.at(4));
  ll->setArtist(fields.at(5));
  ll->setAlbum(fields.at(6));
  ll->setLabel(fields.at(7));
  ll->setComposer(fields.at(8));
  ll->setConductor(fields.at(9));
  ll->setPublisher(fields.at(10));
  ll->setClient(fields.at(11));
  ll->setAgency(fields.at(12));
  ll->setUserDefined(fields.at(13));
}


void LibraryModel::setBoldFont(const QFont &font)
{
  model_bold_font=font;
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
  case Qt::TextAlignmentRole:
    return model_column_alignments.at(index.column());

  case Qt::TextColorRole:
    if(index.column()==2) {
      return model_group_colors[model_column_fields.at(index.row()).
				at(index.column())];
    }
    break;

  case Qt::FontRole:
    if(index.column()==2) {
      return model_bold_font;
    }
    break;

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


void LibraryModel::update(const QString &filter,const QString &grp_name,
			  bool show_audio,bool show_macro)
{
  struct rd_cart *carts=NULL;
  unsigned numrecs=0;
  int err=0;

  LoadColorMap();

  if(model_cart_numbers.size()>0) {
    beginRemoveRows(QModelIndex(),0,model_column_fields.size()-1);
    model_cart_numbers.clear();
    model_column_fields.clear();
    endRemoveRows();
  }
  QString type="";
  if((!show_audio)&&(!show_macro)) {
    return;
  }
  if(show_audio&&(!show_macro)) {
    type="Audio";
  }
  if((!show_audio)&&show_macro) {
    type="Macro";
  }

  if((err=RD_ListCarts(&carts,cnf->serverHostname().toUtf8(),
		       cnf->serverUsername().toUtf8(),
		       cnf->serverPassword().toUtf8(),
		       cnf->serverTicket().toUtf8(),
		       grp_name.toUtf8(),filter,type,&numrecs))==0) {
    if(numrecs>0) {
      beginInsertRows(QModelIndex(),0,numrecs-1);
      for(unsigned i=0;i<numrecs;i++) {
	model_cart_numbers.push_back(carts[i].cart_number);
	model_column_fields.push_back(QStringList());
	model_column_fields.back().
	  push_back(QString().sprintf("%u",carts[i].cart_type));
	model_column_fields.back().
	  push_back(QString().sprintf("%06u",carts[i].cart_number));
	model_column_fields.back().push_back(carts[i].cart_grp_name);
	model_column_fields.back().push_back(GetLength(&carts[i]));
	model_column_fields.back().push_back(carts[i].cart_title);
	model_column_fields.back().push_back(carts[i].cart_artist);
	model_column_fields.back().push_back(carts[i].cart_album);
	model_column_fields.back().push_back(carts[i].cart_label);
	model_column_fields.back().push_back(carts[i].cart_composer);
	model_column_fields.back().push_back(carts[i].cart_conductor);
	model_column_fields.back().push_back(carts[i].cart_publisher);
	model_column_fields.back().push_back(carts[i].cart_client);
	model_column_fields.back().push_back(carts[i].cart_agency);
	model_column_fields.back().push_back(carts[i].cart_user_defined);
      }
      endInsertRows();
    }
  }
  else {
    emit capiError(err,"Error in RD_ListCarts() call");
  }
}


void LibraryModel::LoadColorMap()
{
  struct rd_group *grps=NULL;
  unsigned grp_quan=0;
  int err=0;

  if((err=RD_ListGroups(&grps,cnf->serverHostname(),cnf->serverUsername(),
			cnf->serverPassword(),cnf->serverTicket(),
			&grp_quan))!=0) {
    emit capiError(err,"error in RD_ListGroups() call");
    return;
  }
  model_group_colors.clear();
  for(unsigned i=0;i<grp_quan;i++) {
    model_group_colors[grps[i].grp_name]=QVariant(QColor(grps[i].grp_color));
  }
}


QString LibraryModel::GetLength(struct rd_cart *cart) const
{
  QTime len;

  if(cart->cart_forced_length>0) {
    len=QTime().addMSecs(cart->cart_forced_length);
  }
  else {
    len=QTime().addMSecs(cart->cart_average_length);
  }
  if(cart->cart_forced_length<60000) {
    return len.toString("ss.zzz").left(4);
  }
  if(cart->cart_forced_length<3600000) {
    return len.toString("mm:ss.zzz").left(7);
  }
  return len.toString("hh:mm:ss.zzz").left(10);
}
