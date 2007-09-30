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

#ifndef GELEMENTAL__MISC_HH
#define GELEMENTAL__MISC_HH

#include <libelemental/misc/extras.hh>
#include <libelemental/misc/widgets.hh>
#include <libelemental/value.hh>

#include <queue>
#include <stdexcept>
#include <glibmm/timer.h>
#include <gdkmm/color.h>
#include <gtkmm/radioaction.h>
#include <gtkmm/table.h>
#include <gtkmm/tooltips.h>
#include <gtkmm/treeviewcolumn.h>
#include <sigc++/connection.h>

//******************************************************************************

namespace gElemental {

using namespace Elemental;

Gdk::Color allocate (const color& color);

//******************************************************************************

class EntriesTable
:	public misc::InfoTable, public EntriesView
{
public:

	EntriesTable (Gtk::Tooltips& tips, bool primary = false,
		bool squeeze = true);
	
	virtual ~EntriesTable ();

	virtual void header (const ustring& category) throw ();

	virtual void entry (const ustring& name, const ustring& value,
		const ustring& tip = ustring ()) throw ();
	
	ustring get_primary_header () const;
	
	void trim ();

private:

	Gtk::Tooltips& tips;
	
	bool primary;
	ustring primary_header;
	
	bool squeeze;
	
	Gtk::Widget* last_header;
};

//******************************************************************************

class SortColumn
:	public Gtk::TreeViewColumn
{
public:

	SortColumn (const ustring& title, bool primary,
		Gtk::RadioButtonGroup& group, const ustring& action_name,
		const ustring& action_title);

	RefPtr<Gtk::RadioAction> get_sort_action () const;
	
	void activate ();

protected:

	virtual void on_cell_data (Gtk::CellRenderer* cell,
		const Gtk::TreeIter& iter) = 0;
	virtual void on_clicked ();
	virtual void on_sort_menu ();

	Gtk::CellRendererText cell_renderer;
	RefPtr<Gtk::RadioAction> sort_action;
};

//******************************************************************************

template<class T>
class PrimitiveColumn
:	public SortColumn
{
public:

	PrimitiveColumn (const Gtk::TreeModelColumn<T>& column,
		const ustring& title, Gtk::RadioButtonGroup& group,
		const ustring& action_name, const ustring& action_title,
		bool primary = false);

protected:

	virtual void on_cell_data (Gtk::CellRenderer* cell,
		const Gtk::TreeIter& iter);

	const Gtk::TreeModelColumn<T> &model_column;	
};

template<class T>
PrimitiveColumn<T>::PrimitiveColumn (const Gtk::TreeModelColumn<T>& column,
	const ustring& title, Gtk::RadioButtonGroup& group,
	const ustring& action_name, const ustring& action_title, bool primary)
:	SortColumn (title, primary, group, action_name, action_title),
	model_column (column)
{
	set_sort_column (model_column);
}

template<class T>
void
PrimitiveColumn<T>::on_cell_data (Gtk::CellRenderer* cell,
	const Gtk::TreeIter& iter)
{
	if (cell != NULL && iter)
	{
		const T& value = iter->get_value (model_column);
		if cast (cell, Gtk::CellRendererText, text)
			text->property_text ().set_value (compose::ucompose1 (value));
	}
}

//******************************************************************************

template<class T>
class ValueColumn
:	public SortColumn
{
public:

	ValueColumn (const Gtk::TreeModelColumn<T>& column, const ustring& title,
		Gtk::RadioButtonGroup& group, const ustring& action_name,
		const ustring& action_title, bool primary = false);

protected:

	virtual void on_cell_data (Gtk::CellRenderer* cell,
		const Gtk::TreeIter& iter);

	const Gtk::TreeModelColumn<T> &model_column;
};

template<class T>
ValueColumn<T>::ValueColumn (const Gtk::TreeModelColumn<T>& column,
	const ustring& title, Gtk::RadioButtonGroup& group,
	const ustring& action_name, const ustring& action_title, bool primary)
:	SortColumn (title, primary, group, action_name, action_title),
	model_column (column)
{
	set_sort_column (model_column);
}

template<class T>
void
ValueColumn<T>::on_cell_data (Gtk::CellRenderer* cell,
	const Gtk::TreeIter& iter)
{
	if (cell != NULL && iter)
	{
		const T& value = iter->get_value (model_column);
		if cast (cell, Gtk::CellRendererText, text)
		{
			text->property_text ().set_value (value.get_string ());
			const color_value_base *cv =
				dynamic_cast<const color_value_base*> (&value);
			if (cv != NULL && cv->has_value ())
			{
				text->property_background_gdk ().set_value
					(allocate (cv->get_color ()));
				text->property_foreground_gdk ().set_value
					(allocate (cv->get_color ().get_compliment ()));
			}
			else
			{
				text->property_background_set ().set_value (false);
				text->property_foreground_set ().set_value (false);
			}
		}
	}
}

} // namespace gElemental

#endif // GELEMENTAL__MISC_HH
