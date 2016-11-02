// cmdswitch.cpp
//
// Process Command-Line Switches
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

#include <stdlib.h>
#include <stdio.h>

#include <QApplication>  // So we get 'qApp'
#include <QCoreApplication>

#include "cmdswitch.h"

CmdSwitch::CmdSwitch(int argc,char *argv[],const char *modname,
		     const char *modver,const char *usage)
{
  QStringList args;

  for(int i=0;i<argc;i++) {
    args.push_back(argv[i]);
  }
  Initialize(args,modname,modver,usage);
}


CmdSwitch::CmdSwitch(const char *modname,const char *modver,
		     const char *usage)
{
  Initialize(qApp->arguments(),modname,modver,usage);
}


unsigned CmdSwitch::keys() const
{
  return switch_keys.size();
}


QString CmdSwitch::key(unsigned n) const
{
  return switch_keys[n];
}


QString CmdSwitch::value(unsigned n) const
{
  return switch_values[n];
}


bool CmdSwitch::processed(unsigned n) const
{
  return switch_processed[n];
}


void CmdSwitch::setProcessed(unsigned n,bool state)
{
  switch_processed[n]=state;
}


bool CmdSwitch::allProcessed() const
{
  for(unsigned i=0;i<switch_processed.size();i++) {
    if(!switch_processed[i]) {
      return false;
    }
  }
  return true;
}


void CmdSwitch::Initialize(const QStringList &args,const char *modname,
			     const char *modver,const char *usage)
{
  unsigned l=0;
  bool handled=false;

  for(int i=1;i<args.size();i++) {
#ifndef WIN32
    if(args[i]=="--version") {
      printf("%s v%s\n",modname,modver);
      exit(0);
    }
#endif  // WIN32
    if(args[i]=="--help") {
      printf("\n%s %s\n",modname,usage);
      exit(0);
    }
    l=args[i].length();
    handled=false;
    for(unsigned j=0;j<l;j++) {
      if(args[i][j]=='=') {
	switch_keys.push_back(QString(args[i]).left(j));
	switch_values.push_back(QString(args[i]).right(l-(j+1)));
	switch_processed.push_back(false);
	j=l;
	handled=true;
      }
    }
    if(!handled) {
      switch_keys.push_back(QString(args[i]));
      switch_values.push_back(QString(""));
      switch_processed.push_back(false);
    }
  }
}
