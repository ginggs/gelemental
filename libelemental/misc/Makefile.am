AM_CPPFLAGS = $(LIBMISC_CPPFLAGS) -DG_LOG_DOMAIN=\"libmisc\"
AM_CXXFLAGS = $(LIBMISC_CXXFLAGS)

noinst_LTLIBRARIES = libmisc.la

libmisc_la_SOURCES = \
	macros.hh \
	compose.hh compose.tcc compose.cc \
	ucompose.hh ucompose.tcc ucompose.cc \
	extras.hh extras.cc \
	throttle.hh throttle.cc
libmisc_la_LIBADD = $(LIBMISC_LIBS)

if LIBMISC_GTK
noinst_LTLIBRARIES += libmisc-gtk.la
libmisc_gtk_la_SOURCES = widgets.hh widgets.cc
endif
