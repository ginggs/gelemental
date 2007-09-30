//! \file element.hh
//! Property, Category, and Element classes.
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

#ifndef LIBELEMENTAL__ELEMENT_HH
#define LIBELEMENTAL__ELEMENT_HH

#include <libelemental/value.hh>
#include <libelemental/value-types.hh>

#include <list>
#include <stdexcept>
#include <vector>

//******************************************************************************

namespace Elemental {

#ifndef DOXYGEN_SHOULD_SKIP_THIS
struct ElementData; // defined in data.hh
#endif

//******************************************************************************

//! A property of the chemical elements.
class PropertyBase
{
public:

	PropertyBase (const Message& name, const Message& format,
		const Message& description, const Message* sources[]) throw ();

	virtual ~PropertyBase ();

	//! Returns the translated name of the property.
	ustring get_name () const throw ();
	
	//! Returns whether a compose-style format for values is defined.
	bool has_format () const throw ();
	
	//! Returns a translated compose-style format for values of the property.
	//! If the format is undefined, returns the empty string.
	ustring get_format () const throw ();

	//! Returns a translated explanatory description of the property.
	ustring get_description () const throw ();
	
	//! Citations for the sources of values for the property.
	std::list<const Message*> sources;

	//! Possibly adds a value for this property to an EntriesView.
	/*! An entry will only be made if the value is defined or if its \p always
	 * is \c true.
	 * \param view The EntriesView to be populated.
	 * \param value The value to be added. */
	void make_entry (EntriesView& view, const value_base& value) const throw ();

	//! Adds a string representing a value for this property to an EntriesView.
	/*! \param view The EntriesView to be populated.
	 * \param value The value string to be added. */
	void make_entry (EntriesView& view, const ustring& value) const throw ();
	
	//! Returns whether values of the property have color representations.
	//! If \c true, values may or may not be color_value_base descendants.
	virtual bool is_colorable () const throw ();

protected:

	//! The untranslated name of the property.
	Message name;

	//! An untranslated compose-style format for values of the property.
	Message format;

	//! An untranslated explanatory description of the property.
	Message description;
};

//******************************************************************************

//! A typed property of the chemical elements.
template<class T>
class Property
:	public PropertyBase
{
public:

	inline Property (const Message& name, const Message& format,
		const Message& description, const Message* sources[]) throw ();

	virtual ~Property ();
};

//! A floating-point property of the chemical elements.
template<>
class Property<Float>
:	public PropertyBase
{
public:

	Property (const Message& name, const Message& format,
		const Message& description, const Message* sources[]) throw ();

	virtual ~Property ();
	
	//! Returns whether the scale of values for this property is valid.
	/*! The scale is valid if one or more elements has a defined value for this
	 * property and those values are not all equal. */
	bool is_scale_valid () const throw ();

	//! Returns the position on a normalized 0.0 to 1.0 scale for a value.
	/*! \param instance A defined value for this property.
	 * \param logarithmic Whether to compute the position on a logarithmic
	 * instead of linear scale.
	 * \throw std::domain_error if the property does not have a valid scale.
	 * \throw std::invalid_argument if instance is not a defined value. */
	double get_scale_position (const Float& instance, bool logarithmic = false)
		const throw (std::domain_error, std::invalid_argument);

	//! Returns the least value of this property for any element.
	//! \throw std::domain_error if the property does not have a valid scale.
	double get_minimum () const throw (std::domain_error);

	//! Returns the value of this property that has a scale position of 0.5.
	/*! \param logarithmic Whether to compute the value on a logarithmic scale.
	 * \throw std::domain_error if the property does not have a valid scale. */
	double get_medium (bool logarithmic = false) const
		throw (std::domain_error);

	//! Returns the greatest value of this property for any element.
	//! \throw std::domain_error if the property does not have a valid scale.
	double get_maximum () const throw (std::domain_error);

	virtual bool is_colorable () const throw ();

private:

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	friend void initialize () throw ();
#endif

	void process_value (const Float& instance) throw ();

	bool have_values;
	double minimum;
	double maximum;
};

//! A floating-point property of the chemical elements.
typedef Property<Float> FloatProperty;

//******************************************************************************

//! A category of properties of the chemical elements.
class Category
{
public:

	Category (const Message& name, PropertyBase* properties[]) throw ();

	virtual ~Category ();

	//! Returns the translated name of the category.
	ustring get_name () const throw ();
	
	//! The properties in the category.
	std::list<PropertyBase*> properties;
	
	//! Adds the category name to an EntriesView.
	//! \param view The EntriesView to be populated.
	void make_header (EntriesView& view) const throw ();

protected:

	//! The untranslated name of the category.
	Message name;
};

//******************************************************************************

//! A chemical element.
//! The element properties are named in the Properties module.
class Element
{
public:
	
	//! Returns the value_base of the value for the given Property.
	//! \param property The Element Property to be returned.
	const value_base& get_property_base (const PropertyBase& property) const
		throw (std::invalid_argument);

	//! Returns the value for the given Property.
	//! \param property The Element Property to be returned.
	template<class T> inline const T& get_property (const Property<T>& property)
		const throw (std::invalid_argument);

	//! Generates entries for all properties.
	//! \param view An EntriesView to be populated.
	void make_entries (EntriesView& view) const throw ();

	//! Generates entries for the properties in the given Category.
	/*! \param view An EntriesView to be populated.
	 * \param category The Category whose properties will be provided.
	 * \param all For \p C_GENERAL, controls whether the EntriesView will also
	 * receive the element's primary name, symbol, and atomic number. */
	void make_entries (EntriesView& view, const Category& category,
		bool all = false) const throw ();

	//! Returns the phase of matter assumed by the element at a temperature.
	/*! Standard pressure is implied.
	 * \param tempK The temperature, in Kelvin, at which to determine phase. By
	 * default, the IUPAC standard temperature.
	 * \return The phase of matter determined. */
	Phase get_phase (double tempK = STANDARD_TEMPERATURE) const throw ();

	//! The chemical symbol for this element.
	std::string symbol;
	
	//! The atomic number of this element.
	AtomicNumber number;

private:

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	friend void initialize () throw ();
#endif

	Element (const ElementData& data) throw ();

	const ElementData& data;
	String official_name;
	Phase standard_phase;
	Float lattice_volume;
	
	Float get_lattice_volume () const throw ();
};

} // namespace Elemental

// formerly part of this file; required by element.tcc
#include <libelemental/properties.hh>

#include <libelemental/element.tcc>

#endif // LIBELEMENTAL__ELEMENT_HH
