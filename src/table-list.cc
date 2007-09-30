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
#include "dialogs.hh"
#include "table-list.hh"

#include <glib-object.h>
#include <gtk/gtkmain.h>
#include <gtk/gtktreeviewcolumn.h>
#include <gtkmm/menu.h>
#include <gtkmm/menuitem.h>
#include <gtkmm/radioaction.h>
#include <gtkmm/radiomenuitem.h>
#include <gtkmm/stock.h>

namespace gElemental {


//******************************************************************************
// class TableListColumns


TableListColumns&
TableListColumns::get ()
{
	static TableListColumns the;
	return the;
}


TableListColumns::TableListColumns ()
{
	add (number);
	add (symbol);
	add (color);
	add (compliment);
	add (name);
	add (group);
	add (period);
	add (block);
	add (el);
}


//******************************************************************************
// class TableListStore


RefPtr<TableListStore>
TableListStore::create ()
{
	return RefPtr<TableListStore> (new TableListStore ());
}


TableListStore::TableListStore ()
:	Gtk::ListStore (TableListColumns::get ()),
	cols (TableListColumns::get ())
{
	set_sort_func (cols.group, sigc::bind (sigc::mem_fun
		(*this, &TableListStore::on_value_sort<Int>), cols.group));
	set_sort_func (cols.period, sigc::bind (sigc::mem_fun
		(*this, &TableListStore::on_value_sort<Int>), cols.period));
	set_sort_func (cols.block, sigc::bind (sigc::mem_fun
		(*this, &TableListStore::on_value_sort<Block>), cols.block));

	CONST_FOREACH (Table, get_table (), el)
	{
		Gtk::TreeIter j = append ();
		j->set_value (cols.el, *el);
		j->set_value (cols.number, (*el)->number);
		j->set_value (cols.symbol, (*el)->symbol);
		color color = (*el)->get_property (P_COLOR).get_color ();
		j->set_value (cols.color, allocate (color));
		j->set_value (cols.compliment, allocate (color.get_compliment ()));
		j->set_value (cols.name, (*el)->get_property (P_NAME).get_string ());
		j->set_value (cols.group, (*el)->get_property (P_GROUP));
		j->set_value (cols.period, (*el)->get_property (P_PERIOD));
		j->set_value (cols.block, (*el)->get_property (P_BLOCK));
	}
}


int
TableListStore::default_sort (const Gtk::TreeIter& a, const Gtk::TreeIter& b)
{
	TableListColumns &cols = TableListColumns::get ();
	unsigned int a_number = a->get_value (cols.number),
		b_number = b->get_value (cols.number);

	if (a_number < b_number)
		return -1;
	else if (a_number > b_number)
		return 1;
	else
		return 0;
}


template<class T1>
int
TableListStore::on_value_sort (const Gtk::TreeIter& a, const Gtk::TreeIter& b,
	const Gtk::TreeModelColumn<T1>& col)
{
	if (a && b)
	{
		int result = a->get_value (col).compare (b->get_value (col));
		return (result != 0) ? result : default_sort (a, b);
	}
	else
		return 0;
}


//******************************************************************************
// class PropertyColumn


PropertyColumn::PropertyColumn (const RefPtr<Gtk::ListStore>& store_,
	Gtk::RadioButtonGroup& group)
:	SortColumn (ustring (), false, group, "ViewSortByExtra",
		_("by _Extra Property")),
	store (store_), el_col (TableListColumns::get ().el)
{
	store->set_sort_func (el_col,
		sigc::mem_fun (*this, &PropertyColumn::on_value_sort));

	set_expand (false);
	set_sort_column (el_col);
	
	set_property (NULL);
}


PropertyBase*
PropertyColumn::get_property () const
{
	return const_cast<PropertyBase*> (property);
}


void
PropertyColumn::set_property (PropertyBase* property_)
{
	property = property_;

	if (property != NULL)
	{
		set_title (property->get_name ());
		sort_action->set_sensitive (true);
		if (sort_action->get_active ()) // force a resort of the list
		{
			int unused; Gtk::SortType order;
			store->get_sort_column_id (unused, order);
			store->set_sort_column
				(Gtk::TreeSortable::DEFAULT_UNSORTED_COLUMN_ID, order);
			store->set_sort_column (el_col, order);
		}
	}
	else
	{
		set_title (_("..."));
		sort_action->set_sensitive (false);
		if (sort_action->get_active ())
			signal_unavailable.emit ();
	}
}


int
PropertyColumn::on_value_sort (const Gtk::TreeIter& a, const Gtk::TreeIter& b)
{
	if (a && b)
	{
		if (property == NULL)
			return TableListStore::default_sort (a, b);
		const Element *a_el = a->get_value (el_col),
			*b_el = b->get_value (el_col);
		const value_base &a_value = a_el->get_property_base (*property),
			&b_value = b_el->get_property_base (*property);
		int result = a_value.compare (b_value);
		return (result != 0) ? result : TableListStore::default_sort (a, b);
	}
	else
		return 0;
}


void
PropertyColumn::on_cell_data (Gtk::CellRenderer* cell,
	const Gtk::TreeIter& iter)
{
	if (cell == NULL || !iter) return;
	if (property != NULL)
	{
		const Element& el = *iter->get_value (el_col);
		const value_base& value = el.get_property_base (*property);
		if cast (cell, Gtk::CellRendererText, text)
		{
			text->property_markup ().set_value
				(value.get_string (property->get_format ()));
			const color_value_base *color_value =
				CAST (&value, const color_value_base);
			if (value.has_value () && color_value != NULL)
			{
				text->property_background_gdk ().set_value
					(allocate (color_value->get_color ()));
				text->property_foreground_gdk ().set_value
					(allocate (color_value->get_color ().get_compliment ()));
			}
			else
			{
				text->property_background_set ().set_value (false);
				text->property_foreground_set ().set_value (false);
			}
		}
	}
	else
	{
		if cast (cell, Gtk::CellRendererText, text)
		{
			text->property_markup ().set_value (ustring ());
			text->property_background_set ().set_value (false);
			text->property_foreground_set ().set_value (false);
		}
	}
}


//******************************************************************************
// class TableList


TableList::TableList (const RefPtr<Gtk::UIManager>& ui_)
:	cols (TableListColumns::get ()), store (TableListStore::create ()),
	view (store), ui (ui_), actions (Gtk::ActionGroup::create ()),
	order_recursing (false), prop_col (store, sort_group)
{
	set_policy (Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	view.set_size_request (300, 150);
	view.set_headers_visible (true);
	view.signal_row_activated ().connect
		(sigc::mem_fun (*this, &TableList::on_row_activated));
	add (view);

	Gtk::RadioButtonGroup order_group;
	actions->add (Gtk::Action::create ("ViewSortMenu", _("_Sort")));
	store->signal_sort_column_changed ().connect
		(sigc::mem_fun (*this, &TableList::on_sort_column_changed));

	ascending = Gtk::RadioAction::create (order_group, "ViewSortAscending",
		Gtk::Stock::SORT_ASCENDING);
	actions->add (ascending, sigc::bind (sigc::mem_fun
		(*this, &TableList::on_order_activated), Gtk::SORT_ASCENDING));
	
	descending = Gtk::RadioAction::create (order_group, "ViewSortDescending",
		Gtk::Stock::SORT_DESCENDING);
	actions->add (descending, sigc::bind (sigc::mem_fun
		(*this, &TableList::on_order_activated), Gtk::SORT_DESCENDING));

	SortColumn *col = new PrimitiveColumn<unsigned int> (cols.number,
		_("Number"), sort_group, "ViewSortByNumber", _("by Atomic N_umber"));
	prop_col.signal_unavailable.connect
		(sigc::mem_fun (*col, &SortColumn::activate));
	actions->add (col->get_sort_action ());
	view.append_column (*Gtk::manage (col));
	col->clicked ();

	col = new PrimitiveColumn<std::string> (cols.symbol, P_SYMBOL.get_name (),
		sort_group, "ViewSortBySymbol", _("by _Symbol"));
	actions->add (col->get_sort_action ());
	if cast (col->get_first_cell_renderer (), Gtk::CellRendererText, text)
	{
		col->add_attribute (text->property_background_gdk (), cols.color);
		col->add_attribute (text->property_foreground_gdk (), cols.compliment);
	}
	view.append_column (*Gtk::manage (col));

	col = new PrimitiveColumn<ustring> (cols.name, P_NAME.get_name (),
		sort_group, "ViewSortByName", _("by _Name"), true);
	actions->add (col->get_sort_action ());
	view.append_column (*Gtk::manage (col));
	
	col = new ValueColumn<Int> (cols.group, P_GROUP.get_name (),
		sort_group, "ViewSortByGroup", _("by _Group"));
	actions->add (col->get_sort_action ());
	view.append_column (*Gtk::manage (col));

	col = new ValueColumn<Int> (cols.period, P_PERIOD.get_name (),
		sort_group, "ViewSortByPeriod", _("by _Period"));
	actions->add (col->get_sort_action ());
	view.append_column (*Gtk::manage (col));

	col = new ValueColumn<Block> (cols.block, P_BLOCK.get_name (),
		sort_group, "ViewSortByBlock", _("by _Block"));
	actions->add (col->get_sort_action ());
	view.append_column (*Gtk::manage (col));

	actions->add (prop_col.get_sort_action ());
	view.append_column (prop_col);

	Gtk::RadioButtonGroup props_group;
	actions->add (Gtk::Action::create ("ViewPropertyMenu", _("_Property")));
	actions->add (Gtk::RadioAction::create (props_group, "ViewPropertyNone",
		_("_None")), sigc::bind (sigc::mem_fun
			(prop_col, &PropertyColumn::set_property), (PropertyBase*) NULL));

	ustring extra_ui = "<menuitem action='ViewPropertyNone' />";
	int n = 1;
	RefPtr<Gtk::Action> initial_action;
	FOREACH (std::list<Category*>, CATEGORIES, category)
	{
		ustring cname = compose::ucompose ("ViewCategory%1", n);
		actions->add (Gtk::Action::create (cname, (*category)->get_name ()));
		extra_ui += "<menu action='" + cname + "'>";

		int o = 1;
		FOREACH (std::list<PropertyBase*>, (*category)->properties, prop)
		{
			if (*prop == &P_NUMBER || *prop == &P_SYMBOL || *prop == &P_NAME ||
				*prop == &P_GROUP || *prop == &P_PERIOD || *prop == &P_BLOCK ||
				*prop == &P_COLOR)
				continue; // all already included

			ustring pname = compose::ucompose ("ViewProperty%1_%2", n, o++);
			RefPtr<Gtk::Action> paction = Gtk::RadioAction::create
				(props_group, pname, (*prop)->get_name ());
			if (*prop == &P_ATOMIC_MASS)
				initial_action = paction;
			actions->add (paction, sigc::bind (sigc::mem_fun
				(prop_col, &PropertyColumn::set_property), *prop));
			extra_ui += "<menuitem action='" + pname + "' />";
		}
		
		extra_ui += "</menu>";
		++n;
	}

	view.set_search_column (cols.name);

	ui->insert_action_group (actions);
	ui_id = ui->add_ui_from_string
	(
		"<ui>"
			"<menubar name='MenuBar'><menu action='ViewMenu'>"
				"<separator />"
				"<menu action='ViewPropertyMenu'>"
					+ extra_ui +
				"</menu>"
				"<menu action='ViewSortMenu'>"
					"<menuitem action='ViewSortByNumber' />"
					"<menuitem action='ViewSortBySymbol' />"
					"<menuitem action='ViewSortByName' />"
					"<menuitem action='ViewSortByGroup' />"
					"<menuitem action='ViewSortByPeriod' />"
					"<menuitem action='ViewSortByBlock' />"
					"<menuitem action='ViewSortByExtra' />"
					"<separator />"
					"<menuitem action='ViewSortAscending' />"
					"<menuitem action='ViewSortDescending' />"
				"</menu>"
			"</menu></menubar>"
			"<popup action='ExtraColumnPopup'>"
				+ extra_ui +
			"</popup>"
		"</ui>"
	);

	if (initial_action)
		initial_action->activate ();
	show_all ();
	g_signal_connect (prop_col.gobj ()->button, "button-press-event",
		G_CALLBACK (c_on_extra_column_clicked), this);
}


TableList::~TableList ()
{
	ui->remove_ui (ui_id);
	ui->remove_action_group (actions);
}


TableList::State
TableList::get_state () const
{
	State result;
	store->get_sort_column_id (result.sort_column, result.sort_order);
	result.extra_property = prop_col.get_property ();
	return result;
}


void
TableList::set_state (State& state)
{
	prop_col.set_property (state.extra_property);
	store->set_sort_column (state.sort_column, state.sort_order);
}


void
TableList::on_order_activated (Gtk::SortType order)
{
	int column; Gtk::SortType old_order;
	store->get_sort_column_id (column, old_order);

	if (!order_recursing && order != old_order)
	{
		store->set_sort_column (column, order);
		view.get_column (column)->set_sort_order (order);
	}
}


void
TableList::on_sort_column_changed ()
{
	int column; Gtk::SortType order;
	store->get_sort_column_id (column, order);

	if (ascending->get_active () != (order == Gtk::SORT_ASCENDING))
	{
		order_recursing = true;

		if (order == Gtk::SORT_ASCENDING)
			ascending->set_active (true);
		else
			descending->set_active (true);

		order_recursing = false;
	}
}


bool
TableList::c_on_extra_column_clicked (GtkWidget*, GdkEventButton* ev,
	TableList* self)
{
	return self->on_extra_column_clicked (ev);

}


bool
TableList::on_extra_column_clicked (GdkEventButton* ev)
{
	if (ev->button == 3)
		if cast (ui->get_widget ("/ExtraColumnPopup"), Gtk::Menu, popup)
			popup->popup (ev->button, ev->time);
	return false;
}


void
TableList::on_row_activated (const Gtk::TreePath& treepath,
	Gtk::TreeViewColumn*)
{
	Gtk::TreeIter iter = store->get_iter (treepath);
	if (!iter) return;

	const Element *el = iter->get_value (cols.el);
	if (el != NULL)
	{
		ElementDialog *dialog = new ElementDialog (*el);
		if cast (get_toplevel (), Gtk::Window, top)
			dialog->set_transient_for (*top);
		dialog->present ();
	}			
}


} // namespace gElemental
