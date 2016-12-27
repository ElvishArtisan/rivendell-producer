// playerfactory.cpp
//
// Instantiate a stream player
//
// (C) Copyright 2016 Fred Gleason <fredg@paravelsystems.com>
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

#include "playerfactory.h"
#include "streamplayer_alsa.h"
#include "streamplayer_mme.h"

StreamPlayer *PlayerFactory(Config *c,QObject *parent)
{
#ifdef WIN32
  return new StreamPlayerMme(c,parent);
#else
  return new StreamPlayerAlsa(c,parent);
#endif  // WIN32
}
