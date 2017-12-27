// datepicker.cpp
//
// A Qt-based application for testing General Purpose Outputs (GPO).
//
//   (C) Copyright 2002-2003,2016 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU Library General Public License 
//   version 2 as published by the Free Software Foundation.
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

#include <QPalette>

#include "datepicker.h"

DatePicker::DatePicker(int low_year,int high_year,QWidget *parent)
  :QWidget(parent)
{
  pick_low_year=low_year;
  pick_high_year=high_year;

  //
  // Generate Fonts
  //
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);
  QFont header_font=QFont("Helvetica",12,QFont::Bold);
  header_font.setPixelSize(12);

  //
  // Month
  //
  pick_month_box=new QComboBox(this);
  pick_month_box->setGeometry(0,0,120,26);
  for(int i=1;i<13;i++) {
    pick_month_box->insertItem(pick_month_box->count(),QDate::longMonthName(i));
  }
  connect(pick_month_box,SIGNAL(activated(int)),
	  this,SLOT(monthActivatedData(int)));

  //
  // Year
  //
  if((high_year-low_year)<=10) {
    pick_year_box=new QComboBox(this);
    pick_year_box->setGeometry(130,0,90,26);
    for(int i=low_year;i<(high_year+1);i++) {
      pick_year_box->insertItem(pick_year_box->count(),QString().sprintf("%04d",i));
    }
    connect(pick_year_box,SIGNAL(activated(int)),
	    this,SLOT(yearActivatedData(int)));
    pick_year_spin=NULL;
  }
  else {
    pick_year_spin=new QSpinBox(this);
    pick_year_spin->setGeometry(160,0,60,26);
    pick_year_spin->setRange(low_year,high_year);
    pick_year_box=NULL;
    connect(pick_year_spin,SIGNAL(valueChanged(int)),
	    this,SLOT(yearChangedData(int)));
  }

  //
  // Date Labels
  //
  QPalette weekend_palette=palette();
  weekend_palette.setColor(QPalette::Active,QPalette::Background,
			   palette().color(QPalette::Active,
					   QPalette::Mid));
  weekend_palette.setColor(QPalette::Inactive,QPalette::Background,
			   palette().color(QPalette::Active,
					   QPalette::Mid));

  QLabel *label=new QLabel(tr("Mo"),this);
  label->setGeometry(DATEPICKER_X_ORIGIN,30,30,30);
  label->setFont(header_font);
  label->setAlignment(Qt::AlignCenter);

  label=new QLabel(tr("Tu"),this);
  label->setGeometry(DATEPICKER_X_ORIGIN+DATEPICKER_X_INTERVAL,
		     DATEPICKER_Y_ORIGIN,30,30);
  label->setFont(header_font);
  label->setAlignment(Qt::AlignCenter);

  label=new QLabel(tr("We"),this);
  label->setGeometry(DATEPICKER_X_ORIGIN+DATEPICKER_X_INTERVAL*2,
		     DATEPICKER_Y_ORIGIN,30,30);
  label->setFont(header_font);
  label->setAlignment(Qt::AlignCenter);

  label=new QLabel(tr("Th"),this);
  label->setGeometry(DATEPICKER_X_ORIGIN+DATEPICKER_X_INTERVAL*3,
		     DATEPICKER_Y_ORIGIN,30,30);
  label->setFont(header_font);
  label->setAlignment(Qt::AlignCenter);

  label=new QLabel(tr("Fr"),this);
  label->setGeometry(DATEPICKER_X_ORIGIN+DATEPICKER_X_INTERVAL*4,
		     DATEPICKER_Y_ORIGIN,30,30);
  label->setFont(header_font);
  label->setAlignment(Qt::AlignCenter);

  label=new QLabel(tr("Sa"),this);
  label->setGeometry(DATEPICKER_X_ORIGIN+DATEPICKER_X_INTERVAL*5,
		     DATEPICKER_Y_ORIGIN,30,30);
  label->setFont(header_font);
  label->setAlignment(Qt::AlignCenter);
  label->setPalette(weekend_palette);

  label=new QLabel(tr("Su"),this);
  label->setGeometry(DATEPICKER_X_ORIGIN+DATEPICKER_X_INTERVAL*6,
		     DATEPICKER_Y_ORIGIN,30,30);
  label->setFont(header_font);
  label->setAlignment(Qt::AlignCenter);
  label->setPalette(weekend_palette);


  for(int i=0;i<6;i++) {
    for(int j=0;j<7;j++) {
      pick_date_label[i][j]=new QLabel(this);
      pick_date_label[i][j]->
	setGeometry(DATEPICKER_X_ORIGIN+DATEPICKER_X_INTERVAL*j,
		    DATEPICKER_Y_ORIGIN+20+DATEPICKER_Y_INTERVAL*i,30,30);
      pick_date_label[i][j]->setAlignment(Qt::AlignTop|Qt::AlignHCenter);
    }
  }
  PrintDays();
}


DatePicker::~DatePicker()
{
}


QSize DatePicker::sizeHint() const
{
  return QSize(220,175);
}


QSizePolicy DatePicker::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


QDate DatePicker::date() const
{
  return pick_date;
}


bool DatePicker::setDate(QDate date)
{
  if(!date.isValid()) {
    date=QDate::currentDate();
  }
  if((date.year()<pick_low_year)||(date.year()>pick_high_year)) {
    return false;
  }
  pick_date=date;

  pick_month_box->setCurrentIndex(date.month()-1);
  if(pick_year_box!=NULL) {
    pick_year_box->setCurrentIndex(date.year()-pick_low_year);
  }
  else {
    pick_year_spin->setValue(date.year());
  }
  PrintDays();
  return true;
}


