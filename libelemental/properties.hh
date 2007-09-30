//! \file properties.hh
//! Categories and properties.
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

#ifndef LIBELEMENTAL__PROPERTIES_HH
#define LIBELEMENTAL__PROPERTIES_HH

#ifndef LIBELEMENTAL__ELEMENT_HH
#error "properties.hh must be included from element.hh."
#endif

#include <list>

//******************************************************************************

namespace Elemental {

//! The categories of available properties of the chemical elements.
extern std::list<Category*> CATEGORIES;

//! \defgroup Properties Properties
//@{

//******************************************************************************
//! \name General Properties
//@{

extern Category C_GENERAL;
/* Translators: This should be translated to the name most commonly used in
 * scientific contexts in your language. If this is different from the
 * official name, the official name will be presented untranslated in a
 * second entry. */
extern Property<Message> P_NAME;
extern Property<String> P_OFFICIAL_NAME;
/* Translators: This name is not available for translation. It will not be
 * presented if it is equal to the translated primary name. */
extern Property<String> P_ALTERNATE_NAME;
extern Property<std::string> P_SYMBOL;
extern Property<AtomicNumber> P_NUMBER;	
extern Property<Series> P_SERIES;
extern Property<Int> P_GROUP;
extern Property<Int> P_PERIOD;
extern Property<Block> P_BLOCK;

//@}
//******************************************************************************
//! \name Historical Properties
//@{

extern Category C_HISTORICAL;
extern Property<Event> P_DISCOVERY;
extern Property<Message> P_DISCOVERED_BY;
// Translators: See po/README for translation issues.
extern Property<Message> P_ETYMOLOGY;

//@}
//******************************************************************************
//! \name Physical Properties
//@{

extern Category C_PHYSICAL;
extern Property<Phase> P_PHASE;
extern Property<Float> P_DENSITY_SOLID;
extern Property<Float> P_DENSITY_LIQUID;
extern Property<Float> P_DENSITY_GAS;
extern Property<Message> P_APPEARANCE;

//@}
//******************************************************************************
//! \name Thermal Properties
//@{

extern Category C_THERMAL;
extern Property<Float> P_MELTING_POINT;
extern Property<Float> P_BOILING_POINT;
extern Property<Float> P_FUSION_HEAT;
extern Property<Float> P_VAPORIZATION_HEAT;
extern Property<Float> P_SPECIFIC_HEAT;
extern Property<Float> P_THERMAL_CONDUCTIVITY;
extern Property<Float> P_DEBYE_TEMPERATURE;

//@}
//******************************************************************************
//! \name Atomic Properties
//@{

extern Category C_ATOMIC;
extern Property<Float> P_ATOMIC_MASS;
extern Property<Float> P_ATOMIC_VOLUME;
extern Property<Float> P_ATOMIC_RADIUS;
extern Property<Float> P_COVALENT_RADIUS;
extern Property<Float> P_VAN_DER_WAALS_RADIUS;
extern Property<String> P_IONIC_RADII;

//@}
//******************************************************************************
//! \name Crystallographic Properties
//@{

extern Category C_CRYSTALLOGRAPHIC;
extern Property<LatticeType> P_LATTICE_TYPE;
extern Property<Int> P_SPACE_GROUP;
extern Property<FloatList> P_LATTICE_EDGES;
extern Property<FloatList> P_LATTICE_ANGLES;
extern Property<Float> P_LATTICE_VOLUME;

//@}
//******************************************************************************
//! \name Electronic Properties
//@{

extern Category C_ELECTRONIC;
// Superscripts should be denoted by the tag <tt>\<sup\>\</sup\></tt>.
extern Property<String> P_CONFIGURATION;
extern Property<IntList> P_OXIDATION_STATES;
extern Property<Float> P_ELECTRONEGATIVITY;
extern Property<Float> P_ELECTRON_AFFINITY;
extern Property<Float> P_FIRST_ENERGY;

//@}
//******************************************************************************
//! \name Miscellaneous Properties
//@{

extern Category C_MISCELLANEOUS;
extern Property<ColorValue> P_COLOR;
extern Property<Message> P_NOTES;

//@}
//@}

} // namespace Elemental

#endif // LIBELEMENTAL__PROPERTIES_HH
