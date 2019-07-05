dnl Process this file with autoconf to produce a configure script.

AC_PREREQ([2.59])

AC_INIT([gElemental], [1.2.0], [], [gelemental])
AC_CONFIG_SRCDIR([src/main.cc])

AM_INIT_AUTOMAKE([1.9 dist-bzip2 no-dist-gzip])
AM_MAINTAINER_MODE

dnl ensure that when the Automake generated makefile calls aclocal,
dnl it honours the $ACLOCAL_FLAGS environment variable
ACLOCAL_AMFLAGS="\${ACLOCAL_FLAGS}"
AC_SUBST([ACLOCAL_AMFLAGS])

dnl ****************************************************************************
dnl basic requirements
dnl ****************************************************************************

AC_PROG_CXX
AC_PROG_LIBTOOL
AC_PROG_INSTALL

PKG_CHECK_MODULES([LIBELEMENTAL], [glibmm-2.4 >= 2.6.0 pango])
PKG_CHECK_MODULES([GELEMENTAL], [gtkmm-2.4 >= 2.6.0])

dnl ****************************************************************************
dnl gettext and intltool
dnl ****************************************************************************

IT_PROG_INTLTOOL([0.35.0])

GETTEXT_PACKAGE=gelemental
AC_SUBST([GETTEXT_PACKAGE])
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], ["$GETTEXT_PACKAGE"], [Gettext package])

AM_GLIB_GNU_GETTEXT

dnl ****************************************************************************
dnl maintenance and debugging
dnl ****************************************************************************

AC_MSG_CHECKING([whether to enable debugging support])
AC_ARG_ENABLE([debug],
	AS_HELP_STRING([--enable-debug], [enable debugging support]),
	[], [enable_debug=no])
AC_MSG_RESULT([$enable_debug])

if test "x$USE_MAINTAINER_MODE" = "xyes"; then
	DISABLE_DEPRECATED="-DGDKMM_DISABLE_DEPRECATED -DGTKMM_DISABLE_DEPRECATED -DG_DISABLE_DEPRECATED -DGDK_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED -DPANGO_DISABLE_DEPRECATED"
else
	DISABLE_DEPRECATED=""
fi
AC_SUBST([DISABLE_DEPRECATED])

MAINT_CXXFLAGS=""

if test "x$GXX" = "xyes"; then
	if test "x$USE_MAINTAINER_MODE" = "xyes"; then
		MAINT_CXXFLAGS="-Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-qual -Wwrite-strings -Wunreachable-code -Woverloaded-virtual -Wsign-promo"
	else
		MAINT_CXXFLAGS="-Wall -Wno-unused"
	fi
fi

if test "x$enable_debug" = "xyes"; then
	AC_DEFINE([ENABLE_DEBUG], [1], [Define if debugging support is enabled])
	if test "x$GXX" = "xyes"; then
		MAINT_CXXFLAGS="$MAINT_CXXFLAGS -ggdb3"
	else
		MAINT_CXXFLAGS="$MAINT_CXXFLAGS -g"
	fi
fi

AC_SUBST([MAINT_CXXFLAGS])

dnl ****************************************************************************
dnl optional features
dnl ****************************************************************************

AC_CHECK_PROG([have_doxygen], [doxygen], [yes], [no])
AC_MSG_CHECKING([whether to install API documentation])
AC_ARG_ENABLE([api_docs],
	AS_HELP_STRING([--enable-api-docs], [install API documentation]),
	[], [enable_api_docs=no])
if test "x$enable_api_docs" = "xyes"; then
	if test "x$have_doxygen" = "xno"; then
		AC_MSG_ERROR([Doxygen is required to build API documentation.])
	fi
fi
AC_MSG_RESULT([$enable_api_docs])
AM_CONDITIONAL([ENABLE_API_DOCS], [test "x$enable_api_docs" = "xyes"])

dnl ****************************************************************************
dnl output
dnl ****************************************************************************

AC_SUBST([LIBMISC_CPPFLAGS], ["$GELEMENTAL_CFLAGS"])
AC_SUBST([LIBMISC_CXXFLAGS], ["$MAINT_CXXFLAGS $DISABLE_DEPRECATED"])
AM_CONDITIONAL([LIBMISC_GTK], [true])

AC_SUBST([LIBELEMENTAL_CPPFLAGS], ["$LIBELEMENTAL_CFLAGS"])
AC_SUBST([LIBELEMENTAL_CXXFLAGS], ["$MAINT_CXXFLAGS $DISABLE_DEPRECATED"])
AC_SUBST([LIBELEMENTAL_LIBS])

AC_SUBST([GELEMENTAL_CPPFLAGS], ["$GELEMENTAL_CFLAGS"])
AC_SUBST([GELEMENTAL_CXXFLAGS], ["$MAINT_CXXFLAGS $DISABLE_DEPRECATED"])
AC_SUBST([GELEMENTAL_LIBS])

AC_CONFIG_FILES([
	Makefile
	libelemental/Makefile
	libelemental/misc/Makefile
	src/Makefile
	po/Makefile.in
	docs/Makefile
	docs/api/Makefile
	docs/api/Doxyfile
	data/Makefile
	data/libelemental.pc
	data/libelemental-uninstalled.pc
])
AC_CONFIG_HEADERS([config.h])

AC_OUTPUT
