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
#include <libelemental/data.hh>
#include <libelemental/table.hh>

namespace Elemental {


static Table table;


void
initialize () throw ()
{
	static bool initialized = false;
	if (initialized) return;

	initialized = true;

	bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");

	for (unsigned int i = 0; i < table_length; ++i)
		table.push_back (new Element (table_data[i]));
	
	FOREACH (std::list<Category*>, CATEGORIES, category)
		FOREACH (std::list<PropertyBase*>, (*category)->properties, prop)
		{
			EntriesView::accommodate_name
				(compose::ucompose (_("%1:"), (*prop)->get_name ()).size ());

			if cast (*prop, FloatProperty, float_prop)
				FOREACH (Table, table, el)
					float_prop->process_value
						((*el)->get_property (*float_prop));
		}
}


const Table&
get_table () throw ()
{
	initialize ();
	return table;
}


const Element&
get_element (AtomicNumber number) throw (std::out_of_range)
{
	initialize ();
	return *table.at (number - 1);
}


const Element&
get_element (const std::string& which) throw (std::invalid_argument)
{
	initialize ();

	try
	{
		if (Glib::Unicode::isdigit (which.at (0)))
			return get_element (compose::decompose<AtomicNumber> (which));
	}
	catch (...)
	{}

	CONST_FOREACH (Table, table, el)
		if ((*el)->symbol == which)
			return **el;

	throw std::invalid_argument ("invalid symbol or number");
}


//******************************************************************************
// class Tango


const color Tango::BUTTER (0xFEFEu, 0xF5F5u, 0xA9A9u);
const color Tango::ORANGE (0xF8F8u, 0xCDCDu, 0x8A8Au);
const color Tango::LIGHT_CHOCOLATE (0xF5F5u, 0xDDDDu, 0xB8B8u);
const color Tango::DARK_CHOCOLATE (0x8F8Fu, 0x7474u, 0x4848u);
const color Tango::CHAMELEON (0xC5C5u, 0xF1F1u, 0x9A9Au);
const color Tango::SKY_BLUE (0xB9B9u, 0xD0D0u, 0xE8E8u);
const color Tango::LIGHT_PLUM (0xD7D7u, 0xC0C0u, 0xD4D4u);
const color Tango::DARK_PLUM (0xB8B8u, 0x8B8Bu, 0xC3C3u);
const color Tango::LIGHT_SCARLET_RED (0xF8F8u, 0x9595u, 0x9595u);
const color Tango::DARK_SCARLET_RED (0xFFFFu, 0x6767u, 0x6767u);
const color Tango::LIGHT_ALUMINIUM (0xF7F7u, 0xF7F7u, 0xF6F6u);
const color Tango::MEDIUM_ALUMINIUM (0xC5C5u, 0xC6C6u, 0xC3C3u);


} // namespace Elemental
