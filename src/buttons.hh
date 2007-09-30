/*
 * This file is part of gElemental, a periodic table viewer with detailed
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

#ifndef GELEMENTAL__BUTTONS_HH
#define GELEMENTAL__BUTTONS_HH

#include <libelemental/element.hh>

#include <gtkmm/button.h>
#include <gtkmm/tooltips.h>

//******************************************************************************

namespace gElemental {

//******************************************************************************

class ColorButton
:	public Gtk::Button
{
public:

	void set_color (const color_value_base& value);
	void unset_color ();

protected:

	void set_fgcolor (Gtk::Widget& child, const color_value_base& value);
	void unset_fgcolor (Gtk::Widget& child);

private:

	static bool is_force_needed ();
};

//******************************************************************************

class ElementButton
:	public ColorButton
{
public:

	ElementButton (const Element& el, Gtk::Tooltips& tips);

	int get_x_pos () const;
	int get_y_pos () const;

	void show_properties ();

	void set_color_by_property (const PropertyBase* property,
		double temperature, bool logarithmic);

	const Element &el;

private:

	virtual void on_clicked ();
	
	Int group, period;
};

//******************************************************************************

class ElementIdentity
:	public ColorButton
{
public:

	ElementIdentity (const Element& el);
};

//******************************************************************************

class LegendButton
:	public ColorButton
{
public:

	LegendButton (const color_value_base& value);
};

} // namespace gElemental

#endif // GELEMENTAL__BUTTONS_HH
