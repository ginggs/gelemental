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

#ifndef GELEMENTAL__MAIN_HH
#define GELEMENTAL__MAIN_HH

#include "dialogs.hh"
#include "table-table.hh"
#include "table-list.hh"

#include <glibmm/optiongroup.h>
#include <gtkmm/box.h>
#include <gtkmm/window.h>

//******************************************************************************

namespace gElemental {

int main (int& argc, char**& argv);

//******************************************************************************

class MainOptionGroup
:	public Glib::OptionGroup
{
public:

	MainOptionGroup ();

	vecustrings print;
	void handle_print ();
	
	bool interactive;

	vecustrings show;
	void handle_show (Gtk::Window& window);
};

//******************************************************************************

class MainWindow
:	public Gtk::Window
{
public:

	MainWindow ();
	virtual ~MainWindow ();
	
	void show_properties ();
	void hide_properties ();

private:

	void show_table ();
	void show_list ();
	
	void on_about ();

	RefPtr<Gtk::UIManager> ui;
	RefPtr<Gtk::ActionGroup> actions;

	Gtk::VBox vbox;

	TableTable *table;
	TableTable::State *table_state;

	TableList *list;
	TableList::State *list_state;

	PropertiesDialog *properties;
};

} // namespace gElemental

#endif // GELEMENTAL__MAIN_HH
