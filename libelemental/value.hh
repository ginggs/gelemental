//! \file value.hh
//! Basic definitions and structures, and base value classes. */
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

#ifndef LIBELEMENTAL__VALUE_HH
#define LIBELEMENTAL__VALUE_HH

#include <libelemental/misc/ucompose.hh>
#include <libelemental/misc/extras.hh>

#include <iomanip>
#include <ostream>
#include <vector>
#include <glibmm/refptr.h>
#include <glibmm/ustring.h>

//******************************************************************************

//! A periodic table library with detailed information on elements.
namespace Elemental {

using Glib::RefPtr;
using Glib::ustring;

//! The standard temperature, in Kelvin, as defined by IUPAC.
extern const double STANDARD_TEMPERATURE;

//! A qualification of the meaning of a value.
enum Qualifier
{
	Q_NEUTRAL, //!< Indicates a presumably valid value.
	Q_UNK, //!< Indicates that a value is unknown or unavailable.
	Q_NA, //!< Indicates that a value is not applicable for an element.
	Q_EST, //!< Indicates that a value is estimated or calculated.
	Q_CA, //!< Indicates that a value is approximate.
	Q_ISO //!< Indicates a value for the most stable isotope of an element.
};

//! An atomic number.
typedef unsigned int AtomicNumber;

//******************************************************************************

//! A color description.
//! Its components are suitable as parameters to Gdk::Color::set_rgb_p().
struct color
{
	//! Constructs a new color with red, green, and blue percentages.
	explicit color (double red = 0.0, double green = 0.0, double blue = 0.0)
		throw ();

	//! Constructs a new color with red, green, and blue levels.
	//! Each component ranges from 0 (0.0) to 65535 (1.0).
	color (unsigned int red, unsigned int green, unsigned int blue) throw ();

	//! The red component of the color in the RGB space.
	double red;

	//! The green component of the color in the RGB space.
	double green;

	//! The blue component of the color in the RGB space.
	double blue;

	//! Returns the luminous intensity of the color.
	double get_luminance () const throw ();

	//! Returns a grayscale color complimentary in luminance to this color.
	color get_compliment () const throw ();
	
	//! Returns a composite of this color and another.
	/*! \param other A color with which this color will be composited.
	 * \param alpha The level of the other color in the composite, between 0.0
	 * and 1.0. */
	color composite (const color& other, double alpha) const throw ();
	
	//! Returns a hexadecimal specification of the color ("#rrggbb" format).
	std::string get_hex_spec () const throw ();
};

//******************************************************************************

//! An interface for the display of value entries.
class EntriesView
{
public:

	virtual ~EntriesView ();

	//! Displays a category header.
	//! \param category The name of the category.
	virtual void header (const ustring& category) throw () = 0;

	//! Displays a single value entry.
	/*! \param name The name of the entry.
	 * \param value A localized string representation of the value of the entry.
	 * \param tip Qualifying information, if any, to be displayed as a tip. */
	virtual void entry (const ustring& name, const ustring& value,
		const ustring& tip = ustring ()) throw () = 0;

protected:

	//! Returns the greatest expected length of an entry name.
	//! This value is calculated from the lengths of translated property names.
	static ustring::size_type get_max_name_length () throw ();

private:

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	friend void initialize () throw ();
#endif

	static void accommodate_name (ustring::size_type length) throw ();
	
	static ustring::size_type max_name_length;
};

//******************************************************************************

//! An adapter for the display of value entries on an output stream.
class EntriesStream
:	public EntriesView
{
public:

	//! Constructs a new EntriesStream for an open FILE pointer.
	EntriesStream (FILE* file) throw ();

	//! Constructs a new EntriesStream for an open file descriptor.
	EntriesStream (int fd) throw ();

	//! Constructs a new EntriesStream for a stream buffer.
	EntriesStream (std::streambuf& buf) throw ();

	//! Constructs a new EntriesStream for an output stream.
	EntriesStream (std::ostream& os) throw ();
	
	virtual ~EntriesStream ();

	virtual void header (const ustring& category) throw ();

	virtual void entry (const ustring& name, const ustring& value,
		const ustring& tip = ustring ()) throw ();

private:

	std::streambuf *buf;
	bool buf_owned;

	std::ostream &os;
	bool os_owned;
};

//******************************************************************************

//! A base class for qualified values.
class value_base
{
public:

	virtual ~value_base ();

	//! The qualification of the value's meaning.
	Qualifier qualifier;
	
	//! Returns whether the value is defined.
	//! \return \c false if the value is unknown or inapplicable; else \c true
	bool has_value () const throw ();

	//! Returns a localized text representation of the value.
	/*! \param format An optional compose-style format string with which to
	 * output the value. If not otherwise specified, the value is argument
	 * reference \c %%1.
	 * \return A UTF-8 string containing the localized representation. */
	virtual ustring get_string (const ustring& format = ustring ()) const
		throw ();

	//! Returns a localized message to be displayed as a tooltip.
	/*! The message may also be displayed parenthetically to the value.
	 * \return A UTF-8 string containing the tip text, if any. */
	ustring get_tip () const throw ();

