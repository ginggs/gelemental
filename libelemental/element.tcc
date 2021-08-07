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

#ifndef LIBELEMENTAL__ELEMENT_TCC
#define LIBELEMENTAL__ELEMENT_TCC

#ifndef LIBELEMENTAL__ELEMENT_HH
#error "element.tcc must be included from element.hh."
#endif

namespace Elemental {


//******************************************************************************
// class Property<T>


template<class T>
Property<T>::Property (const Message& name_, const Message& format_,
	const Message& description_, const Message* sources_[]) throw ()
:	PropertyBase (name_, format_, description_, sources_)
{}


template<class T>
Property<T>::~Property ()
{}

	
//******************************************************************************
// class Element


template<class T>
inline const T&
Element::get_property (const Property<T>& property) const
{
	return *reinterpret_cast<const T*> (&get_property_base (property));
}


template<>
inline const std::string&
Element::get_property (const Property<std::string>& property) const
{
	if (&property == &P_SYMBOL) return symbol;
	throw std::invalid_argument ("not a value property");
}


template<>
inline const unsigned int&
Element::get_property (const Property<unsigned int>& property) const
{
	if (&property == &P_NUMBER) return number;
	throw std::invalid_argument ("not a value property");
}


} // namespace Elemental

#endif // LIBELEMENTAL__ELEMENT_TCC
