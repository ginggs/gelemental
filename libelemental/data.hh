//! \file data.hh
//! Private data structures.
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

#ifndef LIBELEMENTAL__DATA_HH
#define LIBELEMENTAL__DATA_HH

#include <libelemental/value.hh>
#include <libelemental/value-types.hh>

//******************************************************************************

namespace Elemental {

//******************************************************************************

/* Some categories and properties appear out of order here for historical
 * reasons. This header file is not part of the public interface, and this
 * structure may be reordered at any time. */
struct ElementData
{
// General

	Message name;
	String alternate_name;
	std::string symbol;
	AtomicNumber number;
	Series series;
	Int group;
	Int period;
	Block block;

// Physical

	Float density_solid;
	Float density_liquid;
	Float density_gas;
	Message appearance;

// Historical

	Event discovery;
	Message discovered_by;
	Message etymology;

// Atomic

	Float atomic_mass;
	Float atomic_volume;
	Float atomic_radius;
	Float covalent_radius;
	Float van_der_waals_radius;
	String ionic_radii;
	
// Crystallographic

	LatticeType lattice_type;
	Int space_group;
	FloatList lattice_edges;
	FloatList lattice_angles;

// Electronic

	String configuration;
	IntList oxidation_states;
	Float electronegativity;
	Float electron_affinity;
	Float first_energy;

// Thermal

	Float melting_point;
	Float boiling_point;
	Float fusion_heat;
	Float vaporization_heat;
	Float specific_heat;
	Float thermal_conductivity;
	Float debye_temperature;

// Miscellaneous

	ColorValue color;
	Message notes;
};

extern const ElementData table_data[];
extern const unsigned int table_length;

} // namespace Elemental

#endif // LIBELEMENTAL__DATA_HH
