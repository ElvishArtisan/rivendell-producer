// cartfilterwidget.cpp
//
// Cart Filter Widget
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

#include <rivendell/rd_listgroups.h>

#include <QMessageBox>

#include "cartfilterwidget.h"
#include "config.h"

CartFilterWidget::CartFilterWidget(QWidget *parent)
  :QWidget(parent)
{
  //
  // Fonts
  //
  QFont bold_font(font().family(),font().pointSize(),QFont::Bold);

  //
  // Filter
  //
  main_filter_label=new QLabel(tr("Filter")+":",this);
  main_filter_label->setFont(bold_font);
  main_filter_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  main_filter_edit=new QLineEdit(this);
  connect(main_filter_edit,SIGNAL(textChanged(const QString &)),
	  this,SLOT(filterChangedData(const QString &)));
  connect(main_filter_edit,SIGNAL(returnPressed()),this,SLOT(searchData()));

  main_search_button=new QPushButton(tr("Search"),this);
  main_search_button->setFont(bold_font);
  connect(main_search_button,SIGNAL(clicked()),this,SLOT(searchData()));

  main_clear_button=new QPushButton(tr("Clear"),this);
  main_clear_button->setFont(bold_font);
  main_clear_button->setDisabled(true);
  connect(main_clear_button,SIGNAL(clicked()),this,SLOT(clearData()));

  //
  // Group Selector
  //
  main_group_label=new QLabel(tr("Group")+":",this);
  main_group_label->setFont(bold_font);
  main_group_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  main_group_box=new ComboBox(this);
  struct rd_group *groups;
  unsigned records=0;
  int err=0;
  if((err=RD_ListGroups(&groups,cnf->serverHostname().toUtf8(),
			cnf->serverUsername().toUtf8(),
			cnf->serverPassword().toUtf8(),&records))!=0) {
    QMessageBox::warning(this,tr("RDLibrary Browser - Error"),
			 tr("Unable to connect to Rivendell server")+
			 " ["+tr("Error")+QString().sprintf(" %d].",err));
    exit(256);
  }
  main_group_box->insertItem(0,tr("ALL"),tr("ALL"));
  for(unsigned i=0;i<records;i++) {
    main_group_box->
      insertItem(main_group_box->count(),groups[i].grp_name,groups[i].grp_name);
  }
  connect(main_group_box,SIGNAL(activated(int)),
	  this,SLOT(groupChangedData(int)));

  //
  // Type Selectors
  //
  main_audio_check=new QCheckBox(this);
  main_audio_check->setChecked(true);
  connect(main_audio_check,SIGNAL(toggled(bool)),
	  this,SLOT(typeChangedData(bool)));
  main_audio_label=new QLabel(tr("Show Audio Carts"),this);
  main_audio_label->setFont(bold_font);
  main_audio_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  main_macro_check=new QCheckBox(this);
  main_macro_check->setChecked(true);
  connect(main_macro_check,SIGNAL(toggled(bool)),
	  this,SLOT(typeChangedData(bool)));
  main_macro_label=new QLabel(tr("Show Macro Carts"),this);
  main_macro_label->setFont(bold_font);
  main_macro_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
}


QSize CartFilterWidget::sizeHint() const
{
  return QSize(100,100);
}


void CartFilterWidget::filterChangedData(const QString &str)
{
  main_search_button->setEnabled(true);
  main_clear_button->setDisabled(str.isEmpty());
}


void CartFilterWidget::groupChangedData(int index)
{
  main_search_button->setEnabled(true);
}


void CartFilterWidget::typeChangedData(bool state)
{
  main_search_button->setEnabled(true);
}


void CartFilterWidget::searchData()
{
  emit updateRequested(main_filter_edit->text(),main_group_box->currentText(),
		       main_audio_check->isChecked(),
		       main_macro_check->isChecked());
  main_search_button->setDisabled(true);
  main_clear_button->setDisabled(true);
}


void CartFilterWidget::clearData()
{
  main_filter_edit->clear();
  main_clear_button->setDisabled(true);
}


void CartFilterWidget::resizeEvent(QResizeEvent *e)
{
  main_filter_label->setGeometry(0,0,60,20);
  main_filter_edit->setGeometry(65,0,size().width()-245,20);

  main_search_button->setGeometry(size().width()-170,0,80,50);
  main_clear_button->setGeometry(size().width()-80,0,80,50);

  main_group_label->setGeometry(0,27,60,20);
  main_group_box->setGeometry(65,27,100,20);

  main_audio_check->setGeometry(65,54,20,20);
  main_audio_label->setGeometry(85,54,150,20);

  main_macro_check->setGeometry(290,54,20,20);
  main_macro_label->setGeometry(310,54,150,20);
}
