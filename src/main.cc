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
#include "main.hh"

#include <clocale>
#include <iostream>
#include <locale>
#include <glib.h>
#include <glibmm/optionentry.h>
#include <gtk/gtkaboutdialog.h>
#include <gtk/gtkversion.h>
#include <gtk/gtkwindow.h>
#include <gtkmm/aboutdialog.h>
#include <gtkmm/icontheme.h>
#include <gtkmm/main.h>
#include <gtkmm/radioaction.h>
#include <gtkmm/stock.h>


//******************************************************************************
// Globals


int
main (int argc, char** argv)
{
	return gElemental::main (argc, argv);
}


//******************************************************************************

namespace gElemental {

	
int
main (int& argc, char**& argv)
{
	bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);

	Glib::init ();
	Glib::set_prgname (PACKAGE);
	Glib::set_application_name (_("gElemental"));

	try
	{
		MainOptionGroup opts;
		Glib::OptionContext context;
		context.set_main_group (opts);
#if GLIB_CHECK_VERSION (2, 12, 0)
		g_option_context_set_summary (context.gobj (), _("A periodic table "
			"viewer with detailed information on elements."));
#endif

		Gtk::Main kit (argc, argv, context);
		gtk_window_set_default_icon_name ("gelemental"); // unwrapped function

		try
		{
			std::locale::global (std::locale (std::setlocale (LC_ALL, NULL)));
		}
		catch (const std::runtime_error &)
		{
			std::locale::global (std::locale::classic ());
		}

		initialize ();

		if (!opts.print.empty ())
		{
			opts.handle_print ();
			if (opts.show.empty () && !opts.interactive)
				return EXIT_SUCCESS;
		}
		
		MainWindow window;
		window.present ();

		if (!opts.show.empty ())
			opts.handle_show (window);

		Gtk::Main::run (window);
		return EXIT_SUCCESS;
	}
	catch (Glib::OptionError &e)
	{
		std::cerr << e.what () << std::endl;
		std::cerr << compose::ucompose (_("Run '%1 --help' to see a full list "
			"of available command line options."), argv[0]) << std::endl;
		return EXIT_FAILURE;
	}
#ifndef ENABLE_DEBUG
	catch (...)
	{
		std::cerr << _("An unknown error occurred.") << std::endl;
		return EXIT_FAILURE;
	}
#endif // !ENABLE_DEBUG
}


//******************************************************************************
// class MainOptionGroup


MainOptionGroup::MainOptionGroup ()
:	Glib::OptionGroup (ustring (), ustring ()), interactive (false)
{
	set_translation_domain (GETTEXT_PACKAGE);

	Glib::OptionEntry entry_print;
	entry_print.set_long_name ("print");
	entry_print.set_short_name ('p');
	entry_print.set_description (N_("Print information on the indicated "
		"element(s) to standard output"));
	entry_print.set_arg_description (N_("SYMBOL_OR_NUMBER"));
	add_entry (entry_print, print);

	Glib::OptionEntry entry_show;
	entry_show.set_long_name ("show");
	entry_show.set_short_name ('s');
	entry_show.set_description (N_("Show the properties dialog for the "
		"indicated element(s)"));
	entry_show.set_arg_description (N_("SYMBOL_OR_NUMBER"));
	add_entry (entry_show, show);

	Glib::OptionEntry entry_interactive;
	entry_interactive.set_long_name ("interactive");
	entry_interactive.set_short_name ('i');
	entry_interactive.set_description (N_("Display the table even if data "
		"have been printed to standard output"));
	add_entry (entry_interactive, interactive);
}


void
MainOptionGroup::handle_print ()
{
	EntriesStream view (std::cout);

	FOREACH (vecustrings, print, el)
	{
		try
			{ get_element (*el).make_entries (view); }
		catch (std::invalid_argument& e)
		{
			throw Glib::OptionError (Glib::OptionError::BAD_VALUE,
				compose::ucompose (_("Unknown element \"%1\"."), *el));
		}
	}
}


void
MainOptionGroup::handle_show (Gtk::Window& window)
{
	FOREACH (vecustrings, show, el)
	{
		try
		{
			ElementDialog *dialog = new ElementDialog (get_element (*el));
			dialog->set_transient_for (window);
			dialog->present ();
		}
		catch (std::invalid_argument& e)
		{
			throw Glib::OptionError (Glib::OptionError::BAD_VALUE,
				compose::ucompose (_("Unknown element \"%1\"."), *el));
		}
	}
}


//******************************************************************************
// class MainWindow


