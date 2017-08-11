// lognamevalidator.h
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

#ifndef LOGNAMEVALIDATOR_H
#define LOGNAMEVALIDATOR_H

#include <QValidator>

class LogNameValidator : public QValidator
{
  Q_OBJECT;
 public:
  LogNameValidator(QObject *parent=0);
  QValidator::State validate(QString &input,int &pos) const;
};


#endif  // LOGNAMEVALIDATOR_H
