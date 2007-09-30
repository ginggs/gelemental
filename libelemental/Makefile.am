SUBDIRS = misc .

myincdir = $(includedir)/libelemental
AM_CPPFLAGS = -I$(top_srcdir) -DLOCALEDIR=\"$(localedir)\" \
	$(LIBELEMENTAL_CPPFLAGS)
AM_CXXFLAGS = $(LIBELEMENTAL_CXXFLAGS)

lib_LTLIBRARIES = libelemental.la
libelemental_la_LDFLAGS = -version-info 0:0:0

libelemental_la_SOURCES = \
	private.hh \
	value.cc \
	value-types.cc \
	data.hh data.cc \
	element.cc \
	properties.cc \
	table.cc

nobase_myinc_HEADERS = \
	misc/ucompose.hh misc/ucompose.tcc \
	misc/extras.hh \
	value.hh value.tcc \
	value-types.hh \
	element.hh element.tcc \
	properties.hh \
	table.hh

libelemental_la_LIBADD = $(LIBELEMENTAL_LIBS) \
	misc/libmisc.la
