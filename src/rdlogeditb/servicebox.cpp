// servicebox.cpp
//
// ServiceBox widget
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

#include "config.h"
#include "servicebox.h"

ServiceBox::ServiceBox(bool incl_all,QWidget *parent)
  : ComboBox(parent)
{
  svc_include_all=incl_all;
}


int ServiceBox::reload()
{
  struct rd_service *services;
  unsigned records=0;
  int err=0;

  clear();
  if(svc_include_all) {
    insertItem(0,tr("ALL"),tr("ALL"));
  }
  if((err=cnf->listServices(&services,&records,false))!=0) {
    return err;
  }
  for(unsigned i=0;i<records;i++) {
    insertItem(count(),QString::fromUtf8(services[i].service_name),
	       QString::fromUtf8(services[i].service_name));
  }
  free(services);
  return 0;
}