MainWindow::MainWindow ()
:	ui (Gtk::UIManager::create ()), actions (Gtk::ActionGroup::create ()),
	table (NULL), table_state (NULL), list (NULL), list_state (NULL),
	properties (NULL)
{
	add (vbox);
	add_accel_group (ui->get_accel_group ());
	set_title (_("Periodic Table"));

	actions->add (Gtk::Action::create ("FileMenu", _("_File")));
	actions->add (Gtk::Action::create ("FileQuit", Gtk::Stock::QUIT),
		sigc::mem_fun (*this, &MainWindow::hide));

	actions->add (Gtk::Action::create ("ViewMenu", _("_View")));
	Gtk::RadioButtonGroup view_group;
	actions->add (Gtk::RadioAction::create (view_group, "ViewAsTable",
		_("as _Table")), sigc::mem_fun (*this, &MainWindow::show_table));
	actions->add (Gtk::RadioAction::create (view_group, "ViewAsList",
		_("as L_ist")), sigc::mem_fun (*this, &MainWindow::show_list));

	actions->add (Gtk::Action::create ("HelpMenu", _("_Help")));
	actions->add (Gtk::Action::create ("HelpProperties",
		Gtk::Stock::HELP, _("_Properties")), Gtk::AccelKey ("F1"),
		sigc::mem_fun (*this, &MainWindow::show_properties));
	actions->add (Gtk::Action::create ("HelpAbout", Gtk::Stock::ABOUT),
		sigc::mem_fun (*this, &MainWindow::on_about));

	ui->insert_action_group (actions);
	ui->add_ui_from_string
	(
		"<ui><menubar name='MenuBar'>"
			"<menu action='FileMenu'>"
				"<menuitem action='FileQuit' />"
			"</menu>"
			"<menu action='ViewMenu'>"
				"<menuitem action='ViewAsTable' />"
				"<menuitem action='ViewAsList' />"
			"</menu>"
			"<menu action='HelpMenu'>"
				"<menuitem action='HelpProperties' />"
				"<menuitem action='HelpAbout' />"
			"</menu>"
		"</menubar></ui>"
	);

	Gtk::Widget *menu_bar = ui->get_widget ("/MenuBar");
	if (menu_bar != NULL)
		vbox.pack_start (*Gtk::manage (menu_bar), Gtk::PACK_SHRINK);

	show_all_children ();
	show_table ();
}


MainWindow::~MainWindow ()
{
	delete properties;

	delete list_state;
	delete list;

	delete table_state;
	delete table;
}


void
MainWindow::show_properties ()
{
	if (properties == NULL)
	{
		properties = new PropertiesDialog (*this);
		properties->signal_hide ().connect
			(sigc::mem_fun (*this, &MainWindow::hide_properties));
	}
	properties->present ();
}


void
MainWindow::hide_properties ()
{
	delete properties;
	properties = NULL;
}


void
MainWindow::show_table ()
{
	if (list != NULL)
	{
		delete list_state;
		list_state = new TableList::State (list->get_state ());

		vbox.remove (*list);
		delete list;
		list = NULL;
	}

	if (table == NULL)
	{
		table = new TableTable (ui);

		if (table_state != NULL)
		{
			table->set_state (*table_state);
			delete table_state;
			table_state = NULL;
		}

		vbox.pack_start (*table, Gtk::PACK_EXPAND_WIDGET);
		table->grab_focus ();
	}
}


void
MainWindow::show_list ()
{
	if (table != NULL)
	{
		delete table_state;
		table_state = new TableTable::State (table->get_state ());

		vbox.remove (*table);
		delete table;
		table = NULL;
	}

	if (list == NULL)
	{
		list = new TableList (ui);

		if (list_state != NULL)
		{
			list->set_state (*list_state);
			delete list_state;
			list_state = NULL;
		}

		vbox.pack_start (*list, Gtk::PACK_EXPAND_WIDGET);
		list->grab_focus ();
	}
}


void
MainWindow::on_about ()
{
	Gtk::AboutDialog about;
	about.set_transient_for (*this);

	about.set_name (_("gElemental"));
	about.set_version (VERSION);
	about.set_comments (_("A periodic table viewer with detailed "
		"information on elements."));
	about.set_website ("https://github.com/ginggs/gelemental");
	about.set_logo_icon_name ("gelemental");

	about.set_copyright
	(
		"Copyright © 2016-2023 Graham Inggs\n"
		"Copyright © 2006-2007 Kevin Daughtridge\n"
		"Copyright © 2003 Jonas Frantz"
	);
	about.set_license (_("This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.\n\nThis program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.\n\nYou should have received a copy of the GNU General Public License along with this program; if not, see <http://www.gnu.org/licenses/>."));
#if GTK_CHECK_VERSION (2, 8, 0)
	gtk_about_dialog_set_wrap_license (about.gobj (), true);
#endif

	std::list<ustring> authors;
	authors.push_back ("Kevin Daughtridge <kevin@kdau.com>");
	authors.push_back ("Jonas Frantz <jonas.frantz@welho.com>");
	authors.push_back ("Kyle R. Burton <mortis@voicenet.com>");
	authors.push_back ("Jon Abbott <?>");
	authors.push_back ("Costantino Ceoldo <antigone34@libero.it>");
	authors.push_back ("Sebastien Bacher <seb128@debian.org>");
	authors.push_back ("Andrew Dalke <info@dalkescientific.com>");
	authors.push_back ("Georges Khaznadar <khazuada@univ-littoral.fr>");
	authors.push_back ("Arkadiusz Lipiec <alipiec@elka.pw.edu.pl>");
	authors.push_back ("Jean Meloche <?>");
	authors.push_back ("Piotr Najman <picio@bogucice.pl>");
	authors.push_back ("Felix Natter <f.natter@ndh.net>");
	authors.push_back ("Ambrogio Oliva <ambrogio.oliva@ctimilano.com>");
	authors.push_back ("Arno W. Peters <a.w.peters@ieee.org>");
	authors.push_back ("Daniel Leidert <dleidert@debian.org>");
	authors.push_back ("Cesare Tirabassi <norsetto@ubuntu.com>");
	authors.push_back ("Joachim Reichel <reichel@debian.org>");
	authors.push_back ("Graham Inggs <ginggs@riseup.net>");

	about.set_authors (authors);
	// Translators: Replace this with your name(s) and address(es).
	about.set_translator_credits (_("translator-credits"));

	about.present ();
	about.run ();
}


} // namespace gElemental
