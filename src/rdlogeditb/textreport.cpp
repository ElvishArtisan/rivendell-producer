// textreport.cpp
//
// Spawn an external text file viewer.
//
//   (C) Copyright 2002-2006,2016-2017 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef WIN32
#include <unistd.h>
#endif  // WIN32
#include <stdlib.h>
#include <stdio.h>

#include <QMessageBox>
#include <QProcess>

#include "textreport.h"

bool TextReport(const QString &data)
{
  char tmpfile[256];
  QString editor;

  if(getenv("VISUAL")==NULL) {
#ifdef WIN32
    editor="wordpad";
#else
    editor="xterm -e vi";
#endif  // WIN32
  }
  else {
    editor=getenv("VISUAL");
  }
#ifdef WIN32
  QString tempfile=QString().sprintf("%s\\rd-%s",(const char *)RDTempDir(),
	           (const char *)QTime::currentTime().toString("hhmmsszzz"));
  FILE *f=fopen(tempfile,"w");
  if(f==NULL) {
    QMessageBox::warning(NULL,"File Error","Unable to create temporary file");
    return false;
  }
  fprintf(f,"%s",(const char *)data);
  fclose(f);
  QStringList args;
  args+=editor;
  args+=tempfile;
  QProcess *proc=new QProcess(args);
  proc->launch("");
  delete proc;
#else
  strcpy(tmpfile,"/tmp/rdreportXXXXXX");
  int fd=mkstemp(tmpfile);
  if(fd<0) {
    QMessageBox::warning(NULL,"File Error","Unable to create temporary file");
    return false;
  }
  else {
    write(fd,data.toUtf8(),data.toUtf8().length());
    ::close(fd);
    if(fork()==0) {
      system((editor+" "+tmpfile).toUtf8());
      unlink(tmpfile);
      exit(0);
    }
  }
#endif  // WIN32
  return true;
}
