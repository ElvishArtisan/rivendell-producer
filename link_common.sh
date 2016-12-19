#!/bin/sh

# link_common.sh
#
#  Link common sources for rivendell-browser
#
#   (C) Copyright 2016 Fred Gleason <fredg@paravelsystems.com>
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License version 2 as
#   published by the Free Software Foundation.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public
#   License along with this program; if not, write to the Free Software
#   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#

DESTDIR=$1

rm -f src/$DESTDIR/cmdswitch.cpp
ln -s ../../src/common/cmdswitch.cpp src/$DESTDIR/cmdswitch.cpp
rm -f src/$DESTDIR/cmdswitch.h
ln -s ../../src/common/cmdswitch.h src/$DESTDIR/cmdswitch.h

rm -f src/$DESTDIR/combobox.cpp
ln -s ../../src/common/combobox.cpp src/$DESTDIR/combobox.cpp
rm -f src/$DESTDIR/combobox.h
ln -s ../../src/common/combobox.h src/$DESTDIR/combobox.h

rm -f src/$DESTDIR/config.cpp
ln -s ../../src/common/config.cpp src/$DESTDIR/config.cpp
rm -f src/$DESTDIR/config.h
ln -s ../../src/common/config.h src/$DESTDIR/config.h

rm -f src/$DESTDIR/profile.cpp
ln -s ../../src/common/profile.cpp src/$DESTDIR/profile.cpp
rm -f src/$DESTDIR/profile.h
ln -s ../../src/common/profile.h src/$DESTDIR/profile.h

rm -f src/$DESTDIR/streamplayer.cpp
ln -s ../../src/common/streamplayer.cpp src/$DESTDIR/streamplayer.cpp
rm -f src/$DESTDIR/streamplayer.h
ln -s ../../src/common/streamplayer.h src/$DESTDIR/streamplayer.h

rm -f src/$DESTDIR/tableview.cpp
ln -s ../../src/common/tableview.cpp src/$DESTDIR/tableview.cpp
rm -f src/$DESTDIR/tableview.h
ln -s ../../src/common/tableview.h src/$DESTDIR/tableview.h

rm -f src/$DESTDIR/transportbutton.cpp
ln -s ../../src/common/transportbutton.cpp src/$DESTDIR/transportbutton.cpp
rm -f src/$DESTDIR/transportbutton.h
ln -s ../../src/common/transportbutton.h src/$DESTDIR/transportbutton.h

rm -f src/$DESTDIR/*.ico
ln -s ../../icons/rdlibrary.ico src/$DESTDIR/rdlibrary.ico
