// editlogdialog.cpp
//
// Remote log editor for Rivendell
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

#include <stdio.h>

#include <QApplication>
#include <QMessageBox>
#include <QPainter>

#include "cmdswitch.h"
#include "datetime.h"
#include "playerfactory.h"
#include "textreport.h"

#include "editlogdialog.h"

EditLogDialog::EditLogDialog(AddLogDialog *ad,QWidget *parent)
  :QDialog(parent)
{
  int err;
  edit_addlog_dialog=ad;
  edit_modified=false;

  //
  // Window Title Bar
  //
  setWindowTitle(tr("RDLogEdit - Edit Log"));

  //
  // Fix the Window Size
  //
  setMinimumSize(sizeHint());

  //
  // Fonts
  //
  QFont bold_font(font().family(),font().pointSize(),QFont::Bold);

  //
  // Dialogs
  //
  edit_date_dialog=new DateDialog(QDate::currentDate().year(),
				  QDate::currentDate().year()+10,this);
  edit_logline_dialog=new EditLogLineDialog(this);
  edit_insertmeta_dialog=new InsertMetaDialog(this);

  //
  // Stream Player
  //
  edit_stream_player=PlayerFactory(cnf,this);
  connect(edit_stream_player,SIGNAL(stateChanged(StreamPlayer::State)),
	  this,SLOT(playerStateChangedData(StreamPlayer::State)));
  connect(edit_stream_player,SIGNAL(error(const QString &)),
	  this,SLOT(playerErrorData(const QString &)));

  //
  // Log Modified Indicator
  //
  edit_modified_label=new QLabel("*",this);
  edit_modified_label->setAlignment(Qt::AlignTop|Qt::AlignCenter);
  edit_modified_label->setFont(bold_font);
  edit_modified_label->hide();

  //
  // Log Name
  //
  edit_name_label_label=new QLabel(tr("Log Name")+":",this);
  edit_name_label_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  edit_name_label_label->setFont(bold_font);
  edit_name_label=new QLabel(this);
  edit_name_label->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);

  //
  // Log Tracks
  //
  edit_tracks_label_label=new QLabel(tr("Tracks")+":",this);
  edit_tracks_label_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  edit_tracks_label_label->setFont(bold_font);
  edit_tracks_label=new QLabel(this);
  edit_tracks_label->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);

  //
  // Log Origin
  //
  edit_origin_label_label=new QLabel(tr("Origin")+":",this);
  edit_origin_label_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  edit_origin_label_label->setFont(bold_font);
  edit_origin_label=new QLabel(this);
  edit_origin_label->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);

  //
  // Log Description
  //
  edit_description_label=new QLabel(tr("Description")+":",this);
  edit_description_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  edit_description_label->setFont(bold_font);
  edit_description_edit=new QLineEdit(this);
  connect(edit_description_edit,SIGNAL(textChanged(const QString &)),
	  this,SLOT(descriptionChanged(const QString &)));

  //
  // Deletion Date
  //
  edit_logdelete_check=new QCheckBox(this);
  connect(edit_logdelete_check,SIGNAL(toggled(bool)),
	  this,SLOT(deleteDateData(bool)));
  edit_logdelete_label=new QLabel(tr("Delete on"),this);
  edit_logdelete_label->setAlignment(Qt::AlignVCenter|Qt::AlignCenter);
  edit_logdelete_label->setFont(bold_font);
  edit_logdelete_edit=new QDateTimeEdit(this);
  edit_logdelete_edit->setDisplayFormat("MM/dd/yyyy");
  edit_logdelete_button=new QPushButton(tr("Select"),this);
  connect(edit_logdelete_button,SIGNAL(clicked()),
	  this,SLOT(deleteDateSelectData()));

  //
  // Service
  //
  edit_service_label=new QLabel(tr("Service")+":",this);
  edit_service_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  edit_service_label->setFont(bold_font);
  edit_service_box=new ServiceBox(false,this);
  connect(edit_service_box,SIGNAL(activated(int)),
	  this,SLOT(boxActivatedData(int)));
  if((err=edit_service_box->reload())!=0) {
    QMessageBox::warning(this,tr("RDLogEdit - Error"),
			 tr("Error in rd_listservices() call")+
			 " ["+tr("Error")+QString().sprintf(" %d].",err));
  }

  //
  // Start Date
  //
  edit_startdate_label=new QLabel(tr("Start Date")+":",this);
  edit_startdate_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  edit_startdate_label->setFont(bold_font);
  edit_startdate_edit=new QDateTimeEdit(this);
  edit_startdate_edit->setDisplayFormat("MM/dd/yyyy");
  connect(edit_startdate_edit,SIGNAL(dateChanged(const QDate &)),
	  this,SLOT(dateChangedData(const QDate &)));
  //
  // End Date
  //
  edit_enddate_label=new QLabel(tr("End Date")+":",this);
  edit_enddate_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  edit_enddate_label->setFont(bold_font);
  edit_enddate_edit=new QDateTimeEdit(this);
  edit_enddate_edit->setDisplayFormat("MM/dd/yyyy");
  connect(edit_enddate_edit,SIGNAL(dateChanged(const QDate &)),
	  this,SLOT(dateChangedData(const QDate &)));

  //
  // Autorefresh
  //
  edit_autorefresh_label=new QLabel(tr("Enable AutoRefresh")+":",this);
  edit_autorefresh_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  edit_autorefresh_label->setFont(bold_font);
  edit_autorefresh_box=new ComboBox(this);
  edit_autorefresh_box->insertItem(0,tr("No"));
  edit_autorefresh_box->insertItem(1,tr("Yes"));
  connect(edit_autorefresh_box,SIGNAL(activated(int)),
	  this,SLOT(boxActivatedData(int)));

  //
  // Start Date Check
  //
  edit_startdate_check=new QCheckBox(this);
  connect(edit_startdate_check,SIGNAL(toggled(bool)),
  	  this,SLOT(startDateData(bool)));
  edit_startdate_check_label=new QLabel(tr("Start Date Enabled"),this);
  edit_startdate_check_label->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
  edit_startdate_check_label->setFont(bold_font);

  //
  // End Date Check
  //
  edit_enddate_check=new QCheckBox(this);
  connect(edit_enddate_check,SIGNAL(toggled(bool)),
  	  this,SLOT(endDateData(bool)));
  edit_enddate_check_label=new QLabel(tr("End Date Enabled"),this);
  edit_enddate_check_label->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
  edit_enddate_check_label->setFont(bold_font);

  //
  // Log Events
  //
  edit_log_model=new LogModel(this);
  edit_log_model->setBoldFont(bold_font);
  connect(edit_log_model,SIGNAL(capiError(int,const QString &)),
	  this,SLOT(capiErrorData(int,const QString &)));
  edit_log_view=new LogTableView(this);
  edit_log_view->setSelectionMode(QAbstractItemView::ContiguousSelection);
  edit_log_view->setModel(edit_log_model);
  edit_log_view->resizeColumnsToContents();
  connect(edit_log_view->selectionModel(),
	  SIGNAL(selectionChanged(const QItemSelection &,
				  const QItemSelection &)),
	  this,
	  SLOT(eventSelectionChangedData(const QItemSelection &,
					 const QItemSelection &)));
  connect(edit_log_view,SIGNAL(doubleClicked(const QModelIndex &)),
	  this,SLOT(eventDoubleClickedData(const QModelIndex &)));
  connect(edit_log_view,SIGNAL(changed()),this,SLOT(logChangedData()));

  //
  // Run Length Widget
  //
  edit_runlength_widget=
    new RunLengthWidget(edit_log_model,edit_log_view->selectionModel(),this);
  edit_runlength_widget->setFont(bold_font);
  connect(edit_log_view->selectionModel(),
	  SIGNAL(selectionChanged(const QItemSelection &,const QItemSelection &)),
	  edit_runlength_widget,
	  SLOT(updateSelection(const QItemSelection &,const QItemSelection &)));

  //
  // Insert Cart Button
  //
  edit_insertcart_button=new QPushButton(tr("Insert")+"\n"+tr("Cart"),this);
  edit_insertcart_button->setFont(bold_font);
  connect(edit_insertcart_button,SIGNAL(clicked()),this,SLOT(insertCartData()));

  //
  // Insert Meta Button
  //
  edit_insertmeta_button=new QPushButton(tr("Insert")+"\n"+tr("Meta"),this);
  edit_insertmeta_button->setFont(bold_font);
  connect(edit_insertmeta_button,SIGNAL(clicked()),this,SLOT(insertMetaData()));

  //
  // Edit Button
  //
  edit_edit_button=new QPushButton(tr("Edit"),this);
  edit_edit_button->setFont(bold_font);
  connect(edit_edit_button,SIGNAL(clicked()),this,SLOT(editData()));

  //
  // Delete Button
  //
  edit_delete_button=new QPushButton(tr("Delete"),this);
  edit_delete_button->setFont(bold_font);
  connect(edit_delete_button,SIGNAL(clicked()),this,SLOT(deleteData()));

  //
  // Up Button
  //
  edit_up_button=new TransportButton(TransportButton::Up,this);
  connect(edit_up_button,SIGNAL(clicked()),this,SLOT(upData()));

  //
  // Down Button
  //
  edit_down_button=new TransportButton(TransportButton::Down,this);
  connect(edit_down_button,SIGNAL(clicked()),this,SLOT(downData()));

  //
  // Cut Button
  //
  edit_cut_button=new QPushButton(tr("Cut"),this);
  edit_cut_button->setFont(bold_font);
  connect(edit_cut_button,SIGNAL(clicked()),this,SLOT(cutData()));

  //
  // Copy Button
  //
  edit_copy_button=new QPushButton(tr("Copy"),this);
  edit_copy_button->setFont(bold_font);
  connect(edit_copy_button,SIGNAL(clicked()),this,SLOT(copyData()));

  //
  // Paste Button
  //
  edit_paste_button=new QPushButton(tr("Paste"),this);
  edit_paste_button->setFont(bold_font);
  connect(edit_paste_button,SIGNAL(clicked()),this,SLOT(pasteData()));

  //
  // Save Button
  //
  edit_save_button=new QPushButton(tr("Save"),this);
  edit_save_button->setFont(bold_font);
  edit_save_button->setDisabled(true);
  connect(edit_save_button,SIGNAL(clicked()),this,SLOT(saveData()));

  //
  // Save As Button
  //
  edit_saveas_button=new QPushButton(tr("Save")+"\n"+tr("As"),this);
  edit_saveas_button->setFont(bold_font);
  connect(edit_saveas_button,SIGNAL(clicked()),this,SLOT(saveasData()));

  //
  // Print Button
  //
  edit_print_button=new QPushButton(tr("Print"),this);
  edit_print_button->setFont(bold_font);
  connect(edit_print_button,SIGNAL(clicked()),this,SLOT(printData()));

  //
  // Play Button
  //
  edit_play_button=new TransportButton(TransportButton::Play,this);
  edit_play_button->setDisabled(true);
  connect(edit_play_button,SIGNAL(clicked()),this,SLOT(playData()));

  //
  // Stop Button
  //
  edit_stop_button=new TransportButton(TransportButton::Stop,this);
  edit_stop_button->on();
  edit_stop_button->setDisabled(true);
  connect(edit_stop_button,SIGNAL(clicked()),this,SLOT(stopData()));

  //
  // Ok Button
  //
  edit_ok_button=new QPushButton(tr("OK"),this);
  edit_ok_button->setFont(bold_font);
  connect(edit_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  // Cancel Button
  //
  edit_cancel_button=new QPushButton(tr("Cancel"),this);
  edit_cancel_button->setFont(bold_font);
  connect(edit_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));
}


