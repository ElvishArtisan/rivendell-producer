// streamplayer.cpp
//
// Stream player widget
//
//   (C) Copyright 2015-2016 Fred Gleason <fredg@paravelsystems.com>
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

#include <stdio.h>

#include <QMessageBox>
#include <QStringList>

#include "streamplayer.h"

StreamPlayer::StreamPlayer(QObject *parent)
  : QObject(parent)
{
  stream_process=NULL;
  stream_state=StreamPlayer::Stopped;
  stream_next_cartnum=0;
  stream_next_cutnum=-1;
}


StreamPlayer::~StreamPlayer()
{
}


StreamPlayer::State StreamPlayer::state() const
{
  return stream_state;
}


void StreamPlayer::play(int cartnum,int cutnum,int start_pos,int end_pos)
{
  if(stream_process==NULL) {
    QStringList args;
    args.push_back("--audio-device="+cnf->audioDeviceType());
    args.push_back("--alsa-device="+cnf->audioDeviceName());  // FIXME
    args.push_back(QString().sprintf("http://localhost/snd/%06u_%03d.wav",
				     cartnum,cutnum));
    stream_process=new QProcess(this);
    stream_process->setReadChannel(QProcess::StandardOutput);
    connect(stream_process,SIGNAL(stateChanged(QProcess::ProcessState)),
	    this,SLOT(processStateChangedData(QProcess::ProcessState)));
    connect(stream_process,SIGNAL(readyRead()),
	    this,SLOT(processReadyReadData()));
    connect(stream_process,SIGNAL(error(QProcess::ProcessError)),
	    this,SLOT(processErrorData(QProcess::ProcessError)));
    connect(stream_process,SIGNAL(finished(int,QProcess::ExitStatus)),
	    this,SLOT(processFinishedData(int,QProcess::ExitStatus)));
    stream_process->
      start("glassplayer",args,QIODevice::Unbuffered|QIODevice::ReadOnly);
    stream_next_cartnum=0;
    stream_next_cutnum=-1;
    stream_next_start_pos=start_pos;
    stream_next_end_pos=end_pos;
  }
  else {
    stream_next_cartnum=cartnum;
    stream_next_cutnum=cutnum;
    stop();
  }

  /*
  if(stream_process==NULL) {
    QString post=QString().sprintf("cart=%d&cut=%d",cartnum,cutnum);
    QStringList args;
    args.push_back("--audio-device="+cnf->audioDeviceType());
    args.push_back("--alsa-device="+cnf->audioDeviceName());  // FIXME
    args.push_back("--post-data="+post);
    args.push_back("--user="+cnf->serverUsername()+":"+cnf->serverPassword());
    args.push_back("http://"+cnf->serverHostname()+":8080/streamcut");
    stream_process=new QProcess(this);
    stream_process->setReadChannel(QProcess::StandardOutput);
    connect(stream_process,SIGNAL(stateChanged(QProcess::ProcessState)),
	    this,SLOT(processStateChangedData(QProcess::ProcessState)));
    connect(stream_process,SIGNAL(readyRead()),
	    this,SLOT(processReadyReadData()));
    connect(stream_process,SIGNAL(error(QProcess::ProcessError)),
	    this,SLOT(processErrorData(QProcess::ProcessError)));
    connect(stream_process,SIGNAL(finished(int,QProcess::ExitStatus)),
	    this,SLOT(processFinishedData(int,QProcess::ExitStatus)));
    stream_process->
      start("glassplayer",args,QIODevice::Unbuffered|QIODevice::ReadOnly);
    
  }
  */
}


void StreamPlayer::stop()
{
  if(stream_process!=NULL) {
    stream_process->terminate();
  }
}


void StreamPlayer::processStateChangedData(QProcess::ProcessState state)
{
  switch(state) {
  case QProcess::NotRunning:
    stream_state=StreamPlayer::Stopped;
    emit stateChanged(stream_state);
    break;

  case QProcess::Starting:
    stream_state=StreamPlayer::Playing;
    emit stateChanged(stream_state);
    break;

  case QProcess::Running:
    break;
  }
}


void StreamPlayer::processReadyReadData()
{
  QString line;
  QStringList f0;

  while(stream_process->canReadLine()) {
    line=stream_process->readLine().trimmed();
    /*
    if(line.isEmpty()) {
      ProcessStats(gui_stats_list);
      gui_stats_list.clear();
    }
    else {
      f0=line.split(" ");
      if(f0[0]=="ME") {
	if(f0.size()==2) {
	  ProcessMeterUpdates(f0[1]);
	}
      }
      else {
	gui_stats_list.push_back(line);
      }
    }
    */
  }
}


void StreamPlayer::processFinishedData(int exit_code,
				       QProcess::ExitStatus status)
{
  if(status!=QProcess::NormalExit) {
    QMessageBox::critical(NULL,tr("Rivendell Browser - Error"),
			  tr("Player process crashed!"));
    exit(256);
  }
  else {
    if(exit_code!=0) {
      QMessageBox::critical(NULL,tr("Rivendell Browser - Error"),
			    tr("Player process exited with non-zero exit code")+
			    QString().sprintf(" [%d]!",exit_code));
      exit(256);
    }
  }
  stream_process->deleteLater();
  stream_process=NULL;
  if(stream_next_cartnum>0) {
    play(stream_next_cartnum,stream_next_cutnum,
	 stream_next_start_pos,stream_next_end_pos);
  }
  else {
    stream_state=StreamPlayer::Stopped;
    emit stateChanged(stream_state);
  }
}


void StreamPlayer::processErrorData(QProcess::ProcessError err)
{
  QMessageBox::critical(NULL,tr("Rivendell Browser"),
			tr("Player process error")+" ["+
			QString().sprintf("%d",err)+"]!");
  exit(256);
}
