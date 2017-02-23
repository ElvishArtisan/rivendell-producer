// insertmetadialog.cpp
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

#include <QSignalMapper>

#include "insertmetadialog.h"
#include "logline.h"

InsertMetaDialog::InsertMetaDialog(QWidget *parent)
  : QDialog(parent)
{
  setWindowTitle(" ");

  QFont bold_font(font().family(),font().pointSize(),QFont::Bold);

  insert_label=new QLabel(tr("Insert a")+":",this);
  insert_label->setFont(bold_font);
  insert_label->setAlignment(Qt::AlignVCenter|Qt::AlignCenter);

  QSignalMapper *mapper=new QSignalMapper(this);
  connect(mapper,SIGNAL(mapped(int)),this,SLOT(clickedData(int)));

  insert_marker_button=new QPushButton(tr("Marker"),this);
  insert_marker_button->setFont(bold_font);
  connect(insert_marker_button,SIGNAL(clicked()),mapper,SLOT(map()));
  mapper->setMapping(insert_marker_button,(int)LogLine::Marker);

  insert_track_button=new QPushButton(tr("Voice Track"),this);
  insert_track_button->setFont(bold_font);
  connect(insert_track_button,SIGNAL(clicked()),mapper,SLOT(map()));
  mapper->setMapping(insert_track_button,(int)LogLine::Track);

  insert_chain_button=new QPushButton(tr("Log Chain"),this);
  insert_chain_button->setFont(bold_font);
  connect(insert_chain_button,SIGNAL(clicked()),mapper,SLOT(map()));
  mapper->setMapping(insert_chain_button,(int)LogLine::Chain);

  insert_cancel_button=new QPushButton(tr("Cancel"),this);
  insert_cancel_button->setFont(bold_font);
  connect(insert_cancel_button,SIGNAL(clicked()),mapper,SLOT(map()));
  mapper->setMapping(insert_cancel_button,(int)LogLine::UnknownType);
}


QSize InsertMetaDialog::sizeHint() const
{
  return QSize(200,240);
}


void InsertMetaDialog::clickedData(int id)
{
  done(id);
}


void InsertMetaDialog::closeEvent(QCloseEvent *e)
{
  clickedData((int)LogLine::UnknownType);
}


void InsertMetaDialog::resizeEvent(QResizeEvent *e)
{
  int edge=(size().height()-40)/4;

  insert_label->setGeometry(10,10,size().width()-20,20);
  insert_marker_button->setGeometry(10,35,size().width()-20,edge);
  insert_track_button->setGeometry(10,35+edge,size().width()-20,edge);
  insert_chain_button->setGeometry(10,35+2*edge,size().width()-20,edge);
  insert_cancel_button->setGeometry(10,35+3*edge,size().width()-20,edge);
}
