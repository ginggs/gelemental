//! \file table.hh
//! Functions for access to the entire table.
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

#ifndef LIBELEMENTAL__TABLE_HH
#define LIBELEMENTAL__TABLE_HH

#include <libelemental/value.hh>
#include <libelemental/value-types.hh>
#include <libelemental/element.hh>

#include <stdexcept>

//******************************************************************************

namespace Elemental {

//! The periodic table of elements.
typedef std::vector<const Element*> Table;

//! Initializes libelemental.
/*! This function may be called more than once. This function, or one of the
 * get_table() or get_element() functions, which call it, should be called
 * before any other features in the library are used. */
void initialize () throw ();

//! Returns the periodic table of elements.
//! The table is a singleton.
const Table& get_table () throw ();

//! Returns the element with a given atomic number.
/*! \param number The atomic number of an element.
 * \return A reference to the corresponding element.
 * \throw std::out_of_range if the atomic number is invalid. */
const Element& get_element (AtomicNumber number);

//! Returns the element with a given symbol or atomic number.
/*! \param which The standard symbol of an element, or the string representation
 * of the atomic number of an element.
 * \return A reference to the corresponding element.
 * \throw std::invalid_argument if the symbol or number is invalid. */
const Element& get_element (const std::string& which);

} // namespace Elemental

#endif // LIBELEMENTAL__TABLE_HH
