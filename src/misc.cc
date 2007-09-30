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
#include "misc.hh"

#include <glibmm/main.h>
#include <gtkmm/eventbox.h>
#include <gtkmm/label.h>

namespace gElemental {


Gdk::Color
allocate (const color& color)
{
	Gdk::Color result;
	result.set_rgb_p (color.red, color.green, color.blue);
	return result;
}


//******************************************************************************
// class EntriesTable


EntriesTable::EntriesTable (Gtk::Tooltips& tips_, bool primary_, bool squeeze_)
:	misc::InfoTable (Gtk::ALIGN_TOP, 12), tips (tips_), primary (primary_),
	squeeze (squeeze_), last_header (NULL)
{
	set_border_width (12);
	set_col_spacings (12);
	set_row_spacings (2);
	indent ();
}


EntriesTable::~EntriesTable ()
{}


void
EntriesTable::header (const ustring& category) throw ()
{
	if (primary)
	{
		primary_header = category;
		primary = false;
	}
	else
	{
		unindent ();
		trim ();
		last_header = add_heading (category);
	}
}


void
EntriesTable::entry (const ustring& name, const ustring& value,
	const ustring& tip) throw ()
{
	last_header = NULL;
	push_composite_child ();

	Gtk::Label *value_label = new Gtk::Label ();
	value_label->set_alignment (Gtk::ALIGN_LEFT, Gtk::ALIGN_TOP);
	value_label->set_line_wrap ();
	value_label->set_selectable ();

	ustring final_value = value;
	Gtk::Widget *item = value_label;
	if (!tip.empty ())
	{
		final_value += " <span color=\"#666\">*</span>";
		Gtk::EventBox *ebox = new Gtk::EventBox ();
		ebox->add (*Gtk::manage (value_label));
		tips.set_tip (*ebox, tip);
		ebox->show ();
		item = ebox;	
	}

	value_label->set_markup (final_value);	
	// I18N: This is a bad hack, but the default wrapping is uglier.
	if (squeeze && !name.empty () && value_label->get_text ().size () > 33)
		value_label->set_size_request (200);
	value_label->show ();

	if (!name.empty ())
		add_item (name, *Gtk::manage (item));
	else
		add_item (*Gtk::manage (item));
		
	pop_composite_child ();
}


ustring
EntriesTable::get_primary_header () const
{
	return primary_header;
}


void
EntriesTable::trim ()
{
	if (last_header != NULL)
	{
		remove (*last_header);
		last_header = NULL;
	}
}


//******************************************************************************
// class SortColumn


SortColumn::SortColumn (const ustring& title, bool primary,
	Gtk::RadioButtonGroup& group, const ustring& action_name,
	const ustring& action_title)
:	Gtk::TreeViewColumn (title),
	sort_action (Gtk::RadioAction::create (group, action_name, action_title))
{
	if (primary)
		cell_renderer.property_weight ().set_value (Pango::WEIGHT_BOLD);
	pack_start (cell_renderer);
	set_cell_data_func (cell_renderer,
		sigc::mem_fun (*this, &SortColumn::on_cell_data));
	sort_action->signal_activate ().connect
		(sigc::mem_fun (*this, &SortColumn::on_sort_menu));
}


RefPtr<Gtk::RadioAction>
SortColumn::get_sort_action () const
{
	return sort_action;
}


void
SortColumn::activate ()
{
	sort_action->activate ();
}


void
SortColumn::on_clicked ()
{
	static bool recursing = false;
	if (!recursing)
	{
		recursing = true;
		Gtk::TreeViewColumn::on_clicked ();
		sort_action->set_active (true);
		recursing = false;
	}
}


void
SortColumn::on_sort_menu ()
{
	if (!get_sort_indicator ())
		clicked ();
}


} // namespace gElemental
