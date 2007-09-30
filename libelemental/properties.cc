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
// previous includes properties.hh

namespace Elemental {


#ifndef DOXYGEN_SHOULD_SKIP_THIS // normal

#	define Src(SRC, desc) static const Message SRC = desc
#	define Cat(CAT, name, ...) \
Category C_##CAT (name, (PropertyBase*[]) { __VA_ARGS__, NULL })
#	define Prop(PROP, type, name, format, desc, ...) Property<type> P_##PROP \
(name, format, desc, (const Message*[]) { __VA_ARGS__, NULL})

#else // Doxygen

#	define Src(SRC, desc)
#	define Cat(CAT, name, ...) \
DOXYGEN_OPEN <summary>\ref IgnoreThisError name</summary> DOXYGEN_CLOSE \
Category C_##CAT;
#	define Prop(PROP, type, name, format, desc, ...) \
DOXYGEN_OPEN <summary>\ref IgnoreThisError name</summary> \
\ref IgnoreThisError desc DOXYGEN_CLOSE \
Property<type> P_##PROP;

#endif



//******************************************************************************
// Sources


Src (BODR_CONSENSUS, N_("Consensus of the Blue Obelisk project."));
Src (CALCULATED, N_("Calculated or based on other properties."));
Src (IUPAC_REC, N_("IUPAC recommendation."));
Src (UNSOURCED, N_("Currently unsourced."));


Src (ALLRED_1961, "Allred, A. L. 1961, <i>J. Inorg. Nucl. Chem.</i>, vol. 17, p. 215.");
Src (BATSANOV_2001, "Batsanov, S. S. 2001, <i>Inorganic Materials</i>, vol. 37, no. 9, pp. 871-885.");
Src (BONDI_1964, "Bondi, A. 1964, <i>J. Phys. Chem.</i>, vol. 68, p. 441.");
Src (CLEMENTI_1963, "Clementi, E., Raimondi, D. L., &amp; Reinhardt, W. P. 1963, <i>J. Chem. Phys.</i> vol. 38, p. 2686.");
Src (DEAN_1992, "Dean, J. A. (ed.) 1992, <i>Lange's Handbook of Chemistry</i>, 14th edn, McGraw-Hill, New York.");
Src (DEAN_1999, "Dean, J. A. (ed.) 1999, <i>Lange's Handbook of Chemistry</i>, 15th edn, McGraw-Hill, New York.");
Src (ELLIS_1972, "Ellis, H. (ed.) 1972, <i>Nuffield Advanced Science Book of Data</i>, Longman, London.");
Src (HO_1974, "Ho, C. Y., Powell, R. W., &amp; Liley, P. E. 1974, <i>J. Phys. Chem. Ref. Data</i>, vol. 3, suppl. 1.");
Src (HUHEEY_1993, "Huheey, J. E., Keiter, E. A., &amp; Keiter, R. L. 1993, <i>Inorganic Chemistry: Principles of Structure and Reactivity</i>, 4th edn, HarperCollins, New York.");
Src (JAMES_1992, "James, A. M. &amp; Lord, M. P. 1992, <i>Macmillan's Chemical and Physical Data</i>, Macmillan, London.");
Src (KAYE_1993, "Kaye, G. W. C. &amp; Laby, T. H. 1993, <i>Tables of physical and chemical constants</i>, 15th edn, Longman, London.");
Src (KUCHLING_1991, "Kuchling, Horst. 1991, <i>Taschenbuch der Physik</i>, 13th edn, Verlag Harri Deutsch, Thun und Frankfurt/Main.");
Src (LIDE_1996, "Lide, D. R. (ed.) 1996, <i>Chemical Rubber Company handbook of chemistry and physics</i>, 77th edn, CRC Press, Boca Raton, Florida.");
Src (LIDE_1998, "Lide, D. R. (ed.) 1998, <i>Chemical Rubber Company handbook of chemistry and physics</i>, 79th edn, CRC Press, Boca Raton, Florida.");
Src (LIDE_2003, "Lide, D. R. (ed.) 2003, <i>Chemical Rubber Company handbook of chemistry and physics</i>, 84th edn, CRC Press, Boca Raton, Florida.");
Src (PAULING_1960, "Pauling, L. 1960, <i>The Nature of the Chemical Bond</i>, 3rd edn, Cornell Univ., USA.");
Src (PORTERFIELD_1984, "Porterfield, W. W. 1984, <i>Inorganic Chemistry: A Unified Approach</i>, Addison-Wesley, Reading, Massachusetts.");
Src (PURE_APPL_1997, "Pure Appl. Chem. 1997, vol. 69, iss. 12, pp. 2471-2473.");
Src (PURE_APPL_2003, "Pure Appl. Chem. 2003, vol. 75, iss. 10, pp. 1613-1615.");
Src (PURE_APPL_2004, "Pure Appl. Chem. 2004, vol. 76, iss. 12, pp. 2101-2103.");
Src (SANDERSON_1962, "Sanderson, R.T. 1962, <i>Chemical Periodicity</i>, Reinhold, New York.");
Src (SLATER_1964, "Slater, J. C. 1964, <i>J. Chem. Phys.</i>, vol. 41, p. 3199.");
Src (SUTTON_1965, "Sutton, L. E. 1965, <i>Table of Interatomic Distances and Configuration in Molecules and Ions</i>, suppl. 1956-1959, spec. pub. no. 18, Chemical Society, London.");
Src (WEBELEMENTS_XTAL, "Multiple sources as compiled at <i>WebElements</i>, http://www.webelements.com/.");
Src (WIESER_2006, "Wieser, M. E. 2006, <i>Pure Appl. Chem.</i>, vol. 78, iss. 11, pp. 2051-2066.");
Src (WIKIPEDIA_AFFINITY, "Multiple sources as compiled at Wikipedia, <i>Electron affinity (data page)</i>, http://en.wikipedia.org/wiki/Electron_affinity_%28data_page%29 (as of 2007 Jan. 29, 06:21 GMT).");


//******************************************************************************
// General


Prop (NAME, Message, N_("Name"), Q_NA,
	N_("The name most commonly used for the element in scientific contexts in the display language."),
	&PURE_APPL_1997, &PURE_APPL_2003, &PURE_APPL_2004);

Prop (OFFICIAL_NAME, String, N_("Official name"), Q_NA,
	N_("The official IUPAC English name of the element."),
	&IUPAC_REC);

Prop (ALTERNATE_NAME, String, N_("Alternate name"), Q_NA,
	N_("A recognized alternate name for the element, if any."),
	&IUPAC_REC);

Prop (SYMBOL, std::string, N_("Symbol"), Q_NA,
	N_("The chemical symbol for the element."),
	&PURE_APPL_1997, &PURE_APPL_2003, &PURE_APPL_2004);

Prop (NUMBER, AtomicNumber, N_("Atomic number"), Q_NA,
	N_("The atomic number of the element."),
	&IUPAC_REC);

Prop (SERIES, Series, N_("Series"), Q_NA,
	N_("The chemical series in which the element is generally classified."),
	&UNSOURCED);

Prop (GROUP, Int, N_("Group"), Q_NA,
	N_("The periodic table group to which the element belongs, if any."),
	&IUPAC_REC);

Prop (PERIOD, Int, N_("Period"), Q_NA,
	N_("The periodic table period to which the element belongs."),
	&IUPAC_REC);

Prop (BLOCK, Block, N_("Block"), Q_NA,
	N_("The periodic table block to which the element belongs."),
	&IUPAC_REC);

Cat (GENERAL, N_("General"),
	&P_NAME,
	&P_OFFICIAL_NAME,
	&P_ALTERNATE_NAME,
	&P_SYMBOL,
	&P_NUMBER,
	&P_SERIES,
	&P_GROUP,
	&P_PERIOD,
	&P_BLOCK
);


//******************************************************************************
// Historical


Prop (DISCOVERY, Event, N_("Discovery"), Q_NA,
	N_("The year and place (country) of the element's discovery. Forward slashes indicate simultaneous, unrelated discoveries."),
	&UNSOURCED);

Prop (DISCOVERED_BY, Message, N_("Discovered by"), Q_NA,
	N_("The people and/or institutions which first discovered the element. Forward slashes indicate simultaneous, unrelated discoveries. Commas indicate multiple people and/or places in collaboration."),
	&UNSOURCED);

Prop (ETYMOLOGY, Message, N_("Etymology"), Q_NA,
	N_("The origin of the element name in the general format &quot;Language: word (meaning)&quot;. If an alternate name exists, its etymology may appear. Unrelated etymologies are separated by semicolons."),
	&UNSOURCED);

// Translators: i.e. the historical properties of an element
Cat (HISTORICAL, N_("Historical"),
	&P_DISCOVERY,
	&P_DISCOVERED_BY,
	&P_ETYMOLOGY
);


//******************************************************************************
// Physical


Prop (PHASE, Phase, N_("Phase"),
	// Translators: Replace " deg. " with the degree symbol here and below, if possible.
	N_("%1 at 0 deg. C"),
	N_("The phase of matter assumed by the element at standard pressure and a given temperature. By default, the IUPAC standard temperature is used."),
	&CALCULATED);

Prop (DENSITY_SOLID, Float, N_("Density, solid"),
	N_("%1 g/cm<sup>3</sup> at 20 deg. C"), N_("The density of the element as a solid at 20 degrees Celsius, if applicable, in grams per cubic centimeter."),
	&JAMES_1992, &LIDE_1996, &DEAN_1992, &KAYE_1993, &DEAN_1999, &LIDE_2003);

Prop (DENSITY_LIQUID, Float, N_("Density, liquid"),
	N_("%1 g/cm<sup>3</sup> at melting point"), N_("The density of the element as a liquid at its melting point, if applicable, in grams per cubic centimeter."),
	&LIDE_2003, &DEAN_1999);

Prop (DENSITY_GAS, Float, N_("Density, gas"), N_("%1 g/L at 0 deg. C"),
	N_("The density of the element as a gas at zero degrees Celsius, if applicable, in grams per liter."),
	&LIDE_2003, &DEAN_1999, &KUCHLING_1991);

Prop (APPEARANCE, Message, N_("Appearance"), Q_NA,
	N_("The general appearance of the most common form of the element."),
	&UNSOURCED);

// Translators: i.e. the physical properties of an element
Cat (PHYSICAL, N_("Physical"),
	&P_PHASE,
	&P_DENSITY_SOLID,
	&P_DENSITY_LIQUID,
	&P_DENSITY_GAS,
	&P_APPEARANCE
);


//******************************************************************************
// Thermal


Prop (MELTING_POINT, Float, N_("Melting point"), N_("%1 K"),
	N_("The melting point of the element in Kelvin."),
	&JAMES_1992, &KAYE_1993, &LIDE_2003, &DEAN_1999);

Prop (BOILING_POINT, Float, N_("Boiling point"), N_("%1 K"),
	N_("The boiling point of the element in Kelvin."),
	&JAMES_1992, &KAYE_1993, &LIDE_2003, &DEAN_1999);

Prop (FUSION_HEAT, Float, N_("Heat of fusion"), N_("%1 kJ/mol"),
	N_("The heat of fusion of the element in kilojoules per mole."),
	&LIDE_2003, &DEAN_1999, &KAYE_1993, &LIDE_1998, &JAMES_1992, &ELLIS_1972);

Prop (VAPORIZATION_HEAT, Float, N_("Heat of vaporization"), N_("%1 kJ/mol"),
	N_("The heat of vaporization of the element in kilojoules per mole."),
	&LIDE_2003, &DEAN_1999, &KAYE_1993, &LIDE_1998, &JAMES_1992, &ELLIS_1972);

Prop (SPECIFIC_HEAT, Float, N_("Specific heat capacity"),
	// Translators: Replace "*" with the midpoint dot symbol here and below, if possible.
	N_("%1 J/(g*K) at 25 deg. C"),
	N_("The specific heat of the element at 25 degrees Celsius, in joules per gram-Kelvin."),
	&LIDE_2003);

Prop (THERMAL_CONDUCTIVITY, Float, N_("Thermal conductivity"), N_("%1 W/(m*K) at 300 K"),
	N_("The thermal conductivity of the element at 300 Kelvin, in watts per meter-Kelvin."),
	&LIDE_2003, &HO_1974, &KAYE_1993, &LIDE_1998, &DEAN_1992, &JAMES_1992);

Prop (DEBYE_TEMPERATURE, Float, N_("Debye temperature"), N_("%1 K"),
	N_("The Debye temperature of the element in Kelvin."), &UNSOURCED);

// Translators: i.e. the thermal properties of an element
Cat (THERMAL, N_("Thermal"),
	&P_MELTING_POINT,
	&P_BOILING_POINT,
	&P_FUSION_HEAT,
	&P_VAPORIZATION_HEAT,
	&P_SPECIFIC_HEAT,
	&P_THERMAL_CONDUCTIVITY,
	&P_DEBYE_TEMPERATURE
);


//******************************************************************************
// Atomic


Prop (ATOMIC_MASS, Float, N_("Atomic mass"), N_("%1 g/mol"),
	N_("The atomic mass of the element in grams per mole."),
	&WIESER_2006);

Prop (ATOMIC_VOLUME, Float, N_("Atomic volume"), N_("%1 cm<sup>3</sup>/mol"),
	N_("The volume of the element in cubic centimeters per mole."),
	&UNSOURCED);

Prop (ATOMIC_RADIUS, Float, N_("Atomic radius"),
	// Translators: "pm" here is "picometre(s)"
	N_("%1 pm"),
	N_("The atomic radius of the element in picometers."),
	&SLATER_1964, &CLEMENTI_1963);

Prop (COVALENT_RADIUS, Float, N_("Covalent radius"), N_("%1 pm"),
	N_("The covalent radius of the element in picometers."),
	&SANDERSON_1962, &SUTTON_1965, &HUHEEY_1993, &PORTERFIELD_1984, &JAMES_1992);

Prop (VAN_DER_WAALS_RADIUS, Float, N_("Van der Waals radius"), N_("%1 pm"),
	N_("The van der Waals radius of the element in picometers."),
	&BONDI_1964, &BATSANOV_2001);

Prop (IONIC_RADII, String, N_("Ionic radii"), N_("%1 pm"),
	N_("The radii of the element's ions, if any, in picometers. The general format is &quot;radius (ion)&quot;."),
	&UNSOURCED);

// Translators: i.e. the atomic properties of an element
Cat (ATOMIC, N_("Atomic"),
	&P_ATOMIC_MASS,
	&P_ATOMIC_VOLUME,
	&P_ATOMIC_RADIUS,
	&P_COVALENT_RADIUS,
	&P_VAN_DER_WAALS_RADIUS,
	&P_IONIC_RADII
);


//******************************************************************************
// Crystallographic


Prop (LATTICE_TYPE, LatticeType, N_("Lattice type"), Q_NA,
	N_("The type of crystal lattice structure assumed by the element as a solid under normal conditions."),
	&WEBELEMENTS_XTAL);

Prop (SPACE_GROUP, Int, N_("Space group"), Q_NA,
	N_("The space group number of the element's crystal structure."),
	&WEBELEMENTS_XTAL);

Prop (LATTICE_EDGES, FloatList, N_("Lattice edge lengths"), N_("%1 pm"),
	N_("The lengths, in picometers, of the a, b, and c edges of a unit cell in the element's crystal structure."),
	&WEBELEMENTS_XTAL);

Prop (LATTICE_ANGLES, FloatList, N_("Lattice angles"), N_("%1 deg."),
	// Translators: Replace "alpha", "beta", and "gamma" with the Greek letters, if possible.
	N_("The alpha, beta, and gamma angles, in degrees, between the edges of a unit cell in the element's crystal structure."),
	&WEBELEMENTS_XTAL);

Prop (LATTICE_VOLUME, Float, N_("Lattice unit volume"), N_("%1 nm<sup>3</sup>"),
	N_("The volume of a unit cell in the element's crystal structure, in cubic nanometers."),
	&CALCULATED);

// Translators: i.e. the crystallographic properties of an element
Cat (CRYSTALLOGRAPHIC, N_("Crystallographic"),
	&P_LATTICE_TYPE,
	&P_SPACE_GROUP,
	&P_LATTICE_EDGES,
	&P_LATTICE_ANGLES,
	&P_LATTICE_VOLUME
);


//******************************************************************************
// Electronic


Prop (CONFIGURATION, String, N_("Electron configuration"), Q_NA,
	N_("The electron configuration of the element, in standard format."),
	&LIDE_2003);

Prop (OXIDATION_STATES, IntList, N_("Oxidation states"), Q_NA,
	N_("The oxidation states of the element, if any."),
	&UNSOURCED);

Prop (ELECTRONEGATIVITY, Float, N_("Electronegativity"), N_("%1 (Pauling scale)"),
	N_("The electronegativity of the element on the Pauling scale."),
	&PAULING_1960, &HUHEEY_1993, &ALLRED_1961);

Prop (ELECTRON_AFFINITY, Float, N_("Electron affinity"), N_("%1 kJ/mol"),
	N_("The electron affinity of the element in kilojoules per mole."),
	&WIKIPEDIA_AFFINITY);

Prop (FIRST_ENERGY, Float, N_("First ionization energy"), N_("%1 kJ/mol"),
	N_("The first ionization energy of the element in kilojoules per mole."),
	&HUHEEY_1993, &JAMES_1992, &LIDE_2003);

// Translators: i.e. the electronic properties of an element
Cat (ELECTRONIC, N_("Electronic"),
	&P_CONFIGURATION,
	&P_OXIDATION_STATES,
	&P_ELECTRONEGATIVITY,
	&P_ELECTRON_AFFINITY,
	&P_FIRST_ENERGY
);


//******************************************************************************
// Miscellaneous


Prop (COLOR, ColorValue, N_("Symbolic color"), Q_NA,
	N_("A color representative of the element."),
	&BODR_CONSENSUS);

Prop (NOTES, Message, N_("Notes"), Q_NA,
	N_("Notes, if any, clarifying other information."),
	&CALCULATED);

Cat (MISCELLANEOUS, N_("Miscellaneous"),
	&P_COLOR,
	&P_NOTES
);


#undef Prop
#undef Cat
#undef Src


//******************************************************************************
// Categories

#ifndef DOXYGEN_SHOULD_SKIP_THIS


static Category* cats[] =
{
	&C_GENERAL,
	&C_HISTORICAL,
	&C_PHYSICAL,
	&C_THERMAL,
	&C_ATOMIC,
	&C_CRYSTALLOGRAPHIC,
	&C_ELECTRONIC,
	&C_MISCELLANEOUS
};


std::list<Category*> CATEGORIES
	(cats, cats + sizeof (cats) / sizeof (Category*));
	

#endif // !DOXYGEN_SHOULD_SKIP_THIS


} // namespace Elemental
