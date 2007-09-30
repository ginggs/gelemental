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
#include <libelemental/misc/widgets.hh>
#include <libelemental/table.hh>
#include "table-table.hh"

#include <climits>
#include <gtkmm/alignment.h>
#include <gtkmm/box.h>
#include <gtkmm/radioaction.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/stock.h>

namespace gElemental {


//******************************************************************************
// class TableTable


TableTable::TableTable (const RefPtr<Gtk::UIManager>& ui_)
:	Gtk::VBox (false, 12), ui (ui_), actions (Gtk::ActionGroup::create ()),
	button_table (10, 18, true), next_reference (1),
	focus_el (NULL), hover_el (NULL),
	throttle_clear_hover (misc::Throttle::DELAY, 1.0), display (false, 6),
	color_by (NULL), temperature (STANDARD_TEMPERATURE, 0, DBL_MAX, 10, 100),
	throttle_update_temperature (misc::Throttle::LIMIT, 0.5),
	logarithmic (_("Lo_garithmic"), true),
	legend (_("_Legend"), true),
	conditions (_("_At %1 K and standard pressure"), true), scales (false, 6)
{
	set_border_width (12);
	
	populate_button_table ();
	update_display ();
	pack_start (button_table, Gtk::PACK_EXPAND_WIDGET);

	legend.set_use_markup (true);
	legend.set_use_underline (true);
	legend.property_expanded ().signal_changed ().connect (sigc::bind
		(sigc::mem_fun (*this, &TableTable::on_legend_toggled), true));
	pack_start (legend, Gtk::PACK_SHRINK);
	
	Gtk::VBox *legend_vbox = new Gtk::VBox (false, 2);
	legend_vbox->set_border_width (6);
	legend.add (*Gtk::manage (legend_vbox));
	
	temperature.signal_value_changed ().connect
		(sigc::mem_fun (*this, &TableTable::on_temperature_changed));
	Gtk::SpinButton *temperature_entry = new Gtk::SpinButton (temperature);
	temperature_entry->set_digits (1);
	temperature_entry->set_width_chars (6);
	conditions.set_param (1, *Gtk::manage (temperature_entry));
	legend_vbox->pack_start (conditions, Gtk::PACK_SHRINK);

	Gtk::RadioButtonGroup scale_group = logarithmic.get_group ();
	scales.pack_start
		(*Gtk::manage (new Gtk::Label (_("Scale:"))), Gtk::PACK_SHRINK);
	legend_vbox->pack_start (scales, Gtk::PACK_SHRINK);

	Gtk::RadioButton *linear =
		new Gtk::RadioButton (scale_group, _("Li_near"), true);
	linear->set_active (true);
	scales.pack_start (*Gtk::manage (linear), Gtk::PACK_SHRINK);

	logarithmic.signal_toggled ().connect
		(sigc::mem_fun (*this, &TableTable::on_logarithmic_toggled));
	scales.pack_start (logarithmic, Gtk::PACK_SHRINK);

	legend_vbox->pack_start (colorbar, Gtk::PACK_EXPAND_WIDGET);

	Gtk::RadioButtonGroup color_group;
	// Translators: i.e. the verb "to color".
	actions->add (Gtk::Action::create ("ViewColorMenu", _("_Color by")));
	actions->add (Gtk::RadioAction::create (color_group, "ViewColorByNone",
		_("_None")), sigc::bind (sigc::mem_fun
			(*this, &TableTable::on_color_by_changed), (PropertyBase*) NULL));
	actions->add (Gtk::RadioAction::create (color_group, "ViewColorByElement",
		_("_Element")), sigc::bind (sigc::mem_fun
			(*this, &TableTable::on_color_by_changed), &P_COLOR));

	ustring extra_ui;
	int n = 1;
	RefPtr<Gtk::Action> initial_action;
	FOREACH (std::list<Category*>, CATEGORIES, category)
	{
		if (*category == &C_MISCELLANEOUS) continue; // already handled

		ustring cname = compose::ucompose ("ViewColorCategory%1", n);
		actions->add (Gtk::Action::create (cname, (*category)->get_name ()));
		extra_ui += "<menu action='" + cname + "'>";

		int o = 1;
		FOREACH (std::list<PropertyBase*>, (*category)->properties, prop)
		{
			if (!(*prop)->is_colorable ()) continue;
			ustring pname = compose::ucompose ("ViewColorBy%1_%2", n, o++);
			RefPtr<Gtk::Action> paction = Gtk::RadioAction::create
				(color_group, pname, (*prop)->get_name ());
			if (*prop == &P_SERIES)
				initial_action = paction;
			actions->add (paction, sigc::bind (sigc::mem_fun
				(*this, &TableTable::on_color_by_changed), *prop));
			extra_ui += "<menuitem action='" + pname + "' />";
		}
		
		extra_ui += "</menu>";
		++n;
	}

	actions->add (show_legend = Gtk::ToggleAction::create
		("ViewShowLegend", _("Show _Legend")), sigc::bind (sigc::mem_fun
			(*this, &TableTable::on_legend_toggled), false));

	ui->insert_action_group (actions);
	ui_id = ui->add_ui_from_string
	(
		"<ui><menubar name='MenuBar'><menu action='ViewMenu'>"
			"<separator />"
			"<menu action='ViewColorMenu'>"
				"<menuitem action='ViewColorByNone' />"
				"<menuitem action='ViewColorByElement' />"
				+ extra_ui +
			"</menu>"
			"<menuitem action='ViewShowLegend' />"
		"</menu></menubar></ui>"
	);

	show_all_children ();
	if (initial_action)
		initial_action->activate ();
	show ();
}


TableTable::~TableTable ()
{
	ui->remove_ui (ui_id);
	ui->remove_action_group (actions);
}


TableTable::State
TableTable::get_state () const
{
	State result =
	{
		color_by,
		temperature.get_value (),
		logarithmic.get_active (),
		legend.get_expanded ()
	};
	return result;
}


void
TableTable::set_state (State& state)
{
	on_color_by_changed (state.color_by);
	throttle_update_temperature.reset ();
	temperature.set_value (state.temperature);
	logarithmic.set_active (state.logarithmic);
	legend.set_expanded (state.legend_expanded);
}


void
TableTable::populate_button_table ()
{
	CONST_FOREACH (Table, get_table (), el)
	{
		ElementButton *button = new ElementButton (**el, tips);
		int x = button->get_x_pos (), y = button->get_y_pos ();
		button_table.attach (*Gtk::manage (button), x, x + 1, y, y + 1,
			Gtk::FILL, Gtk::FILL, 2, 2);

		button->signal_focus_in_event ().connect (sigc::bind_return
			(sigc::hide (sigc::bind (sigc::mem_fun
				(*this, &TableTable::on_button_focusing),
					*el)), false));
		button->signal_focus_out_event ().connect (sigc::bind_return
			(sigc::hide (sigc::bind (sigc::mem_fun
				(*this, &TableTable::on_button_focusing),
					(const Element*) NULL)), false));

		button->signal_enter_notify_event ().connect (sigc::bind_return
			(sigc::hide (sigc::bind (sigc::mem_fun
				(*this, &TableTable::on_button_crossing),
					*el)), false));
		button->signal_leave_notify_event ().connect (sigc::bind_return
			(sigc::hide (sigc::bind (sigc::mem_fun
				(*this, &TableTable::on_button_crossing),
					(const Element*) NULL)), false));
	}

	create_reference (Series (Series::LANTHANIDE).get_string (), 3, 6, 2, 9);
	create_reference (Series (Series::ACTINIDE).get_string (), 3, 7, 2, 10);

	button_table.attach (display, 2, 12, 0, 3, Gtk::EXPAND, Gtk::EXPAND);
}


void
TableTable::update_colors ()
{
	update_display ();

	Glib::ListHandle<Gtk::Widget*> list = button_table.get_children ();
	CONST_FOREACH (Glib::ListHandle<Gtk::Widget*>, list, child)
		if cast (*child, ElementButton, button)
			button->set_color_by_property (color_by,
				temperature.get_value (), logarithmic.get_active ());
}


void
TableTable::create_reference (const ustring& title, guint x1, guint y1,
	guint x2, guint y2)
{
	ustring reference (next_reference++, '*');

	Gtk::EventBox *ebox = new Gtk::EventBox ();
	ebox->set_visible_window (false);
	if (!title.empty ())
		tips.set_tip (*ebox, title);
	button_table.attach (*Gtk::manage (ebox), x1 - 1, x1, y1 - 1, y1);

	Gtk::Label *label = new Gtk::Label (reference);
	ebox->add (*Gtk::manage (label));

	label = new Gtk::Label (reference, Gtk::ALIGN_RIGHT);
	button_table.attach (*Gtk::manage (label), x2 - 1, x2, y2 - 1, y2);
}


void
TableTable::on_button_focusing (const Element* el)
{
	focus_el = el;
	update_display ();
}


void
TableTable::on_button_crossing (const Element* el)
{
	if (el != NULL)
	{
		throttle_clear_hover.clear ();
		hover_el = el;
		update_display ();
	}
	else
	{
		throttle_clear_hover.reset ();
		throttle_clear_hover.queue
			(sigc::mem_fun (*this, &TableTable::clear_hover), true);
	}
}


void
TableTable::clear_hover ()
{
	hover_el = NULL;
	update_display ();
}


void
TableTable::update_display ()
{
	if (display.gobj () == NULL) return;

	display.children ().erase
		(display.children ().begin (), display.children ().end ());

	const Element *el = (hover_el != NULL) ? hover_el : focus_el;
	if (el == NULL) return;

	ElementIdentity *id = new ElementIdentity (*el);
	id->show ();
	display.pack_start (*Gtk::manage (id), Gtk::PACK_SHRINK);

	ustring markup =
		el->get_property (P_NAME).get_string ("<big><b>%1</b></big>");

	if (color_by == &P_PHASE)
		markup += "\n" + el->get_phase (temperature.get_value ()).get_string ();

	else if (color_by != NULL)
		markup += "\n" + el->get_property_base (*color_by)
			.get_string (color_by->get_format ());

	Gtk::Label *name = new Gtk::Label ();
	name->set_markup (markup);
	name->show ();
	display.pack_start (*Gtk::manage (name), Gtk::PACK_SHRINK);
}


void
TableTable::on_color_by_changed (PropertyBase* property)
{
	if (color_by == property) return;
	color_by = property;
	
	legend.set_label (compose::ucompose (_("_Legend (<b>%1</b>)"),
		(color_by != NULL) ? color_by->get_name () : _("None")));

	if (color_by == &P_PHASE)
		conditions.show ();
	else
		conditions.hide ();

	if (CAST (color_by, FloatProperty))
		scales.show ();
	else
		scales.hide ();

	update_colorbar ();
	update_colors ();

	if (color_by == &P_PHASE)
		legend.set_expanded (true);
}


void
TableTable::on_temperature_changed ()
{
	if (color_by == &P_PHASE)
		throttle_update_temperature.queue
			(sigc::mem_fun (*this, &TableTable::update_colors), true);
}


void
TableTable::on_logarithmic_toggled ()
{
	if (CAST (color_by, FloatProperty))
	{
		update_colorbar ();
		update_colors ();
	}
}


void
TableTable::on_legend_toggled (bool from_legend)
{
	if (!show_legend) return;

	if (from_legend)
		show_legend->set_active (legend.get_expanded ());
	else
		legend.set_expanded (show_legend->get_active ());
}


void
TableTable::update_colorbar ()
{
	if (colorbar.get_child ())
		colorbar.remove ();

	Gtk::Table *table = new Gtk::Table (1, 1, true);
	table->set_col_spacings (2);
	table->set_row_spacings (2);
	Gtk::Widget *result = table;

	if (color_by == &P_SERIES)
	{
		table->resize (2, 5);
		for (int i = 0; i < 10; i++)
			table->attach (*Gtk::manage
				(new LegendButton (Series (Series::Value (i)))),
				(i < 5) ? i : i - 5, (i < 5) ? i + 1 : i - 5 + 1,
				(i < 5) ? 0 : 1, (i < 5) ? 1 : 2);
	}
	else if (color_by == &P_BLOCK)
	{
		table->resize (1, 5);
		table->attach (*Gtk::manage
			(new LegendButton (Block (Block::S))), 0, 1, 0, 1);
		table->attach (*Gtk::manage
			(new LegendButton (Block (Block::P))), 1, 2, 0, 1);
		table->attach (*Gtk::manage
			(new LegendButton (Block (Block::D))), 2, 3, 0, 1);
		table->attach (*Gtk::manage
			(new LegendButton (Block (Block::F))), 3, 4, 0, 1);
		table->attach (*Gtk::manage
			(new LegendButton (Block (Q_UNK))), 4, 5, 0, 1);
	}
	else if (color_by == &P_PHASE)
	{
		table->resize (1, 4);
		table->attach (*Gtk::manage
			(new LegendButton (Phase (Phase::SOLID))), 0, 1, 0, 1);
		table->attach (*Gtk::manage
			(new LegendButton (Phase (Phase::LIQUID))), 1, 2, 0, 1);
		table->attach (*Gtk::manage
			(new LegendButton (Phase (Phase::GAS))), 2, 3, 0, 1);
		table->attach (*Gtk::manage
			(new LegendButton (Phase (Q_UNK))), 3, 4, 0, 1);
	}
	else if (color_by == &P_LATTICE_TYPE)
	{
		table->resize (2, 5);
		for (int i = 0; i < 10; i++)
			table->attach (*Gtk::manage
				(new LegendButton (LatticeType (LatticeType::Value (i)))),
				(i < 5) ? i : i - 5, (i < 5) ? i + 1 : i - 5 + 1,
				(i < 5) ? 0 : 1, (i < 5) ? 1 : 2);
	}
	else
	{
		delete table;
		result = table = NULL;
	
		FloatProperty *float_prop = CAST (color_by, FloatProperty);
		if (float_prop != NULL && float_prop->is_scale_valid ())
		{
			Gtk::HBox *buttons = new Gtk::HBox (true, 2);

			LegendButton *button = new LegendButton (ColorValue (0.0));
			button->set_label (compose::ucompose1 (float_prop->get_minimum ()));
			buttons->pack_start (*Gtk::manage (button));
			
			button = new LegendButton (ColorValue (0.5));
			button->set_label (compose::ucompose1
				(float_prop->get_medium (logarithmic.get_active ())));
			buttons->pack_start (*Gtk::manage (button));
			
			button = new LegendButton (ColorValue (1.0));
			button->set_label (compose::ucompose1 (float_prop->get_maximum ()));
			buttons->pack_start (*Gtk::manage (button));

			button = new LegendButton (ColorValue (Q_UNK));
			button->set_label (Float (Q_UNK).get_string ());
			buttons->pack_start (*Gtk::manage (button));

			if (float_prop->has_format ())
			{
				misc::ComposeBox *format = new misc::ComposeBox
					(float_prop->get_format (), false, true);
				format->set_param (1, *Gtk::manage (buttons));
				result = format;
			}
			else
				result = buttons;
		}
	}

	if (result != NULL)
	{
		result->show_all ();
		colorbar.add (*Gtk::manage (result));
	}

	colorbar.property_visible () = colorbar.get_child ();
}


} // namespace gElemental
