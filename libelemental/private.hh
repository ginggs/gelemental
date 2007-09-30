//! \file private.hh
//! Definitions private to libelemental.
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

#ifndef LIBELEMENTAL__PRIVATE_HH
#define LIBELEMENTAL__PRIVATE_HH

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <libelemental/value.hh>
#include <libelemental/misc/macros.hh>

#include <glib/gi18n-lib.h>

namespace Elemental {

class Tango
{
public:

	static const color BUTTER;
	static const color ORANGE;
	static const color LIGHT_CHOCOLATE;
	static const color DARK_CHOCOLATE;
	static const color CHAMELEON;
	static const color SKY_BLUE;
	static const color LIGHT_PLUM;
	static const color DARK_PLUM;
	static const color LIGHT_SCARLET_RED;
	static const color DARK_SCARLET_RED;
	static const color LIGHT_ALUMINIUM;
	static const color MEDIUM_ALUMINIUM;
};

} // namespace Elemental

#endif // LIBELEMENTAL__PRIVATE_HH
