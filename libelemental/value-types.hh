//! \file value-types.hh
//! Instantiations of Value<> and higher-level value types.
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

#ifndef LIBELEMENTAL__VALUE_TYPES_HH
#define LIBELEMENTAL__VALUE_TYPES_HH

#include <libelemental/value.hh>

//******************************************************************************

namespace Elemental {

//! A qualified floating-point value.
typedef Value<double> Float;

//! A qualified integer value.
typedef Value<long> Int;

//! A qualified UTF-8 string value that does not require translation.
typedef Value<ustring> String;

//! A qualified list of floating-point values.
typedef ValueList<double> FloatList;

//! A qualified list of integer values.
typedef ValueList<long> IntList;

//******************************************************************************

//! A qualified UTF-8 string value that requires translation.
//! Messages are compared by collation order.
class Message
:	public value_base
{
public:

	//! Constructs a new defined message.
	/*! \param source The untranslated message.
	 * \param qualifier The qualification of the message. */
	Message (const char* source, Qualifier qualifier = Q_NEUTRAL) throw ();

	//! Constructs a new undefined or empty message.
	//! \param qualifier The qualification of the message.
	Message (Qualifier qualifier = Q_UNK) throw ();

	virtual ~Message ();

	virtual int compare (const value_base& other) const throw ();

	virtual ustring get_string (const ustring& format = ustring ()) const
		throw ();

	//! The untranslated message, if defined.
	ustring value;

protected:

	virtual ustring do_get_string (const ustring& format) const throw ();
};

//******************************************************************************

//! A qualified value representing the time and location of an event.
//! Events are compared by year of occurrence.
class Event
:	public value_base
{
public:

	//! Constructs a new defined event.
	/*! \param when The year in which the event occurred.
	 * \param where The place in which the event occurred.
	 * \param qualifier The qualification of the event. */
	Event (int when, const ustring& where, Qualifier qualifier = Q_NEUTRAL)
		throw ();

	//! Constructs a new undefined or empty event.
	//! \param qualifier The qualification of the event.
	Event (Qualifier qualifier = Q_UNK) throw ();

	virtual ~Event ();

	virtual int compare (const value_base& other) const throw ();

	//! Returns a localized text representation of the event.
	/*! \param format An optional compose-style format string with which to
	 * output the value. The year is "%1" and the place is "%2". By default,
	 * the format is the localized version of \c "%1 (%2)".
	 * \return A UTF-8 string containing the localized representation. */
	virtual ustring get_string (const ustring& format = ustring ()) const
		throw ();
	
	//! The year in which the event occurred, if defined.
	int when;
	
	//! The place in which the event occurred, if defined.
	ustring where;

protected:

	virtual ustring do_get_string (const ustring& format) const throw ();

	//! Compares the qualifications of two events.
	virtual int compare_base (const value_base& other) const throw ();
};

//******************************************************************************

//! A qualified value representing a chemical series.
class Series
:	public color_value_base
{
public:

	//! The possible chemical series values.
	enum Value
	{
		NONMETAL, //!< nonmetals
		NOBLE_GAS, //!< noble gases
		ALKALI_METAL, //!< alkali metals
		ALKALINE_EARTH_METAL, //!< alkaline earth metals
		SEMIMETAL, //!< semimetals
		HALOGEN, //!< halogens
		POST_TRANSITION_METAL, //!< post-transition metals
		TRANSITION_METAL, //!< transition metals
		LANTHANIDE, //!< lanthanides
		ACTINIDE //!< actinides
	};

	//! Constructs a new defined series.
	/*! \param source The initial definition.
	 * \param qualifier The qualification of the initial value. */
	Series (const Value& source, Qualifier qualifier = Q_NEUTRAL) throw ();

	//! Constructs a new undefined or default series.
	//! \param qualifier The qualification of the initial value.
	Series (Qualifier qualifier = Q_UNK) throw ();
	
	virtual ~Series ();

	virtual int compare (const value_base& other) const throw ();

	virtual color get_color () const throw ();
	
	//! The value, if defined.
	Value value;

protected:

	virtual ustring do_get_string (const ustring& format) const throw ();
};

//******************************************************************************

//! A qualified value representing a periodic table block.
class Block
:	public color_value_base
{
public:

	//! The possible lattice type values.
	enum Value
	{
		S, //!< s-block (sharp)
		P, //!< p-block (principal)
		D, //!< d-block (diffuse)
		F //!< f-block (fundamental)
	};

	//! Constructs a new defined block.
	/*! \param source The initial definition.
	 * \param qualifier The qualification of the initial value. */
	Block (const Value& source, Qualifier qualifier = Q_NEUTRAL) throw ();

	//! Constructs a new undefined or default block.
	//! \param qualifier The qualification of the initial value.
	Block (Qualifier qualifier = Q_UNK) throw ();
	
	virtual ~Block ();

	virtual int compare (const value_base& other) const throw ();

	virtual color get_color () const throw ();

	//! The value, if defined.
	Value value;

protected:

	virtual ustring do_get_string (const ustring& format) const throw ();
};

//******************************************************************************

//! A qualified value representing a phase of matter.
class Phase
:	public color_value_base
{
public:

	//! The possible phase values.
	enum Value
	{
		SOLID, //!< solid
		LIQUID, //!< liquid
		GAS //!< gas
	};

	//! Constructs a new defined phase.
	/*! \param source The initial definition.
	 * \param qualifier The qualification of the initial value. */
	Phase (const Value& source, Qualifier qualifier = Q_NEUTRAL) throw ();

	//! Constructs a new undefined or default phase.
	//! \param qualifier The qualification of the initial value.
	Phase (Qualifier qualifier = Q_UNK) throw ();
	
	virtual ~Phase ();

	virtual int compare (const value_base& other) const throw ();

	virtual color get_color () const throw ();

	//! The value, if defined.
	Value value;

protected:

	virtual ustring do_get_string (const ustring& format) const throw ();
};

//******************************************************************************

//! A qualified value representing a Bravais lattice type.
class LatticeType
:	public color_value_base
{
public:

	//! The possible lattice type values.
	enum Value
	{
		TRI, //!< triclinic
		MONO, //!< monoclinic
		ORTH, //!< orthorhombic
		TET, //!< tetragonal
		RHO, //!< rhombohedral
		HEX, //!< hexagonal
		SC, //!< simple cubic
		BCC, //!< body-centered cubic
		FCC //!< face-centered cubic
	};

	//! Constructs a new defined lattice type.
	/*! \param source The initial definition.
	 * \param qualifier The qualification of the initial value. */
	LatticeType (const Value& source, Qualifier qualifier = Q_NEUTRAL) throw ();

	//! Constructs a new undefined or default lattice type.
	//! \param qualifier The qualification of the initial value.
	LatticeType (Qualifier qualifier = Q_UNK) throw ();
	
	virtual ~LatticeType ();

	virtual int compare (const value_base& other) const throw ();

	virtual color get_color () const throw ();

	//! The value, if defined.
	Value value;

protected:

	virtual ustring do_get_string (const ustring& format) const throw ();
};

//******************************************************************************

//! A qualified value representing a display color.
//! Color values are compared by luminous intensity.
class ColorValue
:	public color_value_base
{
public:

	//! Constructs a new defined color.
	/*! \param source The initial definition.
	 * \param qualifier The qualification of the initial value. */
	ColorValue (const color& source, Qualifier qualifier = Q_NEUTRAL) throw ();

	//! Constructs a new undefined or default color.
	//! \param qualifier The qualification of the initial value.
	ColorValue (Qualifier qualifier = Q_UNK) throw ();
	
	//! Constructs a new defined color representing a scale position.
	/*! \param position A scale position between 0.0 and 1.0 inclusive.
	 * \param qualifier The qualification of the initial value. */
	explicit ColorValue (double position, Qualifier qualifier = Q_NEUTRAL)
		throw ();
	
	virtual ~ColorValue ();

	virtual int compare (const value_base& other) const throw ();

	virtual color get_color () const throw ();

	//! The display color, if defined.
	color value;

protected:

	virtual ustring do_get_string (const ustring& format) const throw ();
};

} // namespace Elemental

#endif // LIBELEMENTAL__VALUE_TYPES_HH