QSize EditLogDialog::sizeHint() const
{
  return QSize(810,600);
}


int EditLogDialog::exec(const QString &logname)
{
  QDate date;
  QString err_msg;

  edit_insertcart_button->setDisabled(true);
  edit_insertmeta_button->setDisabled(true);
  edit_edit_button->setDisabled(true);
  edit_delete_button->setDisabled(true);
  edit_up_button->setDisabled(true);
  edit_down_button->setDisabled(true);
  edit_cut_button->setDisabled(true);
  edit_copy_button->setDisabled(true);
  edit_paste_button->setDisabled(true);

  edit_selected_logid=-1;
  edit_log_view->clearSelection();
  edit_play_button->setDisabled(true);
  edit_stop_button->setDisabled(true);
  if(!edit_log_model->load(logname,&err_msg)) {
    QMessageBox::critical(this,tr("RDLogEdit - Error"),
			  tr("Log load failed")+" ["+err_msg+"].");
    return QDialog::exec();
  }
  edit_log_view->resizeColumnsToContents();
  edit_name_label->setText(logname);
  edit_tracks_label->
    setText(QString().sprintf("%d / %d",edit_log_model->completedTracks(),
			      edit_log_model->scheduledTracks()));
  edit_origin_label->setText(edit_log_model->originUsername()+" - "+
			     edit_log_model->originDateTime().
			     toString("MM/dd/yyyy - hh:mm:ss"));
  edit_description_edit->setText(edit_log_model->description());
  if(edit_log_model->purgeDate().isValid()) {
    edit_logdelete_edit->setDate(edit_log_model->purgeDate());
    edit_logdelete_label->setEnabled(true);
    edit_logdelete_edit->setEnabled(true);
    edit_logdelete_button->setEnabled(true);
    edit_logdelete_check->setChecked(true);
  }
  else {
    edit_logdelete_edit->setDate(QDate::currentDate());
    edit_logdelete_label->setDisabled(true);
    edit_logdelete_button->setDisabled(true);
    edit_logdelete_edit->setDisabled(true);
    edit_logdelete_check->setChecked(false);
  }
  edit_service_box->setCurrentItemData(edit_log_model->serviceName());
  edit_autorefresh_box->setCurrentIndex(edit_log_model->autorefresh());
  if(edit_log_model->startDate().isValid()) {
    edit_startdate_edit->setDate(edit_log_model->startDate());
    edit_startdate_label->setEnabled(true);
    edit_startdate_edit->setEnabled(true);
    edit_startdate_check->setChecked(true);
  }
  else {
    edit_startdate_edit->setDate(QDate::currentDate());
    edit_startdate_label->setDisabled(true);
    edit_startdate_edit->setDisabled(true);
    edit_startdate_check->setChecked(false);
  }
  if(edit_log_model->endDate().isValid()) {
    edit_enddate_edit->setDate(edit_log_model->endDate());
    edit_enddate_label->setEnabled(true);
    edit_enddate_edit->setEnabled(true);
    edit_enddate_check->setChecked(true);
  }
  else {
    edit_enddate_edit->setDate(QDate::currentDate());
    edit_enddate_label->setDisabled(true);
    edit_enddate_edit->setDisabled(true);
    edit_enddate_check->setChecked(false);
  }
  SetModified(false);

  return QDialog::exec();
}


