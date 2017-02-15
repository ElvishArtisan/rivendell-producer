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

rm -f src/$DESTDIR/datetime.cpp
ln -s ../../src/common/datetime.cpp src/$DESTDIR/datetime.cpp
rm -f src/$DESTDIR/datetime.h
ln -s ../../src/common/datetime.h src/$DESTDIR/datetime.h

rm -f src/$DESTDIR/librarymodel.cpp
ln -s ../../src/common/librarymodel.cpp src/$DESTDIR/librarymodel.cpp
rm -f src/$DESTDIR/librarymodel.h
ln -s ../../src/common/librarymodel.h src/$DESTDIR/librarymodel.h

rm -f src/$DESTDIR/log.cpp
ln -s ../../src/common/log.cpp src/$DESTDIR/log.cpp
rm -f src/$DESTDIR/log.h
ln -s ../../src/common/log.h src/$DESTDIR/log.h

rm -f src/$DESTDIR/logline.cpp
ln -s ../../src/common/logline.cpp src/$DESTDIR/logline.cpp
rm -f src/$DESTDIR/logline.h
ln -s ../../src/common/logline.h src/$DESTDIR/logline.h

rm -f src/$DESTDIR/logmodel.cpp
ln -s ../../src/common/logmodel.cpp src/$DESTDIR/logmodel.cpp
rm -f src/$DESTDIR/logmodel.h
ln -s ../../src/common/logmodel.h src/$DESTDIR/logmodel.h

rm -f src/$DESTDIR/loglistmodel.cpp
ln -s ../../src/common/loglistmodel.cpp src/$DESTDIR/loglistmodel.cpp
rm -f src/$DESTDIR/loglistmodel.h
ln -s ../../src/common/loglistmodel.h src/$DESTDIR/loglistmodel.h

rm -f src/$DESTDIR/mpeg1decoder.cpp
ln -s ../../src/common/mpeg1decoder.cpp src/$DESTDIR/mpeg1decoder.cpp
rm -f src/$DESTDIR/mpeg1decoder.h
ln -s ../../src/common/mpeg1decoder.h src/$DESTDIR/mpeg1decoder.h

rm -f src/$DESTDIR/playerfactory.cpp
ln -s ../../src/common/playerfactory.cpp src/$DESTDIR/playerfactory.cpp
rm -f src/$DESTDIR/playerfactory.h
ln -s ../../src/common/playerfactory.h src/$DESTDIR/playerfactory.h

rm -f src/$DESTDIR/profile.cpp
ln -s ../../src/common/profile.cpp src/$DESTDIR/profile.cpp
rm -f src/$DESTDIR/profile.h
ln -s ../../src/common/profile.h src/$DESTDIR/profile.h

rm -f src/$DESTDIR/ringbuffer.cpp
ln -s ../../src/common/ringbuffer.cpp src/$DESTDIR/ringbuffer.cpp
rm -f src/$DESTDIR/ringbuffer.h
ln -s ../../src/common/ringbuffer.h src/$DESTDIR/ringbuffer.h

rm -f src/$DESTDIR/streamplayer.cpp
ln -s ../../src/common/streamplayer.cpp src/$DESTDIR/streamplayer.cpp
rm -f src/$DESTDIR/streamplayer.h
ln -s ../../src/common/streamplayer.h src/$DESTDIR/streamplayer.h

rm -f src/$DESTDIR/streamplayer_alsa.cpp
ln -s ../../src/common/streamplayer_alsa.cpp src/$DESTDIR/streamplayer_alsa.cpp
rm -f src/$DESTDIR/streamplayer_alsa.h
ln -s ../../src/common/streamplayer_alsa.h src/$DESTDIR/streamplayer_alsa.h

rm -f src/$DESTDIR/streamplayer_mme.cpp
ln -s ../../src/common/streamplayer_mme.cpp src/$DESTDIR/streamplayer_mme.cpp
rm -f src/$DESTDIR/streamplayer_mme.h
ln -s ../../src/common/streamplayer_mme.h src/$DESTDIR/streamplayer_mme.h

rm -f src/$DESTDIR/tableview.cpp
ln -s ../../src/common/tableview.cpp src/$DESTDIR/tableview.cpp
rm -f src/$DESTDIR/tableview.h
ln -s ../../src/common/tableview.h src/$DESTDIR/tableview.h

rm -f src/$DESTDIR/transportbutton.cpp
ln -s ../../src/common/transportbutton.cpp src/$DESTDIR/transportbutton.cpp
rm -f src/$DESTDIR/transportbutton.h
ln -s ../../src/common/transportbutton.h src/$DESTDIR/transportbutton.h

rm -f src/$DESTDIR/*.ico
ln -s ../../icons/rdadmin.ico src/$DESTDIR/rdadmin.ico
ln -s ../../icons/rdlibrary.ico src/$DESTDIR/rdlibrary.ico
ln -s ../../icons/rdlogedit.ico src/$DESTDIR/rdlogedit.ico
