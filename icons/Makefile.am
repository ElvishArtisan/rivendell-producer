## automake.am
##
## icons/automake.am for Rivendell-Producer
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
	cp rdadminb-16x16.png $(DESTDIR)@prefix@/share/icons/hicolor/16x16/apps/rdadminb.png
	cp rdadminb-48x48.png $(DESTDIR)@prefix@/share/icons/hicolor/48x48/apps/rdadminb.png
	cp rdlibraryb-16x16.png $(DESTDIR)@prefix@/share/icons/hicolor/16x16/apps/rdlibraryb.png
	cp rdlibraryb-48x48.png $(DESTDIR)@prefix@/share/icons/hicolor/48x48/apps/rdlibraryb.png
	cp rdlogeditb-16x16.png $(DESTDIR)@prefix@/share/icons/hicolor/16x16/apps/rdlogeditb.png
	cp rdlogeditb-48x48.png $(DESTDIR)@prefix@/share/icons/hicolor/48x48/apps/rdlogeditb.png


uninstall-local:	
	rm -f $(DESTDIR)@prefix@/share/icons/hicolor/16x16/apps/rdadminb.png
	rm -f $(DESTDIR)@prefix@/share/icons/hicolor/48x48/apps/rdadminb.png
	rm -f $(DESTDIR)@prefix@/share/icons/hicolor/16x16/apps/rdlibraryb.png
	rm -f $(DESTDIR)@prefix@/share/icons/hicolor/48x48/apps/rdlibraryb.png
	rm -f $(DESTDIR)@prefix@/share/icons/hicolor/16x16/apps/rdlogeditb.png
	rm -f $(DESTDIR)@prefix@/share/icons/hicolor/48x48/apps/rdlogeditb.png


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
             rdadminb.ico\
             rdadminb-16x16.png\
             rdadminb-16x16.xpm\
             rdadminb-48x48.png\
             rdlibraryb.ico\
             rdlibraryb-16x16.png\
             rdlibraryb-16x16.xpm\
             rdlibraryb-48x48.png\
             rdlogeditb.ico\
             rdlogeditb-16x16.png\
             rdlogeditb-16x16.xpm\
             rdlogeditb-48x48.png\
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
