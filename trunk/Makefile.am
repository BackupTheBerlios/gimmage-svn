AUTOMAKE_OPTIONS = gnu
SUBDIRS = src po pixmaps pixmaps/icon

# Desktop entry
desktopdir		= $(datadir)/applications
desktop_in_files	= gimmage.desktop.in
desktop_DATA 		= $(desktop_in_files:.desktop.in=.desktop)

EXTRA_DIST		= m4/ChangeLog  $(desktop_DATA)


ACLOCAL_AMFLAGS = -I m4
