## automake.am
##
## icons/automake.am for Rivendell-Browser
##
## (C) Copyright 2002-2006,2016 Fred Gleason <fredg@salemradiolabs.com>
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
##
## Use automake to process this into a Makefile.in

install-exec-am:	
	mkdir -p $(DESTDIR)@prefix@/share/icons/hicolor/16x16/apps
	mkdir -p $(DESTDIR)@prefix@/share/icons/hicolor/48x48/apps
	cp rdadmin-16x16.png $(DESTDIR)@prefix@/share/icons/hicolor/16x16/apps/rdadmin.png
	cp rdadmin-48x48.png $(DESTDIR)@prefix@/share/icons/hicolor/48x48/apps/rdadmin.png
	cp rdlibrary-16x16.png $(DESTDIR)@prefix@/share/icons/hicolor/16x16/apps/rdlibrary.png
	cp rdlibrary-48x48.png $(DESTDIR)@prefix@/share/icons/hicolor/48x48/apps/rdlibrary.png
	cp rdlogedit-16x16.png $(DESTDIR)@prefix@/share/icons/hicolor/16x16/apps/rdlogedit.png
	cp rdlogedit-48x48.png $(DESTDIR)@prefix@/share/icons/hicolor/48x48/apps/rdlogedit.png


uninstall-local:	
	rm -f $(DESTDIR)@prefix@/share/icons/hicolor/16x16/apps/rdadmin.png
	rm -f $(DESTDIR)@prefix@/share/icons/hicolor/48x48/apps/rdadmin.png
	rm -f $(DESTDIR)@prefix@/share/icons/hicolor/16x16/apps/rdlibrary.png
	rm -f $(DESTDIR)@prefix@/share/icons/hicolor/48x48/apps/rdlibrary.png
	rm -f $(DESTDIR)@prefix@/share/icons/hicolor/16x16/apps/rdlogedit.png
	rm -f $(DESTDIR)@prefix@/share/icons/hicolor/48x48/apps/rdlogedit.png


EXTRA_DIST = audiocart.xpm\
             audio.xpm\
             blueball.xpm\
             chain.xpm\
             greenball.xpm\
             greencheckmark.xpm\
             macrocart.xpm\
             macro.xpm\
             marker.xpm\
             musiclink.xpm\
             rdadmin.ico\
             rdadmin-16x16.png\
             rdadmin-16x16.xpm\
             rdadmin-48x48.png\
             rdlibrary.ico\
             rdlibrary-16x16.png\
             rdlibrary-16x16.xpm\
             rdlibrary-48x48.png\
             rdlogedit.ico\
             rdlogedit-16x16.png\
             rdlogedit-16x16.xpm\
             rdlogedit-48x48.png\
             redball.xpm\
             redx.xpm\
             trackcart.xpm\
             trackmarker.xpm\
             trafficlink.xpm\
             whiteball.xpm

CLEANFILES = *~

MAINTAINERCLEANFILES = *~\
                       aclocal.m4\
                       configure\
                       Makefile.in
