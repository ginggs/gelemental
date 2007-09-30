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

#ifndef LIBELEMENTAL__VALUE_TCC
#define LIBELEMENTAL__VALUE_TCC

#ifndef LIBELEMENTAL__VALUE_HH
#error "value.tcc must be included from value.hh."
#endif

namespace Elemental {


//******************************************************************************
// class Value<T>


template<class T>
template<class S>
Value<T>::Value (const S& source, Qualifier qualifier_)
:	value_base (qualifier_), value (source)
{}


template<class T>
template<class S>
Value<T>::Value (const Value<S>& source)
:	value_base (source.qualifier), value (source.value)
{}


template<class T>
Value<T>::Value (Qualifier qualifier_) throw ()
:	value_base (qualifier_)
{}
	
	
template<class T>
Value<T>::~Value ()
{}

	
template<class T>
int
Value<T>::compare (const value_base& other) const throw ()
{
	int base = compare_base (other);
	if (base != YIELD_COMPARE) return base;
		
	const Value<T> *like_other = dynamic_cast<const Value<T>*> (&other);
	if (like_other != NULL)
	{
		if (value < like_other->value)
			return -1;
		else if (value > like_other->value)
			return 1;
		else
			return 0;
	}

	return 0;
}


template<class T>
ustring
Value<T>::do_get_string (const ustring& format) const throw ()
{
	if (format.empty ())
		return compose::ucompose1 (value, std::numeric_limits<T>::digits10);
	else
		return compose::UComposition (format)
			.precision (std::numeric_limits<T>::digits10)
			.arg (value).str ();
}

	
//******************************************************************************
// class ValueList<T>


template<class T>
template<class S>
ValueList<T>::ValueList (const S source[], int count, Qualifier qualifier_)
:	value_base (qualifier_)
{
	for (int i = 0; i < count; ++i)
		values.push_back (T (source[i]));
}


template<class T>
template<class S>
ValueList<T>::ValueList (const ValueList<S>& source)
:	value_base (source.qualifier)
{
	for (typename std::vector<S>::const_iterator i = source.values.begin ();
		i != source.values.end (); ++i)
		values.push_back (T (*i));
}


template<class T>
ValueList<T>::ValueList (Qualifier qualifier_) throw ()
:	value_base (qualifier_)
{}


template<class T>
ValueList<T>::~ValueList ()
{}


template<class T>
int
ValueList<T>::compare (const value_base& other) const throw ()
{
	int base = compare_base (other);
	if (base != YIELD_COMPARE) return base;
		
	const ValueList<T> *like_other = dynamic_cast<const ValueList<T>*> (&other);
	if (like_other != NULL)
	{
		if (values < like_other->values)
			return -1;
		else if (values > like_other->values)
			return 1;
		else
			return 0;
	}

	return 0;
}


template<class T>
ustring
ValueList<T>::do_get_string (const ustring& format) const throw ()
{
	ustring result;
	for (typename std::vector<T>::const_iterator i = values.begin ();
		i != values.end (); ++i)
	{
		if (i != values.begin ())
			result += get_list_separator ();
		if (format.empty ())
			result += compose::ucompose1
				(*i, std::numeric_limits<T>::digits10);
		else
			result += compose::UComposition (format)
				.precision (std::numeric_limits<T>::digits10)
				.arg (*i).str ();
	}
	return result;
}


} // namespace Elemental

#endif // LIBELEMENTAL__VALUE_TCC
