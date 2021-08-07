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
#include <libelemental/element.hh>
#include <libelemental/data.hh>
#include <libelemental/table.hh>

#include <cmath>

namespace Elemental {


//******************************************************************************
// class PropertyBase


PropertyBase::PropertyBase (const Message& name_, const Message& format_,
	const Message& description_, const Message* sources_[]) throw ()
:	name (name_), format (format_), description (description_)
{
	while (sources_ != NULL && *sources_ != NULL)
		sources.push_back (*sources_++);
}


PropertyBase::~PropertyBase ()
{}


ustring
PropertyBase::get_name () const throw ()
{
	return name.get_string ();
}


bool
PropertyBase::has_format () const throw ()
{
	return format.has_value ();
}


ustring
PropertyBase::get_format () const throw ()
{
	return has_format () ? format.get_string () : ustring ();
}


ustring
PropertyBase::get_description () const throw ()
{
	return description.get_string ();
}


void
PropertyBase::make_entry (EntriesView& view, const value_base& value) const
	throw ()
{
	value.make_entry (view,
		// Translators: This is the format for indicating the name of an entry.
		compose::ucompose (_("%1:"), get_name ()),
		get_format ());
}


void
PropertyBase::make_entry (EntriesView& view, const ustring& value) const
	throw ()
{
	make_entry (view, String (value));
}


bool
PropertyBase::is_colorable () const throw ()
{
	try
	{
		return CAST (&get_table ().front ()->get_property_base (*this),
			const color_value_base);
	}
	catch (const std::invalid_argument &)
		{ return false; }
}


//******************************************************************************
// class Property<Float>


Property<Float>::Property (const Message& name_, const Message& format_,
	const Message& description_, const Message* sources_[]) throw ()
:	PropertyBase (name_, format_, description_, sources_), have_values (false)
{}


Property<Float>::~Property ()
{}


bool
Property<Float>::is_scale_valid () const throw ()
{
	return have_values && (minimum < maximum);
}


double
Property<Float>::get_scale_position (const Float& instance, bool logarithmic) const
{
	if (!is_scale_valid ()) throw std::domain_error ("invalid scale");
	if (!instance.has_value ()) throw std::invalid_argument ("undefined value");

	double value = logarithmic ? std::log10 (instance.value) : instance.value;
	double min = logarithmic ? std::log10 (minimum) : minimum;
	double max = logarithmic ? std::log10 (maximum) : maximum;

	return (value - min) / (max - min);
}


double
Property<Float>::get_minimum () const
{
	if (!have_values) throw std::domain_error ("invalid scale");
	return minimum;
}


double
Property<Float>::get_medium (bool logarithmic) const
{
	if (!is_scale_valid ()) throw std::domain_error ("invalid scale");
	
	if (logarithmic)
		return std::pow (10,
			(std::log10 (maximum) + std::log10 (minimum)) / 2.0);
	else
		return (minimum + maximum) / 2.0;
}


double
Property<Float>::get_maximum () const
{
	if (!have_values) throw std::domain_error ("invalid scale");
	return maximum;
}


void
Property<Float>::process_value (const Float& instance) throw ()
{
	if (!instance.has_value ()) return;
	
	if (have_values)
	{
		if (minimum > instance.value)
			minimum = instance.value;
		if (maximum < instance.value)
			maximum = instance.value;
	}
	else
	{
		minimum = maximum = instance.value;
		have_values = true;
	}
}


bool
Property<Float>::is_colorable () const throw ()
{
	return is_scale_valid ();
}


//******************************************************************************
// class Category


Category::Category (const Message& name_, PropertyBase* properties_[]) throw ()
:	name (name_)
{
	while (properties_ != NULL && *properties_ != NULL)
		properties.push_back (*properties_++);
}


Category::~Category ()
{}


ustring
Category::get_name () const throw ()
{
	return name.get_string ();
}


void
Category::make_header (EntriesView& view) const throw ()
{
	view.header (get_name ());
}


//******************************************************************************
// class Element


Element::Element (const ElementData& data_) throw ()
:	symbol (data_.symbol), number (data_.number), data (data_),
	official_name (Q_NA), standard_phase (get_phase ()),
	lattice_volume (get_lattice_volume ())
{
	if (data.name.has_value () &&
		data.name.value != data.name.get_string ())
		official_name = String (data.name.value);
}


const value_base&
Element::get_property_base (const PropertyBase& property) const
{
#define retb(PROP, value) if (prop == &P_##PROP) return value;
#define ret(PROP, member) retb (PROP, data.member)

	const PropertyBase *prop = &property;

	if (prop == &P_SYMBOL || prop == &P_NUMBER)
		throw std::invalid_argument ("not a value property");

	ret (NAME, name)
	retb (OFFICIAL_NAME, official_name)
	ret (ALTERNATE_NAME, alternate_name)
	ret (SERIES, series)
	ret (GROUP, group)
	ret (PERIOD, period)
	ret (BLOCK, block)

	ret (DISCOVERY, discovery)
	ret (DISCOVERED_BY, discovered_by)
	ret (ETYMOLOGY, etymology)

	retb (PHASE, standard_phase)
	ret (DENSITY_SOLID, density_solid)
	ret (DENSITY_LIQUID, density_liquid)
	ret (DENSITY_GAS, density_gas)
	ret (APPEARANCE, appearance)

	ret (MELTING_POINT, melting_point)
	ret (BOILING_POINT, boiling_point)
	ret (FUSION_HEAT, fusion_heat)
	ret (VAPORIZATION_HEAT, vaporization_heat)
	ret (SPECIFIC_HEAT, specific_heat)
	ret (THERMAL_CONDUCTIVITY, thermal_conductivity)
	ret (DEBYE_TEMPERATURE, debye_temperature)

	ret (ATOMIC_MASS, atomic_mass)
	ret (ATOMIC_VOLUME, atomic_volume)
	ret (ATOMIC_RADIUS, atomic_radius)
	ret (COVALENT_RADIUS, covalent_radius)
	ret (VAN_DER_WAALS_RADIUS, van_der_waals_radius)
	ret (IONIC_RADII, ionic_radii)

	ret (LATTICE_TYPE, lattice_type)
	ret (SPACE_GROUP, space_group)
	ret (LATTICE_EDGES, lattice_edges)
	ret (LATTICE_ANGLES, lattice_angles)
	retb (LATTICE_VOLUME, lattice_volume)

	ret (CONFIGURATION, configuration)
	ret (OXIDATION_STATES, oxidation_states)
	ret (ELECTRONEGATIVITY, electronegativity)
	ret (ELECTRON_AFFINITY, electron_affinity)
	ret (FIRST_ENERGY, first_energy)

	ret (COLOR, color)
	ret (NOTES, notes)

	throw std::invalid_argument ("unknown property");

#undef ret
#undef retb
}


void
Element::make_entries (EntriesView& view) const throw ()
{
	FOREACH (std::list<Category*>, CATEGORIES, category)
		make_entries (view, **category, true);
}


void
Element::make_entries (EntriesView& view, const Category& category, bool all)
	const throw ()
{
	if (&category == &C_GENERAL)
	{
		view.header (compose::ucompose (_("%1 Properties"),
			data.name.get_string ()));
		if (all)
			P_NAME.make_entry (view, data.name);
		P_OFFICIAL_NAME.make_entry (view, official_name);
		if (data.alternate_name.has_value () &&
			data.alternate_name.value != data.name.get_string ())
			P_ALTERNATE_NAME.make_entry (view, data.alternate_name);
		if (all)
		{
			P_SYMBOL.make_entry (view, symbol);
			P_NUMBER.make_entry (view, compose::ucompose1 (number));
		}
		P_SERIES.make_entry (view, data.series);
		P_GROUP.make_entry (view, data.group);
		P_PERIOD.make_entry (view, data.period);
		P_BLOCK.make_entry (view, data.block);
	}
	else if (&category == &C_MISCELLANEOUS)
	{
		category.make_header (view);
		P_COLOR.make_entry (view, data.color);
		if (data.notes.has_value ())
			data.notes.make_entry (view, ustring ());
	}
	else
	{
		category.make_header (view);
		CONST_FOREACH (std::list<PropertyBase*>, category.properties, prop)
			(*prop)->make_entry (view, get_property_base (**prop));
	}
}


Phase
Element::get_phase (double tempK) const throw ()
{
	if (data.boiling_point.has_value ())
	{
		if (data.boiling_point.value <= tempK)
			return Phase::GAS;
		else if (data.melting_point.has_value () &&
				data.melting_point.value <= tempK)
			return Phase::LIQUID;
		else
			return Phase::SOLID;
	}
	else if (data.melting_point.has_value ())
	{
		if (data.melting_point.value <= tempK)
			return Q_UNK;
		else
			return Phase::SOLID;
	}
	else
		return Q_UNK;
}


Float
Element::get_lattice_volume () const throw ()
{
	if (!data.lattice_type.has_value () ||
		!data.lattice_edges.has_value () ||
		!data.lattice_angles.has_value ())
		return Q_UNK;

	static const double DEG_RAD = 0.017453293, PM_NM = 0.001;

	double a = data.lattice_edges.values[0] * PM_NM,
		b = data.lattice_edges.values[1] * PM_NM,
		c = data.lattice_edges.values[2] * PM_NM,
		alpha = data.lattice_angles.values[0] * DEG_RAD,
		beta = data.lattice_angles.values[1] * DEG_RAD,
		gamma = data.lattice_angles.values[2] * DEG_RAD,
		result;

	switch (data.lattice_type.value)
	{
	case LatticeType::TRI:
	case LatticeType::RHO:
		result = a * b * c * std::sqrt (1 - std::pow (std::cos (alpha), 2.0) -
			std::pow (std::cos (beta), 2.0) - std::pow (std::cos (gamma), 2.0) +
			2.0 * std::cos (alpha) * std::cos (beta) * std::cos (gamma));
		break;
	case LatticeType::MONO:
		result = a * b * c * std::sin (beta);
		break;
	case LatticeType::ORTH:
	case LatticeType::TET:
	case LatticeType::SC:
	case LatticeType::BCC:
	case LatticeType::FCC:
		result = a * b * c;
		break;
	case LatticeType::HEX:
		result = (std::sqrt (3.0) * a * b * c) / 2.0;
		break;
	default:
		return Q_UNK;
	}
	
	if (!std::isfinite (result)) return Q_UNK;
	return Float (std::floor (result / PM_NM) * PM_NM, Q_CA);
}


} // namespace Elemental
