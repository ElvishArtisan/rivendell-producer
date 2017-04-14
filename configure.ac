dnl configure.ac
dnl
dnl Autoconf configuration
dnl
dnl Use autoconf to process this into a configure script
dnl
dnl   (C) Copyright 2016 Fred Gleason <fredg@paravelsystems.com>
dnl
dnl   This program is free software; you can redistribute it and/or modify
dnl   it under the terms of the GNU General Public License version 2 as
dnl   published by the Free Software Foundation.
dnl
dnl   This program is distributed in the hope that it will be useful,
dnl   but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl   GNU General Public License for more details.
dnl
dnl   You should have received a copy of the GNU General Public
dnl   License along with this program; if not, write to the Free Software
dnl   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
dnl

AC_INIT(rivendell-producer,m4_esyscmd(cat PACKAGE_VERSION))
AM_INIT_AUTOMAKE
AC_SUBST(RPM_RELEASE,1)
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

#
# Basic Compiler Checks
#
AC_PROG_CXX
AC_LANG(C++)

#
# Check for Qt4 (Mandatory)
#
PKG_CHECK_MODULES(QT4,QtGui QtCore QtNetwork Qt3Support QtCLucene QtDBus QtDeclarative QtHelp QtMultimedia QtOpenGL QtScript QtScriptTools QtSql QtSvg QtTest QtUiTools QtWebKit QtXml QtXmlPatterns,[],[AC_MSG_ERROR([*** Qt4 not found ***])])
AC_CHECK_PROG(MOC_NAME,moc-qt4,[moc-qt4],[moc])
AC_SUBST(MOC,$MOC_NAME)

#
# Check for Rivendell C API (Mandatory)
#
PKG_CHECK_MODULES(RIVCAPI,rivendell,[],[AC_MSG_ERROR([*** Rivendell C API not found ***])])

#
# Check for Secret Rabbit Code
#
PKG_CHECK_MODULES(SRC,samplerate,[],[AC_MSG_ERROR([*** libsamplerate not found ***])])

#
# Check for LibCurl
#
PKG_CHECK_MODULES(CURL,libcurl,[],[AC_MSG_ERROR([*** LibCurl not found ***])])

#
# Check for MPEG Audio Decoder
#
PKG_CHECK_MODULES(MAD,mad,[],[AC_MSG_ERROR([*** MAD not found ***])])

#
# Check for ALSA
#
PKG_CHECK_MODULES(ALSA,alsa,[ALSA_FOUND=y],[PKGCONFIG_FAILED=y])
if test $ALSA_FOUND ; then
  if test -z $ALSA_DISABLED ; then
    AC_DEFINE(ALSA,yes)
    USING_ALSA=yes
  fi
fi

#
# Check for Windows MME
#
AC_CHECK_HEADER(windows.h,[MME_FOUND=yes],[])
if test $MME_FOUND ; then
  if test -z $MME_DISABLED ; then
    AC_DEFINE(MME)
    AC_SUBST(MME_LIBS,-lwinmm)
    USING_MME=yes
  else
    AC_SUBST(MME_LIBS,"")
  fi
else
  AC_SUBST(MME_LIBS,"")
fi

#
# Determine the target architecture
#
AR_GCC_TARGET()
AC_SUBST(VENDOR,$ar_gcc_distro)
AC_SUBST(ARCH,$ar_gcc_arch)
if test $ar_gcc_os = mingw32 ; then
  AC_DEFINE(WIN32)
  AC_SUBST(RESOURCES,"icons.o")
  AC_SUBST(LDFLAGS,"-mwindows")
  AC_DEFINE(MINGW32,"yes")
else
  AC_SUBST(RESOURCES,"")
  AC_SUBST(LDFLAGS,"")
fi

#
# Determine Distro
#
AR_GET_DISTRO()
AC_SUBST(DISTRO,$ar_gcc_distro)

#
# Configure RPM Build
#
AC_CHECK_PROG(RPMBUILD_FOUND,rpmbuild,[yes],[])
if test -z $RPMBUILD_FOUND ; then
  AC_SUBST(RPMBUILD,rpm)
else
  AC_SUBST(RPMBUILD,rpmbuild)
fi
if test -d /usr/src/redhat ; then
  AC_SUBST(RPM_ROOT,/usr/src/redhat)
  AC_SUBST(VENDOR,redhat)
else
  AC_SUBST(RPM_ROOT,/usr/src/packages)
  AC_SUBST(VENDOR,suse)
fi 

AC_CONFIG_FILES([ \
    conf/Makefile \
    icons/Makefile \
    src/Makefile \
    src/common/Makefile \
    src/rdadminb/Makefile \
    src/rdlibraryb/Makefile \
    src/rdlogeditb/Makefile \
    xdg/Makefile \
    Makefile \
    rivendell-producer.nsi \
    rivendell-producer.spec \
])
AC_OUTPUT()

#
# Link Common Elements
#
./link_common.sh rdadminb
./link_common.sh rdlibraryb
./link_common.sh rdlogeditb

AC_MSG_NOTICE()
AC_MSG_NOTICE("|-----------------------------------------------------|")
AC_MSG_NOTICE("|   *** RIVENDELL-PRODUCER CONFIGURATION SUMMARY ***  |")
AC_MSG_NOTICE("|-----------------------------------------------------|")
AC_MSG_NOTICE("| Audio Outputs:                                      |")
if test -z $USING_ALSA ; then
AC_MSG_NOTICE("|  Advanced Linux Sound Architecture (ALSA) ... No    |")
else
AC_MSG_NOTICE("|  Advanced Linux Sound Architecture (ALSA) ... Yes   |")
fi
if test -z $USING_MME ; then
AC_MSG_NOTICE("|       Windows Multimedia Extensions (MME) ... No    |")
else
AC_MSG_NOTICE("|       Windows Multimedia Extensions (MME) ... Yes   |")
fi
AC_MSG_NOTICE("|-----------------------------------------------------|")
AC_MSG_NOTICE()
AC_MSG_NOTICE()
AC_MSG_NOTICE(Now enter 'make' to build the software.)
AC_MSG_NOTICE()