void DatePicker::monthActivatedData(int id)
{
  QDate date=QDate(pick_date.year(),id+1,1);
  if(pick_date.day()<=date.daysInMonth()) {
    pick_date=QDate(pick_date.year(),id+1,pick_date.day());
  }
  else {
    pick_date=QDate(pick_date.year(),id+1,date.daysInMonth());
  }
  PrintDays();
}


void DatePicker::yearActivatedData(int id)
{
  QDate date=QDate(pick_low_year+pick_year_box->currentIndex(),
		   pick_date.month(),1);
  if(pick_date.day()<=date.daysInMonth()) {
    pick_date=QDate(pick_low_year+pick_year_box->currentIndex(),
		    pick_date.month(),pick_date.day());
  }
  else {
    pick_date=QDate(pick_low_year+pick_year_box->currentIndex(),
		    pick_date.month(),date.daysInMonth());
  }
  PrintDays();
}


void DatePicker::yearChangedData(int year)
{
  QDate date=QDate(pick_year_spin->value(),pick_date.month(),1);
  if(pick_date.day()<=date.daysInMonth()) {
    pick_date=QDate(pick_year_spin->value(),
		    pick_date.month(),pick_date.day());
  }
  else {
    pick_date=QDate(pick_year_spin->value(),
		    pick_date.month(),date.daysInMonth());
  }
  PrintDays();
}


void DatePicker::mousePressEvent(QMouseEvent *e)
{
  if((e->pos().x()<DATEPICKER_X_ORIGIN)||
     (e->pos().x()>(DATEPICKER_X_ORIGIN+DATEPICKER_X_INTERVAL*7))||
     (e->pos().y()<DATEPICKER_Y_ORIGIN)||
     (e->pos().y()>(DATEPICKER_Y_ORIGIN+DATEPICKER_Y_INTERVAL*7))) {
    QWidget::mousePressEvent(e);
    return;
  }
  int dow=(e->pos().x()-DATEPICKER_X_ORIGIN)/DATEPICKER_X_INTERVAL;
  int week=(e->pos().y()-DATEPICKER_Y_ORIGIN)/DATEPICKER_Y_INTERVAL-1;
  if((dow<0)||(dow>6)||(week<0)||(week>6)) {
    return;
  }
  if(pick_date_label[week][dow]->text().isEmpty()) {
    return;
  }
  pick_date=QDate(pick_date.year(),pick_date.month(),
		  pick_date_label[week][dow]->text().toInt());
  PrintDays();
}


void DatePicker::PrintDays()
{
  QDate top_date;

  //
  // Clear Days
  //
  QPalette weekend_palette=palette();
  weekend_palette.setColor(QPalette::Active,QPalette::Background,
			   palette().color(QPalette::Active,
					   QPalette::Mid));
  weekend_palette.setColor(QPalette::Inactive,QPalette::Background,
			   palette().color(QPalette::Active,
					   QPalette::Mid));
  for(int i=0;i<6;i++) {
    for(int j=0;j<5;j++) {
      pick_date_label[i][j]->clear();
      pick_date_label[i][j]->setPalette(palette());
    }
  }
  for(int i=0;i<6;i++) {
    for(int j=5;j<7;j++) {
      pick_date_label[i][j]->clear();
      pick_date_label[i][j]->setPalette(weekend_palette);
    }
  }

  //
  // Get Top of Month
  //
  if(pick_year_box!=NULL) {
    top_date=QDate(pick_low_year+pick_year_box->currentIndex(),
		   pick_month_box->currentIndex()+1,1);
  }
  else {
    top_date=QDate(pick_year_spin->value(),pick_month_box->currentIndex()+1,1);
  }

  //
  // Print Days
  //
  int dow_offset=top_date.dayOfWeek()-1;
  for(int i=1;i<(top_date.daysInMonth()+1);i++) {
    PrintDay(i,dow_offset);
    if(pick_date.day()==i) {
      SelectDay(i,dow_offset,true);
    }
  }
}


void DatePicker::PrintDay(int day,int dow_offset)
{
  int slot=day+dow_offset-1;
  int week=slot/7;
  int dow=slot-7*week;
  pick_date_label[week][dow]->setText(QString().sprintf("%d",day));
}


void DatePicker::SelectDay(int day,int dow_offset,bool state)
{
  int slot=day+dow_offset-1;
  int week=slot/7;
  int dow=slot-7*week;
  QPalette pal=palette();
  if(state) {
    pal.setColor(QPalette::Active,QPalette::Foreground,
		 palette().
		 color(QPalette::Active,QPalette::HighlightedText));
    pal.setColor(QPalette::Active,QPalette::Background,
		 palette().color(QPalette::Active,QPalette::Highlight));
    pal.setColor(QPalette::Inactive,QPalette::Foreground,
		 palette().
		 color(QPalette::Active,QPalette::HighlightedText));
    pal.setColor(QPalette::Inactive,QPalette::Background,
		 palette().color(QPalette::Active,QPalette::Highlight));
  }
  else {
    pal.setColor(QPalette::Active,QPalette::Foreground,
		 palette().color(QPalette::Active,QPalette::Text));
    pal.setColor(QPalette::Active,QPalette::Background,
		 palette().color(QPalette::Active,QPalette::Background));
    pal.setColor(QPalette::Inactive,QPalette::Foreground,
		 palette().color(QPalette::Active,QPalette::Text));
    pal.setColor(QPalette::Inactive,QPalette::Background,
		 palette().color(QPalette::Active,QPalette::Background));
  }
  pick_date_label[week][dow]->setPalette(pal);
}

