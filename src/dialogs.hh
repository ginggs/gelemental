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

#ifndef GELEMENTAL__DIALOGS_HH
#define GELEMENTAL__DIALOGS_HH

#include <libelemental/element.hh>
#include "misc.hh"

#include <gtkmm/box.h>
#include <gtkmm/tooltips.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treestore.h>
#include <gtkmm/treeview.h>
#include <gtkmm/window.h>

//******************************************************************************

namespace gElemental {

//******************************************************************************

class ElementDialog
:	public Gtk::Window
{
public:

	ElementDialog (const Element& el);

protected:

	virtual void on_hide ();

private:

	const Element& el;
	Gtk::Tooltips tips;
};

//******************************************************************************

class PropertiesColumns
:	public Gtk::TreeModelColumnRecord
{
public:
	
	static PropertiesColumns& get ();
	
	Gtk::TreeModelColumn<Category*> category;
	Gtk::TreeModelColumn<PropertyBase*> property;

protected:

	PropertiesColumns ();
};

//******************************************************************************

class PropertiesDialog
:	public Gtk::Window
{
public:

	PropertiesDialog (Gtk::Window& parent);
	
	virtual ~PropertiesDialog ();

private:

	void on_tree_cell_data (Gtk::CellRenderer* cell, const Gtk::TreeIter& iter);
	bool is_selectable (const RefPtr<Gtk::TreeModel>& model,
		const Gtk::TreePath& path, bool currently_selected);
	void on_selection_changed ();
	
	Gtk::HBox hbox;

	PropertiesColumns &cols;
	RefPtr<Gtk::TreeStore> store;
	Gtk::TreeView view;

	Gtk::Tooltips tips;
	EntriesTable *info;
};

} // namespace gElemental

#endif // GELEMENTAL__DIALOGS_HH
