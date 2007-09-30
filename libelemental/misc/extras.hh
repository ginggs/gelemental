//! \file extras.hh
//! Extra functions for string manipulation.
/*
 * This file is part of libmisc, an assortment of code for reuse.
 *
 * Copyright (c) 2006-2007 Kevin Daughtridge <kevin@kdau.com>
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

#ifndef LIBMISC__EXTRAS_HH
#define LIBMISC__EXTRAS_HH

#include <ctime>
#include <sstream>
#include <string>
#include <vector>
#include <glibmm/ustring.h>

//******************************************************************************

namespace compose {

using std::string;
using Glib::ustring;

//! Decompose a string containing a single representation to its object type.
template<typename T>
inline T
decompose (const string &src, const T &fallback = T ())
{
	std::istringstream is; is.str (src);
	T result (fallback); is >> result;
	return result;
}

//! Compose a single object into its string representation.
template<typename T>
inline string
compose1 (const T &src, int precision = 0)
{
	std::ostringstream os;
	if (precision > 0) os.precision (precision);
	os << src; return os.str ();
}

//! Compose a single object into its UTF-8 string representation.
template<typename T>
inline ustring
ucompose1 (const T &src, int precision = 0)
{
	std::ostringstream os;
	if (precision > 0) os.precision (precision);
	os << src; return os.str ();
}

} // namespace compose

//******************************************************************************

namespace misc {

using std::string;
using Glib::ustring;

//! Compose a string from a time value using libc's strftime safely.
/*! \param fmt A strftime-style format string.
 * \param time The time to be represented, in seconds since the epoch.
 * \param local Whether to represent local (<tt>true</tt>) or UTC
 * (<tt>false</tt>) time.
 * \param fallback String to return if buffer space cannot be allocated. By
 * default, the empty string. */
string strftime (const string &fmt, time_t time, bool local,
	const string &fallback = string ());

//! Splits a string into pieces using a delimiter.
/*! If \p max_tokens is reached, the remainder of \p data is appended to the
 * last token. As a special case, the result of splitting an empty string is an
 * empty vector, not a vector containing a single empty string.
 * \param data A string to split.
 * \param delimiter A string which specifies the places at which to split the
 * string. The delimiter is not included in any of the resulting strings, unless
 * \p max_tokens is reached.
 * \param max_tokens The maximum number of pieces to split \p data into. If this
 * is less than one, the string is split completely.
 * \return A vector of string tokens. */
std::vector<string> strsplit (const string& data, const string& delimiter,
	unsigned int max_tokens = 0);

//! Splits a UTF-8 string into pieces using a delimiter.
/*! If \p max_tokens is reached, the remainder of \p data is appended to the
 * last token. As a special case, the result of splitting an empty string is an
 * empty vector, not a vector containing a single empty string.
 * \param data A UTF-8 string to split.
 * \param delimiter A string which specifies the places at which to split the
 * string. The delimiter is not included in any of the resulting strings, unless
 * \p max_tokens is reached.
 * \param max_tokens The maximum number of pieces to split \p data into. If this
 * is less than one, the string is split completely.
 * \return A vector of UTF-8 string tokens. */
std::vector<ustring> ustrsplit (const ustring& data, const ustring& delimiter,
	unsigned int max_tokens = 0);

//! Removes leading and trailing whitespace from a string.
string strstrip (const string& string);

//! Removes leading and trailing whitespace from a UTF-8 string.
ustring ustrstrip (const ustring& string);

} // namespace misc

#endif // LIBMISC__EXTRAS_HH