void EditLogDialog::descriptionChanged(const QString &str)
{
  SetModified(true);
}


void EditLogDialog::dateChangedData(const QDate &date)
{
  SetModified(true);
}


void EditLogDialog::boxActivatedData(int index)
{
  SetModified(true);
}


void EditLogDialog::deleteDateData(bool state)
{
  edit_logdelete_label->setEnabled(state);
  edit_logdelete_edit->setEnabled(state);
  edit_logdelete_button->setEnabled(state);
  SetModified(true);
}


void EditLogDialog::deleteDateSelectData()
{
  QDate date=edit_logdelete_edit->date();

  if(edit_date_dialog->exec(&date)) {
    edit_logdelete_edit->setDate(date);
    SetModified(true);
  }
}


void EditLogDialog::startDateData(bool state)
{
  edit_startdate_label->setEnabled(state);
  edit_startdate_edit->setEnabled(state);
  SetModified(true);
}


void EditLogDialog::endDateData(bool state)
{
  edit_enddate_label->setEnabled(state);
  edit_enddate_edit->setEnabled(state);
  SetModified(true);
}


void EditLogDialog::eventSelectionChangedData(const QItemSelection &newsel,
					      const QItemSelection &oldsel)
{
  LogLine *ll=NULL;
  QItemSelectionModel *s=edit_log_view->selectionModel();
  int rows=s->selectedRows().size();
  bool end_selected=false;

  for(int i=0;i<rows;i++) {
    if(s->selectedRows().at(i).row()>=edit_log_model->rowCount()-1) {
      end_selected=true;
    }
  }

  if(rows==1) {
    ll=edit_log_model->logLine(s->selectedRows().at(0).row());
    edit_play_button->
      setEnabled((ll!=NULL)&&(ll->type()==LogLine::Cart));
    edit_stop_button->
      setEnabled((ll!=NULL)&&(ll->type()==LogLine::Cart));

    if(edit_stream_player->state()==StreamPlayer::Playing) {
      if((ll!=NULL)&&(ll->type()==LogLine::Cart)) {
	if(ll->id()!=edit_selected_logid) {
	  edit_stream_player->
	    play(ll->cartNumber(),1,-1,-1);
	  edit_selected_logid=ll->id();
	}
      }
      else {
	edit_stream_player->stop();
      }
    }
  }

  edit_insertcart_button->setEnabled(rows==1);
  edit_insertmeta_button->setEnabled(rows==1);
  edit_edit_button->setEnabled((rows==1)&&(!end_selected));
  edit_delete_button->setEnabled(!end_selected);
  edit_up_button->setEnabled((rows==1)&&(!end_selected));
  edit_down_button->setEnabled((rows==1)&&(!end_selected));
  edit_cut_button->setEnabled((rows>0)&&(!end_selected));
  edit_copy_button->setEnabled((rows>0)&&(!end_selected));
  edit_paste_button->setEnabled(rows>0);
}


