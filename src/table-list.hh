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

#ifndef GELEMENTAL__TABLE_LIST_HH
#define GELEMENTAL__TABLE_LIST_HH

#include "misc.hh"

#include <gtkmm/actiongroup.h>
#include <gtkmm/liststore.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treeview.h>
#include <gtkmm/uimanager.h>

//******************************************************************************

namespace gElemental {

//******************************************************************************

class TableListColumns
:	public Gtk::TreeModelColumnRecord
{
public:
	
	static TableListColumns& get ();

	Gtk::TreeModelColumn<unsigned int> number;
	Gtk::TreeModelColumn<std::string> symbol;
	Gtk::TreeModelColumn<Gdk::Color> color, compliment;
	Gtk::TreeModelColumn<ustring> name;
	Gtk::TreeModelColumn<Int> group, period;
	Gtk::TreeModelColumn<Block> block;
	Gtk::TreeModelColumn<const Element*> el;

protected:

	TableListColumns ();
};

//******************************************************************************

class TableListStore
:	public Gtk::ListStore
{
public:

	static RefPtr<TableListStore> create ();

	static int default_sort (const Gtk::TreeIter& a, const Gtk::TreeIter& b);

protected:

	TableListStore ();

	template<class T1> int on_value_sort (const Gtk::TreeIter& a,
		const Gtk::TreeIter& b, const Gtk::TreeModelColumn<T1>& col);

	TableListColumns &cols;
};

//******************************************************************************

class PropertyColumn
:	public SortColumn
{
public:

	PropertyColumn (const RefPtr<Gtk::ListStore>& store,
		Gtk::RadioButtonGroup& group);
	
	PropertyBase* get_property () const;
	void set_property (PropertyBase* property);
	
	sigc::signal<void> signal_unavailable;
	
protected:

	virtual int on_value_sort (const Gtk::TreeIter& a, const Gtk::TreeIter& b);

	virtual void on_cell_data (Gtk::CellRenderer* cell,
		const Gtk::TreeIter& iter);

private:

	RefPtr<Gtk::ListStore> store;
	Gtk::TreeModelColumn<const Element*> &el_col;
	PropertyBase *property;
};

//******************************************************************************

class TableList
:	public Gtk::ScrolledWindow
{
public:

	TableList (const RefPtr<Gtk::UIManager>& ui);
	virtual ~TableList ();

	struct State
	{
		int sort_column;
		Gtk::SortType sort_order;
		PropertyBase *extra_property;
	};
	
	State get_state () const;
	void set_state (State& state);

private:

	void on_order_activated (Gtk::SortType order);
	void on_sort_column_changed ();

	static bool c_on_extra_column_clicked (GtkWidget*, GdkEventButton* ev,
		TableList* self);
	bool on_extra_column_clicked (GdkEventButton* ev);

	void on_row_activated (const Gtk::TreePath& treepath, Gtk::TreeViewColumn*);

	TableListColumns &cols;
	RefPtr<TableListStore> store;
	Gtk::TreeView view;
	
	RefPtr<Gtk::UIManager> ui;
	Gtk::UIManager::ui_merge_id ui_id;
	RefPtr<Gtk::ActionGroup> actions;
	RefPtr<Gtk::RadioAction> ascending, descending;
	bool order_recursing;

	Gtk::RadioButtonGroup sort_group;
	PropertyColumn prop_col;
};

} // namespace gElemental

#endif // GELEMENTAL__TABLE_LIST_HH