	//! Possibly adds the result of get_string() to an EntriesView.
	/*! An entry will only be made if the value is defined or if \p always is
	 * \c true.
	 * \param view The EntriesView to be populated.
	 * \param name The name of the value to be passed.
	 * \param format An optional compose-style format for get_string(). */
	void make_entry (EntriesView& view, const ustring& name,
		const ustring& format = ustring ()) const throw ();

	//! Compares two qualified values.
	/*! Derived classes may override this function, first calling compare_base,
	 * and use a dynamic_cast to check other.
	 * \param other The qualified value for comparison.
	 * \return \c -1 if this value is undefined or is less than \p other; \c 0
	 * if the values are equal; or \c 1 if \p other is undefined or this value
	 * is greater than \p other. */
	virtual int compare (const value_base& other) const throw ();

protected:

	//!
	//! \param qualifier The qualification of the initial value.
	value_base (Qualifier qualifier) throw ();

	//! Returns a localized text representation of a defined value.
	/*! Derived classes must implement this function to produce a text
	 * representation based on the format. It will be called by get_string(),
	 * which will then decorate its output as appropriate for the current
	 * qualifier. If has_value() is \c false, the behavior is undefined.
	 * \param format A compose-style format string. Derived classes may
	 * define the number of argument references expected in the string; the
	 * default is one. If \p format is empty, a minimal default should be used.
	 * \return A UTF-8 string containing the localized representation. */
	virtual ustring do_get_string (const ustring& format) const throw () = 0;

	//! Compares the qualifications of two values.
	/*! This base function handles definedness only. Derived classes may
	 * override this function to handle qualifiers more specifically.
	 * \param other The qualified value for comparison.
	 * \return \c -1 if this value sorts before \p other; \c 0 if the values
	 * sort equally; \c 1 if this value sorts after \p other; or
	 * \p YIELD_COMPARE if the result should be determined based on values. */
	virtual int compare_base (const value_base& other) const throw ();
	
	//! Indicates that comparison should be based on values, not definedness.
	//! Returned by compare_base() when qualifiers are equivalent.
	static const int YIELD_COMPARE;

	//! Whether an undefined value should make an entry.
	/*! May be set by derived classes.
	 * If set to \c false (default), make_entry() will only populate an
	 * EntriesView if has_value() returns \c true. If set to \c true, an
	 * EntriesView will always be populated. */
	bool always;
};

//******************************************************************************

//! A base class for qualified values which have representative colors.
class color_value_base
:	public value_base
{
public:

	virtual ~color_value_base ();

	//! Returns a color representing the value.
	/*! Derived classes must implement this function, returning a color that
	 * corresponds to the defined value. A special color should be returned if
	 * has_value() is \c false.
	 * \return A color structure. */
	virtual color get_color () const throw () = 0;

protected:

	//!
	//! \param qualifier The qualification of the initial value.
	color_value_base (Qualifier qualifier) throw ();
};

//******************************************************************************

//! A qualified value of a generic type.
/*! This template requires a class with default and copy constructors and
 * less-than, greater-than, and stream insertion operators. Compiler-generated
 * functions are acceptable. */
template<class T>
class Value
:	public value_base
{
public:

	//! Constructs a new defined value.
	/*! \param source The initial definition.
	 * \param qualifier The qualification of the initial value. */
	template<class S> Value (const S& source, Qualifier qualifier = Q_NEUTRAL);

	//! Constructs a new value based on another value.
	//! \param source The value to be copied.
	template<class S> Value (const Value<S>& source);

	//! Constructs a new undefined or default value.
	//! \param qualifier The qualification of the initial value.
	Value (Qualifier qualifier = Q_UNK) throw ();
	
	virtual ~Value ();

	virtual int compare (const value_base& other) const throw ();

	//! The value, if defined.
	T value;

protected:

	virtual ustring do_get_string (const ustring& format) const throw ();
};

//******************************************************************************

#ifndef DOXYGEN_SHOULD_SKIP_THIS
//! Returns a series of characters separating members of a list in output.
//! For internal use.
ustring get_list_separator () throw ();
#endif

//! A qualified list of values of a generic type.
/*! This template requires a class with default and copy constructors and
 * less-than, greater-than, and stream insertion operators. Compiler-generated
 * functions are acceptable. */
template<class T>
class ValueList
:	public value_base
{
public:

	//! Constructs a new defined value list.
	/*! \param source An array of the initial values.
	 * \param count The length of the \p source array.
	 * \param qualifier The qualification of the initial values. */
	template<class S> ValueList (const S source[], int count,
		Qualifier qualifier = Q_NEUTRAL);

	//! Constructs a new value list based on another value list.
	//! \param source The value list to be copied.
	template<class S> ValueList (const ValueList<S>& source);

	//! Constructs a new undefined or empty value list.
	//! \param qualifier The qualification of the initial values.
	ValueList (Qualifier qualifier = Q_UNK) throw ();
	
	virtual ~ValueList ();

	virtual int compare (const value_base& other) const throw ();

	//! The values, if defined.
	mutable std::vector<T> values;

protected:

	virtual ustring do_get_string (const ustring& format) const throw ();
};

} // namespace Elemental

#include <libelemental/value.tcc>

#endif // LIBELEMENTAL__VALUE_HH