void EditLogDialog::eventDoubleClickedData(const QModelIndex &index)
{
  editData();
}


void EditLogDialog::insertCartData()
{
  LogLine *ll=NULL;
  QItemSelectionModel *s=edit_log_view->selectionModel();
  if(s->hasSelection()) {
    ll=new LogLine();
    ll->setType(LogLine::Cart);
    if(edit_logline_dialog->
       exec(ll,edit_service_box->currentItemData().toString(),
	    GetStartTimes(-1))) {
      ll->setId(GetNextId());
      edit_log_model->insert(s->selectedRows()[0].row(),ll);
      SetModified(true);
    }
  }
}


void EditLogDialog::insertMetaData()
{
  LogLine *ll=NULL;
  QItemSelectionModel *s=edit_log_view->selectionModel();
  if(s->hasSelection()) {
    ll=new LogLine();
    ll->setType((LogLine::Type)edit_insertmeta_dialog->exec());
    if(ll->type()==LogLine::UnknownType) {
      delete ll;
      return;
    }
    if(edit_logline_dialog->
       exec(ll,edit_service_box->currentItemData().toString(),
	    GetStartTimes(s->selectedRows()[0].row()))) {
      ll->setId(GetNextId());
      edit_log_model->insert(s->selectedRows()[0].row(),ll);
    }
  }
}


