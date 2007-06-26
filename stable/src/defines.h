// A set of global definitions for gimmmage

// we will use this to produce proper utf8 strings from gettext output
#define GT(x) Glib::locale_to_utf8(gettext(x))

// interpolation type, will later be read from config
#define INTERPTYPE Gdk::INTERP_BILINEAR

