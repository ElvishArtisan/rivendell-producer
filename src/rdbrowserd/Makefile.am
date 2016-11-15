## automake.am
##
## Makefile for rdbrowserd(8)
##
## (C) Copyright 2016 Fred Gleason <fredg@paravelsystems.com>
##
##   This program is free software; you can redistribute it and/or modify
##   it under the terms of the GNU General Public License version 2 as
##   published by the Free Software Foundation.
##
##   This program is distributed in the hope that it will be useful,
##   but WITHOUT ANY WARRANTY; without even the implied warranty of
##   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##   GNU General Public License for more details.
##
##   You should have received a copy of the GNU General Public
##   License along with this program; if not, write to the Free Software
##   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
##
## Use automake to process this into a Makefile.in

AM_CPPFLAGS = -Wall -DPREFIX=\"$(prefix)\" -Wno-strict-aliasing @QT4_CFLAGS@ @WEBHOST_CFLAGS@
MOC = @QT4_MOC@

# The dependency for qt's Meta Object Compiler (moc)
moc_%.cpp:	%.h
	@MOC@ $< -o $@

# The dependency for Windows resource compiler
%.o:	%.rc
	$(WINDRES) -O coff $< $@

sbin_PROGRAMS = rdbrowserd

dist_rdbrowserd_SOURCES = db.cpp db.h\
                          httpserver.cpp httpserver.h\
                          rdbrowserd.cpp rdbrowserd.h

nodist_rdbrowserd_SOURCES = cmdswitch.cpp cmdswitch.h\
                            config.cpp config.h\
                            moc_db.cpp\
                            moc_httpserver.cpp\
                            moc_rdbrowserd.cpp\
                            profile.cpp profile.h

EXTRA_rdbrowserd_DEPENDENCIES = @RESOURCES@

rdbrowserd_LDADD = @QT4_LIBS@ @WEBHOST_LIBS@ @RESOURCES@
rdbrowserd_LDFLAGS = @LDFLAGS@

CLEANFILES = *~\
             moc_*\
             *.obj\
             *.idb\
             *.pdb\
             *ilk

DISTCLEANFILES = *.ico\
                 cmdswitch.cpp cmdswitch.h\
                 combobox.cpp combobox.h\
                 config.cpp config.h\
                 profile.cpp profile.h\
                 streamplayer.cpp streamplayer.h\
                 tableview.cpp tableview.h\
                 transportbutton.cpp transportbutton.h

MAINTAINERCLEANFILES = *~\
                       Makefile.in
