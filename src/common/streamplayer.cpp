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

  //
  // Garbage Collector
  //
  stream_garbage_timer=new QTimer(this);
  stream_garbage_timer->setSingleShot(true);
  connect(stream_garbage_timer,SIGNAL(timeout()),this,SLOT(garbageData()));
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
    QString post=QString("COMMAND=1&")+
      "LOGIN_NAME="+cnf->serverUsername()+"&"+
      "PASSWORD="+cnf->serverPassword()+"&"+
      QString().sprintf("CART_NUMBER=%d&",cartnum)+
      QString().sprintf("CUT_NUMBER=%d&",cutnum)+
      QString().sprintf("FORMAT=3&")+
      QString().sprintf("CHANNELS=%u&",cnf->audioChannels())+
      QString().sprintf("SAMPLE_RATE=%u&",cnf->audioSampleRate())+
      QString().sprintf("BIT_RATE=%u&",cnf->audioBitRate())+
      "QUALITY=0&"+
      QString().sprintf("START_POINT=%d&",start_pos)+
      QString().sprintf("END_POINT=%d&",end_pos)+
      "NORMALIZATION_LEVEL=0&"+
      "ENABLE_METADATA=0";

    QStringList args;
    args.push_back("--audio-device="+cnf->audioDeviceType());
    args.push_back("--alsa-device="+cnf->audioDeviceName());  // FIXME
    args.push_back("--post-data="+post);
    args.push_back("http://"+cnf->serverHostname()+"/rd-bin/rdxport.cgi");
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
  stream_garbage_timer->start(1);
  stream_state=StreamPlayer::Stopped;
  emit stateChanged(stream_state);
}


void StreamPlayer::processErrorData(QProcess::ProcessError err)
{
  QMessageBox::critical(NULL,tr("Rivendell Browser"),
			tr("Player process error")+" ["+
			QString().sprintf("%d",err)+"]!");
  exit(256);
}


void StreamPlayer::garbageData()
{
  delete stream_process;
  stream_process=NULL;
}