void EditLogDialog::editData()
{
  LogLine *ll=NULL;
  QItemSelectionModel *s=edit_log_view->selectionModel();
  if(s->hasSelection()) {
    if((ll=edit_log_model->logLine(s->selectedRows()[0]))==NULL) {
      insertCartData();
    }
    else {
      if(edit_logline_dialog->
	 exec(ll,edit_service_box->currentItemData().toString(),
	      GetStartTimes(s->selectedRows()[0].row()))) {
	edit_log_model->updateRow(s->selectedRows()[0].row());
	SetModified(true);
      }
    }
  }
}


void EditLogDialog::deleteData()
{
  QItemSelectionModel *s=edit_log_view->selectionModel();
  if(s->hasSelection()&&
     (s->selectedRows().at(0).row()<(edit_log_model->rowCount()-1))) {
    int newrow=s->selectedRows().at(0).row();
    edit_log_model->
      removeAt(s->selectedRows().at(0).row(),s->selectedRows().size());
    edit_log_view->select(newrow,1);
    SetModified(true);
  }
}


void EditLogDialog::upData()
{
  QItemSelectionModel *s=edit_log_view->selectionModel();
  if(s->hasSelection()) {
    int row=s->selectedRows()[0].row();
    if((row>0)&&(row<(edit_log_model->rowCount()-1))) {
      edit_log_model->move(row,row-1);
      s->select(edit_log_model->
		index(row-1,0,QModelIndex()),QItemSelectionModel::Rows|
		QItemSelectionModel::ClearAndSelect);
      SetModified(true);
    }
  }
}


void EditLogDialog::downData()
{
  QItemSelectionModel *s=edit_log_view->selectionModel();
  if(s->hasSelection()) {
    int row=s->selectedRows()[0].row();
    if(row<(edit_log_model->rowCount()-2)) {
      edit_log_model->move(row,row+1);
      s->select(edit_log_model->
		index(row+1,0,QModelIndex()),QItemSelectionModel::Rows|
		QItemSelectionModel::ClearAndSelect);
      SetModified(true);
    }
  }
}


void EditLogDialog::cutData()
{
  copyData();
  deleteData();
}


void EditLogDialog::copyData()
{
  QItemSelectionModel *s=edit_log_view->selectionModel();
  edit_clipboard.clear();
  for(int i=0;i<s->selectedRows().size();i++) {
    edit_clipboard.
      push_back(new LogLine(*edit_log_model->
			    logLine(s->selectedRows().at(i).row())));
  }
}


void EditLogDialog::pasteData()
{
  QItemSelectionModel *s=edit_log_view->selectionModel();
  int row=s->selectedRows().at(0).row();
  for(int i=0;i<edit_clipboard.size();i++) {
    edit_clipboard.at(i)->setId(GetNextId());
    edit_log_model->insert(row+i,edit_clipboard.at(i));
  }
  s->select(edit_log_model->index(row+edit_clipboard.size(),0,QModelIndex()),
	    QItemSelectionModel::ClearAndSelect|QItemSelectionModel::Rows);
  SetModified(true);
}


