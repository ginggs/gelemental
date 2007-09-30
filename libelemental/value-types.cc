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
#include <libelemental/value-types.hh>

#include <glibmm/date.h>

namespace Elemental {

	
//******************************************************************************
// class Message


Message::Message (const char* source, Qualifier qualifier_) throw ()
:	value_base (qualifier_), value (source)
{}


Message::Message (Qualifier qualifier_) throw ()
:	value_base (qualifier_)
{}


Message::~Message ()
{}


int
Message::compare (const value_base& other) const throw ()
{
	int base = compare_base (other);
	if (base != YIELD_COMPARE) return base;
	
	if cast (&other, const Message, like_other)
		return do_get_string (ustring ()).collate_key ().compare
			(like_other->do_get_string (ustring ()).collate_key ());

	return 0;
}


ustring
Message::get_string (const ustring& format) const throw ()
{
	switch (qualifier)
	{
	case Q_EST:
	case Q_CA:
		// Translators: "(?) %1" prefixes a string indicating an inexact value.
		return compose::ucompose (_("(?) %1"), do_get_string (format));
	default:
		return value_base::get_string (format);
	}
}


ustring
Message::do_get_string (const ustring& format) const throw ()
{
	ustring result = _(value.data ());
	return format.empty () ? result : compose::ucompose (format, result);
}


//******************************************************************************
// class Event


Event::Event (int when_, const ustring& where_, Qualifier qualifier_) throw ()
:	value_base (qualifier_), when (when_), where (where_)
{
	always = true;
}


Event::Event (Qualifier qualifier_) throw ()
:	value_base (qualifier_)
{
	always = true;
}

Event::~Event ()
{}


int
Event::compare (const value_base& other) const throw ()
{
	int base = compare_base (other);
	if (base != YIELD_COMPARE) return base;

	if cast (&other, const Event, like_other)
	{
		if (when < like_other->when)
			return -1;
		else if (when > like_other->when)
			return 1;
		else
			return 0;
	}

	return 0;
}


ustring
Event::get_string (const ustring& format) const throw ()
{
	switch (qualifier)
	{
	case Q_UNK: return _("Undiscovered");
	case Q_NA: return _("Known to the ancients");
	case Q_NEUTRAL:
	default:
		return do_get_string (format.empty () ? _("%1 (%2)") : format);
	}
}


ustring
Event::do_get_string (const ustring& format) const throw ()
{
	return compose::ucompose (format,
		Glib::Date (1, Glib::Date::JANUARY, when).format_string ("%Y"),
		_(where.data ()));
}


int
Event::compare_base (const value_base& other) const throw ()
{
	switch (qualifier)
	{
	case Q_UNK:
		return (other.qualifier == Q_UNK) ? 0 : 1;
	case Q_NA:
		return (other.qualifier == Q_NA) ? 0 : -1;
	case Q_NEUTRAL:
	default:
		switch (other.qualifier)
		{
		case Q_UNK:
			return -1;
		case Q_NA:
			return 1;
		case Q_NEUTRAL:
		default:
			return YIELD_COMPARE;
		}
	}
}


//******************************************************************************
// class Series


Series::Series (const Value& source, Qualifier qualifier_) throw ()
:	color_value_base (qualifier_), value (source)
{}


Series::Series (Qualifier qualifier_) throw ()
:	color_value_base (qualifier_)
{}


Series::~Series ()
{}


int
Series::compare (const value_base& other) const throw ()
{
	int base = compare_base (other);
	if (base != YIELD_COMPARE) return base;

	if cast (&other, const Series, like_other)
	{
		if (int (value) < int (like_other->value))
			return -1;
		else if (int (value) > int (like_other->value))
			return 1;
		else
			return 0;
	}

	return 0;
}


color
Series::get_color () const throw ()
{
	if (has_value ())
		switch (value)
		{
		case NONMETAL:
			return Tango::CHAMELEON;
		case NOBLE_GAS:
			return Tango::SKY_BLUE;
		case ALKALI_METAL:
			return Tango::DARK_SCARLET_RED;
		case ALKALINE_EARTH_METAL:
			return Tango::ORANGE;
		case SEMIMETAL:
			return Tango::LIGHT_CHOCOLATE;
		case HALOGEN:
			return Tango::BUTTER;
		case POST_TRANSITION_METAL:
			return Tango::MEDIUM_ALUMINIUM;
		case TRANSITION_METAL:
			return Tango::LIGHT_SCARLET_RED;
		case LANTHANIDE:
			return Tango::LIGHT_PLUM;
		case ACTINIDE:
			return Tango::DARK_PLUM;
		}

	return Tango::LIGHT_ALUMINIUM;
}


ustring
Series::do_get_string (const ustring& format) const throw ()
{
	ustring result = _("(unknown)");

	switch (value)
	{
	case NONMETAL:
		result = _("Nonmetals"); break;
	case NOBLE_GAS:
		result = _("Noble gases"); break;
	case ALKALI_METAL:
		result = _("Alkali metals"); break;
	case ALKALINE_EARTH_METAL:
		result = _("Alkaline earth metals"); break;
	case SEMIMETAL:
		result = _("Semimetals"); break;
	case HALOGEN:
		result = _("Halogens"); break;
	case POST_TRANSITION_METAL:
		result = _("Post-transition metals"); break;
	case TRANSITION_METAL:
		result = _("Transition metals"); break;
	case LANTHANIDE:
		result = _("Lanthanides"); break;
	case ACTINIDE:
		result = _("Actinides"); break;
	}

	return format.empty () ? result : compose::ucompose (format, result);
}


//******************************************************************************
// class Block


Block::Block (const Value& source, Qualifier qualifier_) throw ()
:	color_value_base (qualifier_), value (source)
{}


Block::Block (Qualifier qualifier_) throw ()
:	color_value_base (qualifier_)
{}


Block::~Block ()
{}


int
Block::compare (const value_base& other) const throw ()
{
	int base = compare_base (other);
	if (base != YIELD_COMPARE) return base;

	if cast (&other, const Block, like_other)
	{
		if (int (value) < int (like_other->value))
			return -1;
		else if (int (value) > int (like_other->value))
			return 1;
		else
			return 0;
	}

	return 0;
}


color
Block::get_color () const throw ()
{
	if (has_value ())
		switch (value)
		{
		case S:	return Tango::ORANGE;
		case P:	return Tango::BUTTER;
		case D:	return Tango::LIGHT_SCARLET_RED;
		case F:	return Tango::DARK_PLUM;
		}

	return Tango::LIGHT_ALUMINIUM;
}


ustring
Block::do_get_string (const ustring& format) const throw ()
{
	ustring result = _("(unknown)");

	switch (value)
	{
	case S: result = _("s-block"); break;
	case P: result = _("p-block"); break;
	case D: result = _("d-block"); break;
	case F: result = _("f-block"); break;
	}

	return format.empty () ? result : compose::ucompose (format, result);
}


//******************************************************************************
// class Phase


Phase::Phase (const Value& source, Qualifier qualifier_) throw ()
:	color_value_base (qualifier_), value (source)
{}


Phase::Phase (Qualifier qualifier_) throw ()
:	color_value_base (qualifier_)
{}


Phase::~Phase ()
{}


int
Phase::compare (const value_base& other) const throw ()
{
	int base = compare_base (other);
	if (base != YIELD_COMPARE) return base;

	if cast (&other, const Phase, like_other)
	{
		if (int (value) < int (like_other->value))
			return -1;
		else if (int (value) > int (like_other->value))
			return 1;
		else
			return 0;
	}

	return 0;
}


color
Phase::get_color () const throw ()
{
	if (has_value ())
		switch (value)
		{
		case SOLID:
			return Tango::LIGHT_CHOCOLATE;
		case LIQUID:
			return Tango::CHAMELEON;
		case GAS:
			return Tango::SKY_BLUE;
		}

	return Tango::LIGHT_ALUMINIUM;
}


ustring
Phase::do_get_string (const ustring& format) const throw ()
{
	ustring result = _("(unknown)");

	switch (value)
	{
	case SOLID:
		result = _("Solid"); break;
	case LIQUID:
		result = _("Liquid"); break;
	case GAS:
		result = _("Gas"); break;
	}

	return format.empty () ? result : compose::ucompose (format, result);
}


//******************************************************************************
// class LatticeType


LatticeType::LatticeType (const Value& source, Qualifier qualifier_) throw ()
:	color_value_base (qualifier_), value (source)
{}


LatticeType::LatticeType (Qualifier qualifier_) throw ()
:	color_value_base (qualifier_)
{}


LatticeType::~LatticeType ()
{}


int
LatticeType::compare (const value_base& other) const throw ()
{
	int base = compare_base (other);
	if (base != YIELD_COMPARE) return base;

	if cast (&other, const LatticeType, like_other)
	{
		if (int (value) < int (like_other->value))
			return -1;
		else if (int (value) > int (like_other->value))
			return 1;
		else
			return 0;
	}

	return 0;
}


color
LatticeType::get_color () const throw ()
{
	if (has_value ())
		switch (value)
		{
		case TRI:
			return Tango::CHAMELEON;
		case MONO:
			return Tango::SKY_BLUE;
		case ORTH:
			return Tango::ORANGE;
		case TET:
			return Tango::BUTTER;
		case RHO:
			return Tango::MEDIUM_ALUMINIUM;
		case HEX:
			return Tango::LIGHT_SCARLET_RED;
		case SC:
		case BCC:
		case FCC:
			return Tango::DARK_PLUM;
		}

	return Tango::LIGHT_ALUMINIUM;
}


ustring
LatticeType::do_get_string (const ustring& format) const throw ()
{
	ustring result = _("(unknown)");

	switch (value)
	{
	case TRI:
		result = _("Triclinic"); break;
	case MONO:
		result = _("Monoclinic"); break;
	case ORTH:
		result = _("Orthorhombic"); break;
	case TET:
		result = _("Tetragonal"); break;		
	case RHO:
		result = _("Rhombohedral"); break;
	case HEX:
		result = _("Hexagonal"); break;
	case SC:
		result = _("Simple cubic"); break;
	case BCC:
		result = _("Body-centered cubic"); break;
	case FCC:
		result = _("Face-centered cubic"); break;
	}

	return format.empty () ? result : compose::ucompose (format, result);
}


//******************************************************************************
// class ColorValue


ColorValue::ColorValue (const color& source, Qualifier qualifier_) throw ()
:	color_value_base (qualifier_), value (source)
{}


ColorValue::ColorValue (Qualifier qualifier_) throw ()
:	color_value_base (qualifier_)
{}


ColorValue::ColorValue (double position, Qualifier qualifier_) throw ()
:	color_value_base (qualifier_),
	value (Tango::BUTTER.composite (Tango::DARK_SCARLET_RED, position))
{}


ColorValue::~ColorValue ()
{}


int
ColorValue::compare (const value_base& other) const throw ()
{
	int base = compare_base (other);
	if (base != YIELD_COMPARE) return base;

	if cast (&other, const ColorValue, like_other)
	{
		double luminance = value.get_luminance (),
			other_luminance = like_other->value.get_luminance ();
		if (luminance < other_luminance)
			return -1;
		else if (luminance > other_luminance)
			return 1;
		else
			return 0;
	}

	return 0;
}


color
ColorValue::get_color () const throw ()
{
	if (has_value ())
		return value;
	else
		return Tango::LIGHT_ALUMINIUM;
}


ustring
ColorValue::do_get_string (const ustring& format) const throw ()
{
	ustring result = value.get_hex_spec ();
	return format.empty () ? result : compose::ucompose (format, result);
}


} // namespace Elemental
