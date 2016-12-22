// streamplayer_glass.cpp
//
// Stream player class for GlassPlayer
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

#include "streamplayer_glass.h"

StreamPlayerGlass::StreamPlayerGlass(Config *c,QObject *parent)
  : StreamPlayer(c,parent)
{
  stream_process=NULL;
}


StreamPlayerGlass::~StreamPlayerGlass()
{
}


void StreamPlayerGlass::startDevice(const QString &url,int start_pos,
				    int end_pos)
{
  QStringList args;
  args.push_back("--audio-device="+cnf->audioDeviceType());
  args.push_back("--alsa-device="+cnf->audioDeviceName());  // FIXME
  args.push_back(url);
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


void StreamPlayerGlass::stopDevice()
{
  if(stream_process!=NULL) {
    stream_process->terminate();
  }
}


void StreamPlayerGlass::processStateChangedData(QProcess::ProcessState state)
{
  switch(state) {
  case QProcess::NotRunning:
    //    setState(StreamPlayerGlass::Stopped);
    break;

  case QProcess::Starting:
    setState(StreamPlayerGlass::Playing);
    break;

  case QProcess::Running:
    break;
  }
}


void StreamPlayerGlass::processReadyReadData()
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


void StreamPlayerGlass::processFinishedData(int exit_code,
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
  setState(StreamPlayerGlass::Stopped);
}


void StreamPlayerGlass::processErrorData(QProcess::ProcessError err)
{
  QMessageBox::critical(NULL,tr("Rivendell Browser"),
			tr("Player process error")+" ["+
			QString().sprintf("%d",err)+"]!");
  exit(256);
}
