## automake.am
##
## Makefile for rdlogeditb(1)
##
## (C) Copyright 2016-2017 Fred Gleason <fredg@paravelsystems.com>
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

AM_CPPFLAGS = -Wall -DPREFIX=\"$(prefix)\" -Wno-strict-aliasing @QT4_CFLAGS@ @RIVCAPI_CFLAGS@ @CURL_CFLAGS@ @SRC_CFLAGS@ @ALSA_CFLAGS@
MOC = @QT4_MOC@

# The dependency for qt's Meta Object Compiler (moc)
moc_%.cpp:	%.h
	@MOC@ $< -o $@

# The dependency for Windows resource compiler
%.o:	%.rc
	$(WINDRES) -O coff $< $@

bin_PROGRAMS = rdlogeditb

dist_rdlogeditb_SOURCES = addlogdialog.cpp addlogdialog.h\
                          datedialog.cpp datedialog.h\
                          datepicker.cpp datepicker.h\
                          editlogdialog.cpp editlogdialog.h\
                          editloglinedialog.cpp editloglinedialog.h\
                          log.cpp log.h\
                          logline.cpp logline.h\
                          loglistmodel.cpp loglistmodel.h\
                          rdlogeditb.cpp rdlogeditb.h

nodist_rdlogeditb_SOURCES = cmdswitch.cpp cmdswitch.h\
                            combobox.cpp combobox.h\
                            config.cpp config.h\
                            datetime.cpp datetime.h\
                            librarymodel.cpp librarymodel.h\
                            logmodel.cpp logmodel.h\
                            moc_addlogdialog.cpp\
                            moc_combobox.cpp\
                            moc_datedialog.cpp\
                            moc_datepicker.cpp\
                            moc_editlogdialog.cpp\
                            moc_editloglinedialog.cpp\
                            moc_librarymodel.cpp\
                            moc_loglistmodel.cpp\
                            moc_logmodel.cpp\
                            moc_rdlogeditb.cpp\
                            moc_streamplayer.cpp\
                            moc_streamplayer_alsa.cpp\
                            moc_streamplayer_mme.cpp\
                            moc_tableview.cpp\
                            moc_transportbutton.cpp\
                            mpeg1decoder.cpp mpeg1decoder.h\
                            playerfactory.cpp playerfactory.h\
                            profile.cpp profile.h\
                            ringbuffer.cpp ringbuffer.h\
                            streamplayer.cpp streamplayer.h\
                            streamplayer_alsa.cpp streamplayer_alsa.h\
                            streamplayer_mme.cpp streamplayer_mme.h\
                            tableview.cpp tableview.h\
                            transportbutton.cpp transportbutton.h

EXTRA_rdlogeditb_DEPENDENCIES = @RESOURCES@

rdlogeditb_LDADD = @QT4_LIBS@ @RIVCAPI_LIBS@ @CURL_LIBS@ @SRC_LIBS@ @MAD_LIBS@ @ALSA_LIBS@ @MME_LIBS@ -lexpat @RESOURCES@ -lpthread
rdlogeditb_LDFLAGS = @LDFLAGS@

EXTRA_DIST = icons.rc

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
                 datetime.cpp datetime.h\
                 librarymodel.cpp librarymodel.h\
                 mpeg1decoder.cpp mpeg1decoder.h\
                 playerfactory.cpp playerfactory.h\
                 profile.cpp profile.h\
                 ringbuffer.cpp ringbuffer.h\
                 streamplayer.cpp streamplayer.h\
                 streamplayer_alsa.cpp streamplayer_alsa.h\
                 streamplayer_mme.cpp streamplayer_mme.h\
                 tableview.cpp tableview.h\
                 transportbutton.cpp transportbutton.h

MAINTAINERCLEANFILES = *~\
                       Makefile.in
