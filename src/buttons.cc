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

#include "private.hh"
#include <libelemental/table.hh>
#include "misc.hh"
#include "dialogs.hh"
#include "buttons.hh"

#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/settings.h>

namespace gElemental {


//******************************************************************************
// class ColorButton


void
ColorButton::set_color (const color_value_base& value)
{
	Gdk::Color color = allocate (value.get_color ());

	if (is_force_needed ())
	{
		RefPtr<Gtk::Style> style = Gtk::Style::create ();
		style->set_bg (Gtk::STATE_NORMAL, color);
		style->set_bg (Gtk::STATE_ACTIVE, color);
		style->set_bg (Gtk::STATE_PRELIGHT, color);
		set_style (style);
	}
	else
	{
		modify_bg (Gtk::STATE_NORMAL, color);
		modify_bg (Gtk::STATE_ACTIVE, color);
		modify_bg (Gtk::STATE_PRELIGHT, color);
	}

	if (Gtk::Widget *child = get_child ())
		set_fgcolor (*child, value);
}


void
ColorButton::unset_color ()
{
	if (is_force_needed ())
		unset_style ();
	else
	{
		unset_bg (Gtk::STATE_NORMAL);
		unset_bg (Gtk::STATE_ACTIVE);
		unset_bg (Gtk::STATE_PRELIGHT);
	}

	if (Gtk::Widget *child = get_child ())
		unset_fgcolor (*child);
}


void
ColorButton::set_fgcolor (Gtk::Widget& child, const color_value_base& value)
{
	Gdk::Color compliment = allocate (value.get_color ().get_compliment ());

	child.modify_fg (Gtk::STATE_NORMAL, compliment);
	child.modify_fg (Gtk::STATE_ACTIVE, compliment);
	child.modify_fg (Gtk::STATE_PRELIGHT, compliment);
}


void
ColorButton::unset_fgcolor (Gtk::Widget& child)
{
	child.unset_fg (Gtk::STATE_NORMAL);
	child.unset_fg (Gtk::STATE_ACTIVE);
	child.unset_fg (Gtk::STATE_PRELIGHT);
}


bool
ColorButton::is_force_needed ()
{
	static const char *OFFENDING_THEMES[] = 
	{
		"Amaranth",
		"Lush",
		"Nuvola",
		"New Wave",
		"Mint-X",
		"Mint-X-Aqua",
		"Mint-X-Blue",
		"Mint-X-Brown",
		"Mint-X-Grey",
		"Mint-X-Orange",
		"Mint-X-Pink",
		"Mint-X-Purple",
		"Mint-X-Red",
		"Mint-X-Sand",
		"Mint-X-Teal",
		"Mint-Y",
		"Mint-Y-Aqua",
		"Mint-Y-Blue",
		"Mint-Y-Brown",
		"Mint-Y-Dark",
		"Mint-Y-Red",
		"Mint-Y-Dark-Red",
		"Mint-Y-Dark-Aqua",
		"Mint-Y-Dark-Blue",
		"Mint-Y-Dark-Brown",
		"Mint-Y-Darker-Aqua",
		"Mint-Y-Darker-Blue",
		"Mint-Y-Darker-Brown",
		"Mint-Y-Darker-Grey",
		"Mint-Y-Darker-Orange",
		"Mint-Y-Darker-Pink",
		"Mint-Y-Darker-Purple",
		"Mint-Y-Darker-Red",
		"Mint-Y-Darker-Sand",
		"Mint-Y-Darker-Teal",
		"Mint-Y-Dark-Grey",
		"Mint-Y-Dark-Orange",
		"Mint-Y-Dark-Pink",
		"Mint-Y-Dark-Purple",
		"Mint-Y-Dark-Sand",
		"Mint-Y-Dark-Teal",
		"Mint-Y-Grey",
		"Mint-Y-Orange",
		"Mint-Y-Pink",
		"Mint-Y-Purple",
		"Mint-Y-Sand",
		"Mint-Y-Teal",
		"Breeze",
		"Breeze Dark",
		NULL
	};

	static bool tested = false;
	static bool needed = false;
	
	if (!tested)
	{
		ustring theme = Gtk::Settings::get_default ()->
			property_gtk_theme_name ().get_value ();
		
		for (const char **test = OFFENDING_THEMES; *test; ++test)
			if (theme == *test)
			{
				needed = true;
				break;
			}
		
		tested = true;
	}
	
	return needed;
}


//******************************************************************************
// class ElementButton


ElementButton::ElementButton (const Element& el_, Gtk::Tooltips& tips)
:	el (el_), group (el.get_property (P_GROUP)),
	period (el.get_property (P_PERIOD))
{
	set_label (el.symbol);

	tips.set_tip (*this, compose::ucompose (_("%1 (%2)"),
		el.get_property (P_NAME).get_string (), el.number));
}


int
ElementButton::get_x_pos () const
{
	switch (period.value)
	{
	case 6:
		if (!group.has_value () || group.value == 3)
			return 3 + (el.number - 57) - 1;
	case 7:
		if (!group.has_value () || group.value == 3)
			return 3 + (el.number - 89) - 1;
	default:
		return group.value - 1;
	}
}


int
ElementButton::get_y_pos () const
{
	switch (period.value)
	{
	case 6:
		if (!group.has_value () || group.value == 3)
			return 9 - 1;
	case 7:
		if (!group.has_value () || group.value == 3)
			return 10 - 1;
	default:
		return period.value - 1;
	}
}


void
ElementButton::show_properties ()
{
	ElementDialog *dialog = new ElementDialog (el);
	if cast (get_toplevel (), Gtk::Window, top)
		dialog->set_transient_for (*top);
	dialog->present ();
}


void
ElementButton::set_color_by_property (const PropertyBase* property,
	double temperature, bool logarithmic)
{
	if (property == NULL)
		unset_color ();

	else if (property == &P_PHASE)
		set_color (el.get_phase (temperature));
	
	else if cast (property, const FloatProperty, float_prop)
	{
		const Float &float_value = el.get_property (*float_prop);
	
		if (float_value.has_value () && float_prop->is_scale_valid ())
			set_color (ColorValue
				(float_prop->get_scale_position (float_value, logarithmic)));

		else
			set_color (ColorValue ());
	}
	
	else if cast (&el.get_property_base (*property),
		const color_value_base, color_value)
		set_color (*color_value);
	
	else
		unset_color ();
}


void
ElementButton::on_clicked ()
{
	ColorButton::on_clicked ();
	show_properties ();
}


//******************************************************************************
// class ElementIdentity


ElementIdentity::ElementIdentity (const Element& el)
{
	property_can_focus ().set_value (false);
	set_focus_on_click (false);
	set_size_request (-1, 50);

	const ColorValue& color = el.get_property (P_COLOR);
	set_color (color);

	push_composite_child ();

	Gtk::VBox *square = new Gtk::VBox (false, 6);
	square->set_border_width (6);
	add (*Gtk::manage (square));

	Gtk::Label *number = new Gtk::Label ();
	set_fgcolor (*number, color);
	number->set_markup ("<b>" + compose::ucompose1 (el.number) + "</b>");
	square->pack_start (*Gtk::manage (number), Gtk::PACK_EXPAND_PADDING);

	Gtk::Label *symbol = new Gtk::Label ();
	set_fgcolor (*symbol, color);
	symbol->set_markup ("<big><big><b>" + el.symbol + "</b></big></big>");
	square->pack_start (*Gtk::manage (symbol), Gtk::PACK_EXPAND_PADDING);
	
	pop_composite_child ();
	show_all_children ();
}


//******************************************************************************
// class LegendButton


LegendButton::LegendButton (const color_value_base& value)
{
	if (!CAST (&value, const ColorValue))
		set_label (value.get_string ());
	set_color (value);
	property_can_focus ().set_value (false);
	set_focus_on_click (false);
}


} // namespace gElemental
