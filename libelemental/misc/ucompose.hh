//! \file ucompose.hh
/*! String composition library for UTF-8.
 * Defines compose::ucompose(fmt, arg...) for easy, i18n-friendly
 * composition of strings. Uses Glib::ustring instead of std::string, which
 * doesn't work with Glib due to character encoding troubles with stringstreams.
 *
 * Basic usage is like
 *
 * \code compose::ucompose("This is a %1x%2 matrix.", rows, cols); \endcode
 *
 * \sa See http://www.cs.aau.dk/~olau/compose/ or the included README for more
 * details. Note that these refer to the official version, which uses different
 * naming than this modified version.
 *
 * Modified from compose version 1.0.4. */
/*
 * This file is part of libmisc, an assortment of code for reuse.
 *
 * Copyright (c) 2006-2007 Kevin Daughtridge <kevin@kdau.com>
 * Copyright (c) 2002-2004 Ole Laursen <olau@hardworking.dk>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBMISC__UCOMPOSE_HH
#define LIBMISC__UCOMPOSE_HH

#include <sstream>
#include <string>
#include <list>
#include <map>
#include <glibmm/convert.h>
#include <glibmm/ustring.h>

//******************************************************************************

namespace compose {

using std::string;
using Glib::ustring;

//******************************************************************************

//! The actual composition class.
class UComposition
{
public:

	//! Initialise and prepare format string on the form "foo %1 bar %2 etc".
	explicit UComposition (const string &fmt);

	//! Supply a replacement argument starting from %1.
	template<typename T> inline UComposition& arg (const T &obj);

	//! Compose and return string.
	ustring str () const;

	//! Set format flags for future arguments.
	UComposition& setf (std::ios_base::fmtflags set);

	//! Unset format flags for future arguments.
	UComposition& unsetf (std::ios_base::fmtflags unset);
	
	//! Set precision for future arguments.
	UComposition& precision (std::streamsize prec);

private:

	void do_arg (const ustring &rep);

	ustring do_stringify (const std::wstring& src);

	template<typename T> inline string stringify (T obj);

	std::wostringstream os;
	int arg_no;

	//!
	/*! The output is stored as a list; when the output string is requested, the
	 * list is concatenated to a string. This way, iterators can be kept into
	 * the list instead of into a string where they're possibly invalidated
	 * when inserting a specification string. */
	typedef std::list<string> output_list;
	output_list output;

	//!
	/*! The initial parse of the format string fills in the specification map
	 * with positions for each of the various %?s. */
	typedef std::multimap<int, output_list::iterator> specification_map;
	specification_map specs;
};

} // namespace compose

#include "ucompose.tcc"

#endif // LIBMISC__UCOMPOSE_HH
