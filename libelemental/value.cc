/*
 * This file is part of libelemental, a periodic table library with detailed
 * information on elements.
 *
 * Copyright (C) 2006-2007 Kevin Daughtridge <kevin@kdau.com>
 * Copyright (C) 2003 Jonas Frantz <jonas.frantz@welho.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include "private.hh"
#include <libelemental/value.hh>
#include <libelemental/value-types.hh>

#include <ext/stdio_filebuf.h>
#include <glibmm/utility.h>
#include <pango/pango-attributes.h>

namespace Elemental {


const double STANDARD_TEMPERATURE = 273.15; // Kelvin


ustring
get_list_separator () throw ()
{
	// Translators: This is the separator for items in a list.
	static const Message LIST_SEPARATOR = N_(", ");
	return LIST_SEPARATOR.get_string ();
}


//******************************************************************************
// struct color


color::color (double red_, double green_, double blue_) throw ()
:	red (CLAMP (red_, 0.0, 1.0)),
	green (CLAMP (green_, 0.0, 1.0)),
	blue (CLAMP (blue_, 0.0, 1.0))
{}


color::color (unsigned int red_, unsigned int green_, unsigned int blue_)
	throw ()
:	red (std::min (red_, 65535u) / 65535.0),
	green (std::min (green_, 65535u) / 65535.0),
	blue (std::min (blue_, 65535u) / 65535.0)
{}


double
color::get_luminance () const throw ()
{
	double luminance = red * 0.2126 + green * 0.7152 + blue * 0.0722;
	return CLAMP (luminance, 0.0, 1.0);
}


color
color::get_compliment () const throw ()
{
	double grey = (get_luminance () > 0.4) ? 0.0 : 1.0;
	return color (grey, grey, grey);
}


color
color::composite (const color& other, double alpha) const throw ()
{
	alpha = CLAMP (alpha, 0.0, 1.0);
	double factor = 1.0 - alpha;
	return color (red * factor + other.red * alpha,
		green * factor + other.green * alpha,
		blue * factor + other.blue * alpha);
}


std::string
color::get_hex_spec () const throw ()
{
	std::ostringstream os;
	os << "#" << std::hex << std::setfill ('0');
	os << std::setw (2) << CLAMP (int (red * 255.0), 0, 255);
	os << std::setw (2) << CLAMP (int (green * 255.0), 0, 255);
	os << std::setw (2) << CLAMP (int (blue * 255.0), 0, 255);
	return os.str ();
}


//******************************************************************************
// class EntriesView


EntriesView::~EntriesView ()
{}


ustring::size_type
EntriesView::get_max_name_length () throw ()
{
	return max_name_length;
}


void
EntriesView::accommodate_name (ustring::size_type length) throw ()
{
	if (length > max_name_length)
		max_name_length = length;
}


ustring::size_type
EntriesView::max_name_length = 0;


//******************************************************************************
// class EntriesStream


EntriesStream::EntriesStream (FILE* file) throw ()
:	buf (new __gnu_cxx::stdio_filebuf<char> (file, std::ios_base::out)),
	buf_owned (true), os (*new std::ostream (buf)), os_owned (true)
{}


EntriesStream::EntriesStream (int fd) throw ()
:	buf (new __gnu_cxx::stdio_filebuf<char> (fd, std::ios_base::out)),
	buf_owned (true), os (*new std::ostream (buf)), os_owned (true)
{}


EntriesStream::EntriesStream (std::streambuf& buf_) throw ()
:	buf (&buf_), buf_owned (false),
	os (*new std::ostream (buf)), os_owned (true)
{}


EntriesStream::EntriesStream (std::ostream& os_) throw ()
:	buf (NULL), buf_owned (false),
	os (os_), os_owned (false)
{}


EntriesStream::~EntriesStream ()
{
	if (os_owned) delete &os;
	if (buf_owned) delete buf;
}


void
EntriesStream::header (const ustring& category) throw ()
{
	os << std::endl << category << std::endl;
}


void
EntriesStream::entry (const ustring& name, const ustring& value,
	const ustring& tip) throw ()
{
	ustring result = "  ";

	if (!name.empty ())
	{
		result += name;

		ustring::size_type padding =
			std::max (0, int (get_max_name_length ()) - int (name.size ())) + 1;
		result += ustring (padding, ' ');
	}

	{
		Glib::ScopedPtr<char> parsed_value;
		GError *error = NULL;
		if (!pango_parse_markup (value.data (), -1, 0, NULL,
			parsed_value.addr (), NULL, &error))
		{
			os << Glib::Error (error).what () << std::endl;
			return;
		}

		result += tip.empty () ? parsed_value.get ()
			: compose::ucompose (_("%1 (%2)"), parsed_value.get (), tip);
	}

	if (Glib::get_charset ())
		os << result;
	else
	{
		std::string charset;
		Glib::get_charset (charset);
		os << Glib::convert_with_fallback (result, charset, "UTF-8", "?");
	}
		
	os << std::endl;
}


//******************************************************************************
// class value_base


const int value_base::YIELD_COMPARE = -2;


value_base::value_base (Qualifier qualifier_) throw ()
:	qualifier (qualifier_), always (false)
{}


value_base::~value_base ()
{}


bool
value_base::has_value () const throw ()
{
	switch (qualifier)
	{
	case Q_UNK:
	case Q_NA:
		return false;
	case Q_NEUTRAL:
	case Q_EST:
	case Q_CA:
	case Q_ISO:
	default:
		return true;
	}
}


ustring
value_base::get_string (const ustring& format) const throw ()
{
	switch (qualifier)
	{
	case Q_UNK: return _("(unknown)");
	case Q_NA: return _("(n/a)");
	// Translators: "(%1)" represents an estimated or calculated value.
	case Q_EST: return compose::ucompose (_("(%1)"), do_get_string (format));
	// Translators: "~%1" represents an approximate value.
	case Q_CA: return compose::ucompose (_("~%1"), do_get_string (format));
	// Translators: "[%1]" represents a value for a most stable isotope.
	case Q_ISO: return compose::ucompose (_("[%1]"), do_get_string (format));
	case Q_NEUTRAL:
	default:
		return do_get_string (format);
	}
}


ustring
value_base::get_tip () const throw ()
{
	switch (qualifier)
	{
	case Q_EST: return _("Estimated or calculated value");
	case Q_CA: return _("Approximate");
	case Q_ISO: return _("Value for most stable isotope");
	default: return ustring ();
	}
}


void
value_base::make_entry (EntriesView& view, const ustring& name,
	const ustring& format) const throw ()
{
	if (always || has_value ())
		view.entry (name, get_string (format), get_tip ());
}


int
value_base::compare (const value_base& other) const throw ()
{
	int base = compare_base (other);
	return (base == YIELD_COMPARE) ? 0 : base;
}


int
value_base::compare_base (const value_base& other) const throw ()
{
	if (has_value ()) {
		if (other.has_value ())
			return YIELD_COMPARE;
		else
			return -1;
	} else {
		if (other.has_value ())
			return 1;
		else
			return 0;
	}
}


//******************************************************************************
// class color_value_base


color_value_base::color_value_base (Qualifier qualifier_) throw ()
:	value_base (qualifier_)
{}


color_value_base::~color_value_base ()
{}


} // namespace Elemental
