dnl This file has been shamelessly stolen and modified from gspeakers by Danial Sundberg
dnl http://gspeakers.sourceforge.net/

dnl Process this file with autoconf to produce a configure script.
AC_PREREQ(2.13)

AC_INIT(src/main.cpp)

AM_CONFIG_HEADER(config.h)

dnl -----------------------------------------------
dnl Package name and version number (user defined)
dnl -----------------------------------------------

PACKAGE=gimmage
VERSION=0.2.3

dnl ---
dnl default prefix
dnl ---

dnl -----------------------------------------------
dnl Package name and version number (DO NOT MODIFY)
dnl -----------------------------------------------

AM_INIT_AUTOMAKE($PACKAGE, $VERSION)

dnl Checks for programs.
AC_PROG_CXX

dnl Checks for libraries.
PKG_CHECK_MODULES(DEPS, gtkmm-2.4 >= 2.10.0 cairomm-1.0)
AC_SUBST(DEPS_CFLAGS)
AC_SUBST(DEPS_LIBS)


dnl List all available languages here
AM_GNU_GETTEXT([external])
AC_DEFINE_UNQUOTED(LOCALEDIR,["${prefix}/share/locale"],[localedir])


dnl configure option for building a debug build

gimmage_debug="no"
AC_ARG_ENABLE(debug,
    [  --enable-debug	build debugging version],
    [gimmage_debug="yes"])

if test "$gimmage_debug" = "yes"; then
    AC_DEFINE_UNQUOTED([DEBUG],[],[debug build])
    CXXFLAGS="-ggdb ${CXXFLAGS}"
fi

# Switch to C++ mode and check for needed C++ headers
AC_LANG_SAVE
AC_LANG_CPLUSPLUS

AC_CHECK_HEADERS([iostream list fstream string iterator],,
  AC_MSG_ERROR([You need to have the libstdc++ headers installed]))

AC_LANG_RESTORE

dnl Create makefiles
AC_OUTPUT(
	Makefile
	src/Makefile
	pixmaps/Makefile
	pixmaps/icon/Makefile
	po/Makefile.in
	gimmage.desktop
)