void EditLogDialog::playData()
{
  QItemSelectionModel *s=edit_log_view->selectionModel();
  if(s->hasSelection()) {
    unsigned cartnum=
      edit_log_model->logLine(s->selectedRows()[0])->cartNumber();
    edit_stream_player->play(cartnum,1,-1,-1);
  }
}


void EditLogDialog::stopData()
{
  edit_stream_player->stop();
}


void EditLogDialog::playerStateChangedData(StreamPlayer::State state)
{
  switch(state) {
  case StreamPlayer::Playing:
    edit_play_button->on();
    edit_stop_button->off();
    break;

  case StreamPlayer::Stopped:
    /*
    if(edit_is_closing) {
      exit(0);
    }
    */
    edit_play_button->off();
    edit_stop_button->on();
    break;
  }
}


void EditLogDialog::playerErrorData(const QString &msg)
{
  QMessageBox::information(this,"RDLogEdit - "+tr("Player Error"),msg);
}


void EditLogDialog::saveData()
{
  Save();
}


void EditLogDialog::printData()
{
  QString report="";

  //
  // Generate Header
  //
  QString refresh="No ";
  if(edit_autorefresh_box->currentIndex()) {
    refresh="Yes";
  }
  QString start_date=tr("[none]");
  if(edit_startdate_check->isChecked()) {
    start_date=edit_startdate_edit->date().toString("MM/dd/yyyy");
  }
  QString end_date=tr("[none]");
  if(edit_enddate_check->isChecked()) {
    end_date=edit_enddate_edit->date().toString("MM/dd/yyyy");
  }
  report="                                                     Rivendell Log Listing\n";
  report+=QString().
    sprintf("Generated: %s                        Log: %-30s  Description: %s\n",
	    (const char *)QDateTime::currentDateTime().
	    toString("MM/dd/yyyy - hh:mm:ss").toUtf8(),
	    (const char *)edit_name_label->text().left(30).toUtf8(),
	    (const char *)edit_description_edit->text().left(27).toUtf8());
  report+=QString().
    sprintf("Service: %-10s          AutoRefresh Enabled: %-3s   Start Date: %-10s               End Date: %s\n",
	    (const char *)edit_service_box->currentText().toUtf8(),
	    (const char *)refresh.toUtf8(),
	    (const char *)start_date.toUtf8(),(const char *)end_date.toUtf8());
  report+="\n";
  report+="-Type-- -Time---- Trans -Cart- -Group---- -Length- -Title--------------------------- -Artist----------------------- -Source----- Line\n";

  //
  // Generate Event Listing
  //
  LogLine *logline;
  for(int i=0;i<(edit_log_model->rowCount()-1);i++) {
    logline=edit_log_model->logLine(i);

    //
    // Type
    //
    report+=QString().sprintf("%-7s ",
	       (const char *)LogLine::typeText(logline->type()).toUtf8());

    //
    // Time
    //
    if(logline->timeType()==LogLine::Hard) {
      report+="T";
    }
    else {
      report+=" ";
    }
    if(!logline->startTime().isNull()) {
      report+=QString().sprintf("%-8s ",
				 (const char *)logline->startTime().
				 toString("hh:mm:ss").toUtf8());
    }
    else {
      report+="         ";
    }

    //
    // Transition Type
    //
    report+=QString().sprintf("%-5s ",
      (const char *)LogLine::transText(logline->transType()).left(5).toUtf8());

    switch(logline->type()) {
    case LogLine::Cart:
    case LogLine::Macro:
      report+=QString().sprintf("%06u ",logline->cartNumber());
      report+=QString().sprintf("%-10s ",
				(const char *)logline->groupName().toUtf8());
      report+=QString().sprintf("%8s ",(const char *)QTime().addMSecs(logline->length(LogLine::CartPointer)).toString("h:mm:ss").toUtf8());
      report+=
	QString().sprintf("%-33s ",
			  (const char *)logline->title().left(33).toUtf8());
      report+=
	QString().sprintf("%-30s ",
			  (const char *)logline->artist().left(30).toUtf8());
      break;

    case LogLine::Marker:
    case LogLine::Track:
      report+="       ";
      report+="           ";
      report+="     :00 ";
      report+=
	QString().sprintf("%-30s ",
		    (const char *)logline->markerComment().left(30).toUtf8());
      report+="                               ";
      break;

    case LogLine::TrafficLink:
      report+="       ";
      report+="           ";
      report+="     :00 ";
      report+="Traffic Import                 ";
      report+="                               ";
      break;

    case LogLine::MusicLink:
      report+="       ";
      report+="           ";
      report+="     :00 ";
      report+="Music Import                 ";
      report+="                               ";
      break;

    case LogLine::Chain:
      report+="       ";
      report+="           ";
      report+="         ";
      report+=
	QString().sprintf("%-30s ",
		      (const char *)logline->markerLabel().left(30).toUtf8());
      report+="                               ";
      break;
      break;

    case LogLine::OpenBracket:
    case LogLine::CloseBracket:
    case LogLine::UnknownType:
      break;
    }

    //
    // Source
    //
    report+=QString().sprintf("%-12s ",
	      (const char *)LogLine::sourceText(logline->source()).toUtf8());

    //
    // Line
    //
    report+=QString().sprintf("%4d",i);

    //
    // End of Line
    //
    report+="\n";
  }
  TextReport(report);
}


