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

#include "extras.hh"

#include <algorithm>
#include <stdexcept>
#include <glib.h>
#include <glibmm/utility.h>

namespace misc {


string
strftime (const string& format, time_t time, bool local,
	const string& fallback)
{
	try
	{
		struct tm tm_data;
		if (local) {
			if (localtime_r (&time, &tm_data) == NULL)
				throw std::runtime_error ("localtime_r");
		} else {
			if (gmtime_r (&time, &tm_data) == NULL)
				throw std::runtime_error ("gmtime_r");
		}

		size_t bufsize = std::max<size_t> (2 * format.size (), 128);

		do
		{
			const Glib::ScopedPtr<char> buf
				(static_cast<char*> (g_malloc (bufsize)));
			buf.get ()[0] = '\1';

			size_t len = std::strftime (buf.get (), bufsize, format.c_str (),
				&tm_data);

			if (len != 0 || buf.get ()[0] == '\0')
			{
				if (len >= bufsize)
					throw std::length_error ("strftime");
				return string (buf.get (), len);
			}
		}
		while ((bufsize *= 2) <= 65536);

		throw std::length_error ("strftime");
	}
	catch (...)
	{
		return fallback;
	}
}


std::vector<string>
strsplit (const string& data, const string& delimiter, unsigned int max_tokens)
{
	std::vector<string> result;
	if (data.empty ()) return result;
	unsigned int tokens = 0;

	string::size_type pos = 0, next = 0;
	while ((next = data.find_first_of (delimiter, pos)) != string::npos)
	{
		if (++tokens == max_tokens) break;
		result.push_back (data.substr (pos, next - pos));
		pos = next + 1;
	}
	result.push_back (data.substr (pos));

	return result;
}


std::vector<ustring>
ustrsplit (const ustring& data, const ustring& delimiter,
	unsigned int max_tokens)
{
	std::vector<ustring> result;
	if (data.empty ()) return result;
	unsigned int tokens = 0;

	ustring::size_type pos = 0, next = 0;
	while ((next = data.find_first_of (delimiter, pos)) != ustring::npos)
	{
		if (++tokens == max_tokens) break;
		result.push_back (data.substr (pos, next - pos));
		pos = next + 1;
	}
	result.push_back (data.substr (pos));

	return result;
}


string
strstrip (const string& string)
{
	return Glib::ScopedPtr<gchar>
		(g_strstrip (g_strdup (string.data ()))).get ();
}


ustring
ustrstrip (const ustring& string)
{
	return Glib::ScopedPtr<gchar>
		(g_strstrip (g_strdup (string.data ()))).get ();
}


} // namespace misc
