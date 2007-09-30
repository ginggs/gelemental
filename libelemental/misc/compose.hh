//! \file compose.hh
/*! String composition library.
 * Defines compose::compose(fmt, arg...) for easy, i18n-friendly
 * composition of strings.
 * 
 * Basic usage is like
 *
 * \code compose::compose("This is a %1x%2 matrix.", rows, cols); \endcode
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

#ifndef LIBMISC__COMPOSE_HH
#define LIBMISC__COMPOSE_HH

#include <sstream>
#include <string>
#include <list>
#include <map>

//******************************************************************************

//! String composition library.
/*! A series of functions which accept a format string on the form "text %1
 * more %2 less %3" and a number of templated parameters and spits out the
 * composited string. */
namespace compose {

using std::string;

//******************************************************************************

//! The actual composition class.
class Composition
{
public:

	//! Initialise and prepare format string on the form "foo %1 bar %2 etc".
	explicit Composition (const string &fmt);

	//! Supply a replacement argument starting from %1.
	template<typename T> inline Composition& arg (const T &obj);

	//! Compose and return string.
	string str () const;

	//! Set format flags for future arguments.
	Composition& setf (std::ios_base::fmtflags set);

	//! Unset format flags for future arguments.
	Composition& unsetf (std::ios_base::fmtflags unset);
	
	//! Set precision for future arguments.
	Composition& precision (std::streamsize prec);

private:

	void do_arg (const string &rep);

	std::ostringstream os;
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

#include "compose.tcc"

#endif // LIBMISC__COMPOSE_HH
