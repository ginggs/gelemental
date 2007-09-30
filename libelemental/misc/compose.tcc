/*
 * This file is part of libmisc, an assortment of code for reuse.
 * Modified from compose version 1.0.4.
 *
 * Copyright (c) 2006-2007 Kevin Daughtridge <kevin@kdau.com>
 * Copyright (c) 2002-2004 Ole Laursen <olau@hardworking.dk>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBMISC__COMPOSE_TCC
#define LIBMISC__COMPOSE_TCC

#ifndef LIBMISC__COMPOSE_HH
#error "compose.tcc must be included from compose.hh."
#endif

namespace compose {


//******************************************************************************
// class Composition


template<typename T>
inline Composition&
Composition::arg (const T &obj)
{
	os << obj;
	do_arg (os.str ());
	return *this;
}


//******************************************************************************
//! \name Composition Functions
//@{


template<typename  T1>
inline string
compose (const string &fmt,
         const  T1  &o1)
{
	Composition c (fmt);
	c.arg (o1);
	return c.str ();
}


template<typename  T1, typename  T2>
inline string
compose (const string &fmt,
         const  T1  &o1, const  T2  &o2)
{
	Composition c (fmt);
	c.arg (o1).arg (o2);
	return c.str ();
}


template<typename  T1, typename  T2, typename  T3>
inline string
compose (const string &fmt,
         const  T1  &o1, const  T2  &o2, const  T3  &o3)
{
	Composition c (fmt);
	c.arg (o1).arg (o2).arg (o3);
	return c.str ();
}


template<typename  T1, typename  T2, typename  T3, typename  T4>
inline string
compose (const string &fmt,
         const  T1  &o1, const  T2  &o2, const  T3  &o3, const  T4  &o4)
{
	Composition c (fmt);
	c.arg (o1).arg (o2).arg (o3).arg (o4);
	return c.str ();
}


template<typename  T1, typename  T2, typename  T3, typename  T4, typename  T5>
inline string
compose (const string &fmt,
         const  T1  &o1, const  T2  &o2, const  T3  &o3, const  T4  &o4,
         const  T5  &o5)
{
	Composition c (fmt);
	c.arg (o1).arg (o2).arg (o3).arg (o4).arg (o5);
	return c.str ();
}


template<typename  T1, typename  T2, typename  T3, typename  T4, typename  T5,
         typename  T6>
inline string
compose (const string &fmt,
         const  T1  &o1, const  T2  &o2, const  T3  &o3, const  T4  &o4,
         const  T5  &o5, const  T6  &o6)
{
	Composition c (fmt);
	c.arg (o1).arg (o2).arg (o3).arg (o4).arg (o5).arg (o6);
	return c.str ();
}


template<typename  T1, typename  T2, typename  T3, typename  T4, typename  T5,
         typename  T6, typename  T7>
inline string
compose (const string &fmt,
         const  T1  &o1, const  T2  &o2, const  T3  &o3, const  T4  &o4,
         const  T5  &o5, const  T6  &o6, const  T7  &o7)
{
	Composition c (fmt);
	c.arg (o1).arg (o2).arg (o3).arg (o4).arg (o5).arg (o6).arg (o7);
	return c.str ();
}


template<typename  T1, typename  T2, typename  T3, typename  T4, typename  T5,
         typename  T6, typename  T7, typename  T8>
inline string
compose (const string &fmt,
         const  T1  &o1, const  T2  &o2, const  T3  &o3, const  T4  &o4,
         const  T5  &o5, const  T6  &o6, const  T7  &o7, const  T8  &o8)
{
	Composition c (fmt);
	c.arg (o1).arg (o2).arg (o3).arg (o4).arg (o5).arg (o6).arg (o7).arg (o8);
	return c.str ();
}


template<typename  T1, typename  T2, typename  T3, typename  T4, typename  T5,
         typename  T6, typename  T7, typename  T8, typename  T9>
inline string
compose (const string &fmt,
         const  T1  &o1, const  T2  &o2, const  T3  &o3, const  T4  &o4,
         const  T5  &o5, const  T6  &o6, const  T7  &o7, const  T8  &o8,
         const  T9  &o9)
{
	Composition c (fmt);
	c.arg (o1).arg (o2).arg (o3).arg (o4).arg (o5).arg (o6).arg (o7).arg (o8)
		.arg (o9);
	return c.str ();
}


template<typename  T1, typename  T2, typename  T3, typename  T4, typename  T5,
         typename  T6, typename  T7, typename  T8, typename  T9, typename T10>
inline string
compose (const string &fmt,
         const  T1  &o1, const  T2  &o2, const  T3  &o3, const  T4  &o4,
         const  T5  &o5, const  T6  &o6, const  T7  &o7, const  T8  &o8,
         const  T9  &o9, const T10 &o10)
{
	Composition c (fmt);
	c.arg (o1).arg (o2).arg (o3).arg (o4).arg (o5).arg (o6).arg (o7).arg (o8)
		.arg (o9).arg (o10);
	return c.str ();
}


template<typename  T1, typename  T2, typename  T3, typename  T4, typename  T5,
         typename  T6, typename  T7, typename  T8, typename  T9, typename T10,
         typename T11>
inline string
compose (const string &fmt,
         const  T1  &o1, const  T2  &o2, const  T3  &o3, const  T4  &o4,
         const  T5  &o5, const  T6  &o6, const  T7  &o7, const  T8  &o8,
         const  T9  &o9, const T10 &o10, const T11 &o11)
{
	Composition c (fmt);
	c.arg (o1).arg (o2).arg (o3).arg (o4).arg (o5).arg (o6).arg (o7).arg (o8)
		.arg (o9).arg (o10).arg (o11);
	return c.str ();
}


template<typename  T1, typename  T2, typename  T3, typename  T4, typename  T5,
         typename  T6, typename  T7, typename  T8, typename  T9, typename T10,
         typename T11, typename T12>
inline string
compose (const string &fmt,
         const  T1  &o1, const  T2  &o2, const  T3  &o3, const  T4  &o4,
         const  T5  &o5, const  T6  &o6, const  T7  &o7, const  T8  &o8,
         const  T9  &o9, const T10 &o10, const T11 &o11, const T12 &o12)
{
	Composition c (fmt);
	c.arg (o1).arg (o2).arg (o3).arg (o4).arg (o5).arg (o6).arg (o7).arg (o8)
		.arg (o9).arg (o10).arg (o11).arg (o12);
	return c.str ();
}


template<typename  T1, typename  T2, typename  T3, typename  T4, typename  T5,
         typename  T6, typename  T7, typename  T8, typename  T9, typename T10,
         typename T11, typename T12, typename T13>
inline string
compose (const string &fmt,
         const  T1  &o1, const  T2  &o2, const  T3  &o3, const  T4  &o4,
         const  T5  &o5, const  T6  &o6, const  T7  &o7, const  T8  &o8,
         const  T9  &o9, const T10 &o10, const T11 &o11, const T12 &o12,
         const T13 &o13)
{
	Composition c (fmt);
	c.arg (o1).arg (o2).arg (o3).arg (o4).arg (o5).arg (o6).arg (o7).arg (o8)
		.arg (o9).arg (o10).arg (o11).arg (o12).arg (o13);
	return c.str ();
}


template<typename  T1, typename  T2, typename  T3, typename  T4, typename  T5,
         typename  T6, typename  T7, typename  T8, typename  T9, typename T10,
         typename T11, typename T12, typename T13, typename T14>
inline string
compose (const string &fmt,
         const  T1  &o1, const  T2  &o2, const  T3  &o3, const  T4  &o4,
         const  T5  &o5, const  T6  &o6, const  T7  &o7, const  T8  &o8,
         const  T9  &o9, const T10 &o10, const T11 &o11, const T12 &o12,
         const T13 &o13, const T14 &o14)
{
	Composition c (fmt);
	c.arg (o1).arg (o2).arg (o3).arg (o4).arg (o5).arg (o6).arg (o7).arg (o8)
		.arg (o9).arg (o10).arg (o11).arg (o12).arg (o13).arg (o14);
	return c.str ();
}


template<typename  T1, typename  T2, typename  T3, typename  T4, typename  T5,
         typename  T6, typename  T7, typename  T8, typename  T9, typename T10,
         typename T11, typename T12, typename T13, typename T14, typename T15>
inline string
compose (const string &fmt,
         const  T1  &o1, const  T2  &o2, const  T3  &o3, const  T4  &o4,
         const  T5  &o5, const  T6  &o6, const  T7  &o7, const  T8  &o8,
         const  T9  &o9, const T10 &o10, const T11 &o11, const T12 &o12,
         const T13 &o13, const T14 &o14, const T15 &o15)
{
	Composition c (fmt);
	c.arg (o1).arg (o2).arg (o3).arg (o4).arg (o5).arg (o6).arg (o7).arg (o8)
		.arg (o9).arg (o10).arg (o11).arg (o12).arg (o13).arg (o14).arg (o15);
	return c.str ();
}


//@}


} // namespace compose

#endif // LIBMISC__COMPOSE_TCC