void EditLogDialog::saveasData()
{
  QString svcname=edit_service_box->currentText();
  QString logname;
  struct rd_log *logs;
  unsigned log_quan;
  int err;
  QString err_str;

  if(edit_addlog_dialog->exec(&svcname,&logname)) {
    if((err=cnf->listLogs(&logs,&log_quan,"",logname,false))==0) {
      if(log_quan>0) {
	if(QMessageBox::question(this,"RDLogEdit - Save As",
				 tr("Log")+" \""+logname+"\" "+
				 tr("already exists!")+"\n"+
				 tr("Overwrite?"),
				 QMessageBox::Yes,QMessageBox::No)!=QMessageBox::Yes) {
	  return;
	}
      }
      else {
	if((err=cnf->addLog(logname,svcname))!=0) {
	  QMessageBox::warning(this,tr("RDLogEdit - Error"),
			       tr("Error in rd_addlog() call")+
			       " ["+tr("Error")+QString().sprintf(" %d].",err));
	}
      }
      edit_name_label->setText(logname);
      edit_service_box->setCurrentItemData(svcname);
      //      Save();
      edit_log_model->save(logname,&err_str);
    }
    else {
      QMessageBox::warning(this,tr("RDLogEdit - Error"),
			   tr("Error in rd_listlogs() call")+
			   " ["+tr("Error")+QString().sprintf(" %d].",err));
    }
  }
}


void EditLogDialog::okData()
{
  edit_stream_player->stop();
  Save();
  done(true);
}


void EditLogDialog::cancelData()
{
  if(edit_modified) {
    switch(QMessageBox::question(this,tr("RDLogEdit - Unsaved Changes"),
				 tr("This log contains unsaved changes.")+
				 "\n"+
				 tr("Do you want to save before exiting?"),
				 QMessageBox::Yes,QMessageBox::No,
				 QMessageBox::Cancel)) {
    case QMessageBox::Yes:
      okData();
      break;

    case QMessageBox::No:
      break;

    default:
      return;
    }
  }
  edit_stream_player->stop();
  done(false);
}


void EditLogDialog::capiErrorData(int err,const QString &err_msg)
{
  QMessageBox::warning(this,tr("RDLogEdit - C API Error"),
		       err_msg+QString().sprintf(" [Error: %d]",err));
}


void EditLogDialog::logChangedData()
{
  SetModified(true);
}


void EditLogDialog::closeEvent(QCloseEvent *e)
{
  cancelData();
}


