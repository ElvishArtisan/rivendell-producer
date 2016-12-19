// streamplayer.h
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

#ifndef STREAMPLAYER_H
#define STREAMPLAYER_H

#include <QObject>
#include <QProcess>
#include <QTimer>

#include "config.h"

class StreamPlayer : public QObject
{
 Q_OBJECT;
 public:
  enum State {Stopped=0,Playing=1};
  StreamPlayer(QObject *parent=0);
  ~StreamPlayer();
  State state() const;

 signals:
  void stateChanged(StreamPlayer::State state);

 public slots:
  void play(int cartnum,int cutnum,int start_pos,int end_pos);
  void stop();

 private slots:
  void processStateChangedData(QProcess::ProcessState state);
  void processReadyReadData();
  void processFinishedData(int exit_code,QProcess::ExitStatus status);
  void processErrorData(QProcess::ProcessError err);

 private:
  QProcess *stream_process;
  State stream_state;
  unsigned stream_next_cartnum;
  int stream_next_cutnum;
  int stream_next_start_pos;
  int stream_next_end_pos;
};


#endif  // STREAMPLAYER_H
