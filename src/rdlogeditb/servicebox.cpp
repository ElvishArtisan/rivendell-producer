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

ServiceBox::ServiceBox(QWidget *parent)
  : ComboBox(parent)
{
}


int ServiceBox::reload()
{
  struct rd_service *services;
  unsigned records=0;
  int err=0;

  clear();
  insertItem(0,tr("ALL"),tr("ALL"));
  if((err=cnf->listServices(&services,&records,false))!=0) {
    return err;
  }
  for(unsigned i=0;i<records;i++) {
    insertItem(count(),services[i].service_name,services[i].service_name);
  }
  free(services);
  return 0;
}
