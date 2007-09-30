/*
 * This file is part of libmisc, an assortment of code for reuse.
 * Modified from compose version 1.0.4.
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

#include "compose.hh"

namespace compose {


// Helper for converting spec string numbers.
inline int
char_to_int (char c)
{
	switch (c)
	{
	case '0': return 0;
	case '1': return 1;
	case '2': return 2;
	case '3': return 3;
	case '4': return 4;
	case '5': return 5;
	case '6': return 6;
	case '7': return 7;
	case '8': return 8;
	case '9': return 9;
	default: return -1000;
	}
}

// Helper for identifying spec string numbers.
inline bool
is_number (int n)
{
	switch (n)
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		return true;

	default:
		return false;
	}
}


//******************************************************************************
// class Composition


Composition::Composition (const string &fmt_)
	: arg_no (1)
{
	string::size_type b = 0, i = 0;
	string fmt = fmt_;

	// fill in output with the strings between the %1 %2 %3 etc.
	// and fill in specs with the positions
	while (i < fmt.length ())
	{
		if (fmt[i] == '%' && i + 1 < fmt.length ())
		{
			if (fmt[i + 1] == '%') // catch %%
				fmt.replace (i++, 2, "%");
			else if (is_number (fmt[i + 1])) // aha! a spec!
			{
				// save string
				output.push_back (fmt.substr (b, i - b));

				int n = 1; // number of digits
				int spec_no = 0;

				do
				{
					spec_no += char_to_int (fmt[i + n]);
					spec_no *= 10;
					++n;
				}
				while (i + n < fmt.length () && is_number (fmt[i + n]));

				spec_no /= 10;
				output_list::iterator pos = output.end ();
				--pos; // safe since we have just inserted a string

				specs.insert (specification_map::value_type (spec_no, pos));

				// jump over spec string
				i += n;
				b = i;
			}
			else
				++i;
		}
		else
			++i;
	}

	if (i - b > 0) // add the rest of the string
		output.push_back (fmt.substr (b, i - b));
}


string
Composition::str () const
{
	// assemble string
	string result;

	for (output_list::const_iterator i = output.begin (), end = output.end ();
		i != end; ++i)
	result += *i;

	return result;
}


Composition&
Composition::setf (std::ios_base::fmtflags set)
{
	os.setf (set);
	return *this;
}


Composition&
Composition::unsetf (std::ios_base::fmtflags unset)
{
	os.unsetf (unset);
	return *this;
}


Composition&
Composition::precision (std::streamsize prec)
{
	if (prec > 0) os.precision (prec);
	return *this;
}


void
Composition::do_arg (const string &rep)
{
	if (!rep.empty ()) // manipulators don't produce output
	{
		for (specification_map::const_iterator i = specs.lower_bound (arg_no),
				end = specs.upper_bound (arg_no); i != end; ++i)
		{
			output_list::iterator pos = i->second;
			++pos;

			output.insert (pos, rep);
		}

		os.str (string ());
		//os.clear ();
		++arg_no;
	}
}


} // namespace compose
