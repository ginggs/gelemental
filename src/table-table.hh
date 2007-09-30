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

#ifndef GELEMENTAL__TABLE_TABLE_HH
#define GELEMENTAL__TABLE_TABLE_HH

#include <libelemental/misc/throttle.hh>
#include "misc.hh"
#include "buttons.hh"

#include <vector>
#include <gtkmm/actiongroup.h>
#include <gtkmm/adjustment.h>
#include <gtkmm/box.h>
#include <gtkmm/eventbox.h>
#include <gtkmm/expander.h>
#include <gtkmm/label.h>
#include <gtkmm/radiobutton.h>
#include <gtkmm/table.h>
#include <gtkmm/toggleaction.h>
#include <gtkmm/uimanager.h>

//******************************************************************************

namespace gElemental {

//******************************************************************************

class TableTable
:	public Gtk::VBox
{
public:

	TableTable (const RefPtr<Gtk::UIManager>& ui);
	virtual ~TableTable ();

	struct State
	{
		PropertyBase *color_by;
		double temperature;
		bool logarithmic;
		bool legend_expanded;
	};
	
	State get_state () const;
	void set_state (State& state);

private:
	
	RefPtr<Gtk::UIManager> ui;
	Gtk::UIManager::ui_merge_id ui_id;
	RefPtr<Gtk::ActionGroup> actions;
	Gtk::Tooltips tips;

	void populate_button_table ();
	void update_colors ();
	Gtk::Table button_table;

	void create_reference (const ustring& title, guint x1, guint y1,
		guint x2, guint y2);
	Glib::ustring::size_type next_reference;

	void on_button_focusing (const Element* el);
	void on_button_crossing (const Element* el);
	void clear_hover ();
	const Element *focus_el, *hover_el;
	misc::Throttle throttle_clear_hover;

	void update_display ();
	Gtk::HBox display;

	void on_color_by_changed (PropertyBase* property);
	PropertyBase *color_by;

	void on_temperature_changed ();
	Gtk::Adjustment temperature;
	misc::Throttle throttle_update_temperature;

	void on_logarithmic_toggled ();
	Gtk::RadioButton logarithmic;

	void on_legend_toggled (bool from_legend);
	Gtk::Expander legend;
	RefPtr<Gtk::ToggleAction> show_legend;
	misc::ComposeBox conditions;
	Gtk::HBox scales;

	void update_colorbar ();
	Gtk::EventBox colorbar;
};

} // namespace gElemental

#endif // GELEMENTAL__TABLE_TABLE_HH