void EditLogDialog::resizeEvent(QResizeEvent *e)
{
  edit_modified_label->setGeometry(60,12,20,18);

  edit_name_label_label->setGeometry(70,11,100,18);
  edit_name_label->setGeometry(175,11,size().width()-500,18);

  edit_tracks_label_label->setGeometry(size().width()-490,11,80,18);
  edit_tracks_label->setGeometry(size().width()-405,11,40,18);

  edit_origin_label_label->setGeometry(size().width()-335,11,50,18);
  edit_origin_label->setGeometry(size().width()-280,11,200,18);

  edit_description_label->setGeometry(10,40,95,20);
  edit_description_edit->setGeometry(110,40,size().width()-410,20);

  edit_logdelete_check->setGeometry(size().width()-275,42,15,15);
  edit_logdelete_label->setGeometry(size().width()-260,40,80,20);
  edit_logdelete_edit->setGeometry(size().width()-170,40,100,20);
  edit_logdelete_button->setGeometry(size().width()-60,37,50,26);

  edit_service_box->setGeometry(110,73,120,22);
  //  edit_service_edit->setGeometry(110,73,120,22);
  edit_service_label->setGeometry(10,73,95,22);

  edit_autorefresh_box->setGeometry(180,104,50,22);
  //  edit_autorefresh_edit->setGeometry(180,104,50,22);
  edit_autorefresh_label->setGeometry(10,104,165,22);

  edit_startdate_label->setGeometry(240,73,90,22);
  edit_startdate_edit->setGeometry(335,73,100,22);
  edit_startdate_check->setGeometry(250,108,15,15);
  edit_startdate_check_label->setGeometry(270,108,175,20);

  edit_enddate_label->setGeometry(435,73,90,22);
  edit_enddate_edit->setGeometry(530,73,100,22);
  edit_enddate_check->setGeometry(450,108,15,15);
  edit_enddate_check_label->setGeometry(470,106,175,20);

  edit_runlength_widget->setGeometry(640,68,250,85);

  edit_log_view->setGeometry(10,150,size().width()-20,size().height()-280);

  edit_insertcart_button->setGeometry(20,size().height()-125,80,50);
  edit_insertmeta_button->setGeometry(110,size().height()-125,80,50);
  edit_edit_button->setGeometry(200,size().height()-125,80,50);
  edit_delete_button->setGeometry(290,size().height()-125,80,50);

  edit_up_button->setGeometry(390,size().height()-125,50,50);
  edit_down_button->setGeometry(450,size().height()-125,50,50);

  edit_cut_button->setGeometry(size().width()-280,size().height()-125,80,50);
  edit_copy_button->setGeometry(size().width()-190,size().height()-125,80,50);
  edit_paste_button->setGeometry(size().width()-100,size().height()-125,80,50);

  edit_save_button->setGeometry(10,size().height()-60,80,50);
  edit_saveas_button->setGeometry(100,size().height()-60,80,50);

  edit_print_button->setGeometry(270,size().height()-60,80,50);

  edit_play_button->setGeometry(size().width()-390,size().height()-60,80,50);
  edit_stop_button->setGeometry(size().width()-300,size().height()-60,80,50);

  edit_ok_button->setGeometry(size().width()-180,size().height()-60,80,50);
  edit_cancel_button->setGeometry(size().width()-90,size().height()-60,80,50);
}


void EditLogDialog::paintEvent(QPaintEvent *e)
{
  QColor system_mid_color = palette().mid().color();
  QPainter *p=new QPainter(this);
  p->fillRect(60,8,size().width()-120,24,QColor(system_mid_color));
  p->fillRect(9,size().height()-130,size().width()-20,60,
	      QColor(system_mid_color));
  p->end();
  delete p;
}


void EditLogDialog::Save()
{
  QString err_msg;

  if(edit_modified) {
    edit_log_model->setServiceName(edit_service_box->currentText());
    edit_log_model->setDescription(edit_description_edit->text());
    edit_log_model->setAutorefresh(edit_autorefresh_box->currentIndex());
    if(edit_logdelete_check->isChecked()) {
      edit_log_model->setPurgeDate(edit_logdelete_edit->date());
    }
    else {
      edit_log_model->setPurgeDate(QDate());
    }
    if(edit_startdate_check->isChecked()) {
      edit_log_model->setStartDate(edit_startdate_edit->date());
    }
    else {
      edit_log_model->setStartDate(QDate());
    }
    if(edit_enddate_check->isChecked()) {
      edit_log_model->setEndDate(edit_enddate_edit->date());
    }
    else {
      edit_log_model->setEndDate(QDate());
    }
    if(edit_log_model->save(edit_name_label->text(),&err_msg)) {
      SetModified(false);
    }
    else {
      QMessageBox::critical(this,"RDLogEdit - Error",
			    tr("Unable to save log")+" ["+err_msg+"]");
      
    }
  }
}


QList<QTime> EditLogDialog::GetStartTimes(int except_row) const
{
  QList<QTime> start_times;
  for(int i=0;i<(edit_log_model->rowCount()-1);i++) {
    if((except_row!=i)&&
       (edit_log_model->logLine(i)->timeType()==LogLine::Hard)) {
      start_times.push_back(edit_log_model->logLine(i)->startTime());
    }
  }
  return start_times;
}


int EditLogDialog::GetNextId() const
{
  int id=1;

  for(int i=0;i<(edit_log_model->rowCount()-1);i++) {
    if(edit_log_model->logLine(i)->id()>=id) {
      id=edit_log_model->logLine(i)->id()+1;
    }
  }
  return id;
}


void EditLogDialog::SetModified(bool state)
{
  if(state!=edit_modified) {
    if(state) {
      edit_modified_label->show();
    }
    else {
      edit_modified_label->hide();
    }
    edit_save_button->setEnabled(state);
    edit_modified=state;
  }
}
