// lognamevalidator.cpp
//
// A validator for Rivendell log names.
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

#include "lognamevalidator.h"

LogNameValidator::LogNameValidator(QObject *parent)
  : QValidator(parent)
{
}


QValidator::State LogNameValidator::validate(QString &input,int &pos) const
{
  if(input.contains(" ")||
     input.contains("\"")||
     input.contains("'")||
     input.contains("\\")||
     input.contains("`")) {
    return QValidator::Invalid;
  }
  return QValidator::Acceptable;
}
