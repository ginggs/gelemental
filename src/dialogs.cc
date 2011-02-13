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
#include "dialogs.hh"
#include "buttons.hh"

#include <gdk/gdkkeysyms.h>
#include <gtkmm/accelgroup.h>
#include <gtkmm/alignment.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/notebook.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/stock.h>

namespace gElemental {


//******************************************************************************
// class ElementDialog


ElementDialog::ElementDialog (const Element& el_)
:	el (el_), tips ()
{
	set_border_width (6);
	set_resizable (false);
	set_type_hint (Gdk::WINDOW_TYPE_HINT_DIALOG);
	set_position (Gtk::WIN_POS_CENTER_ON_PARENT);

	tips.enable ();
	
	Gtk::VBox *vbox = new Gtk::VBox (false, 6);
	add (*Gtk::manage (vbox));

	Gtk::Notebook *book = new Gtk::Notebook ();
	vbox->pack_start (*Gtk::manage (book), Gtk::PACK_EXPAND_WIDGET);

	EntriesTable *table = new EntriesTable (tips, true);
	book->append_page (*Gtk::manage (table), _("General"));

	Gtk::Alignment *align =
		new Gtk::Alignment (Gtk::ALIGN_RIGHT, Gtk::ALIGN_CENTER, 0.0, 0.0);
	ElementIdentity *identity = new ElementIdentity (el);
	align->add (*Gtk::manage (identity));

	Gtk::Label *name = new Gtk::Label ();
	name->set_alignment (Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER);
	name->set_markup
		(el.get_property (P_NAME).get_string ("<big><b>%1</b></big>"));
	name->set_selectable ();

	table->add_item (*Gtk::manage (align), *Gtk::manage (name));

	el.make_entries (*table, C_GENERAL, false);
	el.make_entries (*table, C_HISTORICAL);
	el.make_entries (*table, C_MISCELLANEOUS);
	table->trim ();
	set_title (table->get_primary_header ());

	table = new EntriesTable (tips, true);
	el.make_entries (*table, C_PHYSICAL);
	el.make_entries (*table, C_THERMAL);
	table->trim ();
	if (table->get_children ().empty ())
		delete table;
	else
		book->append_page (*Gtk::manage (table), table->get_primary_header ());	

	table = new EntriesTable (tips, true);
	el.make_entries (*table, C_ATOMIC);
	el.make_entries (*table, C_CRYSTALLOGRAPHIC);
	el.make_entries (*table, C_ELECTRONIC);
	table->trim ();
	if (table->get_children ().empty ())
		delete table;
	else
		book->append_page (*Gtk::manage (table), table->get_primary_header ());	

	Gtk::HButtonBox *hbox = new Gtk::HButtonBox (Gtk::BUTTONBOX_END);
	vbox->pack_start (*Gtk::manage (hbox), Gtk::PACK_SHRINK);
	
	Gtk::Button *close = new Gtk::Button (Gtk::Stock::CLOSE);
	close->add_accelerator ("clicked", get_accel_group (), GDK_Escape,
		Gdk::ModifierType (0), Gtk::AccelFlags (0));
	close->signal_clicked ().connect
		(sigc::mem_fun (*this, &Gtk::Window::hide));
	hbox->pack_end (*Gtk::manage (close), Gtk::PACK_SHRINK);

	show_all_children ();
}


void
ElementDialog::on_hide ()
{
	Gtk::Window::on_hide ();
	delete this;
}


//******************************************************************************
// class PropertiesColumns


PropertiesColumns&
PropertiesColumns::get ()
{
	static PropertiesColumns the;
	return the;
}


PropertiesColumns::PropertiesColumns ()
{
	add (category);
	add (property);
}


//******************************************************************************
// class PropertiesDialog


PropertiesDialog::PropertiesDialog (Gtk::Window& parent)
:	cols (PropertiesColumns::get ()),
	store (Gtk::TreeStore::create (PropertiesColumns::get ())),
	view (store), info (NULL)
{
	set_title (_("Properties Information"));
	set_border_width (6);
	set_type_hint (Gdk::WINDOW_TYPE_HINT_DIALOG);
	set_position (Gtk::WIN_POS_CENTER_ON_PARENT);
	set_transient_for (parent);

	Gtk::VBox *vbox = new Gtk::VBox (false, 6);
	add (*Gtk::manage (vbox));

	Gtk::HBox *hboxpar = new Gtk::HBox (false, 6);
	vbox->pack_start (*Gtk::manage (hboxpar), Gtk::PACK_EXPAND_WIDGET);
	
	Gtk::TreePath first_prop;
	FOREACH (std::list<Category*>, CATEGORIES, category)
	{
		Gtk::TreeIter iter = store->append ();
		iter->set_value (cols.category, *category);
		iter->set_value (cols.property, (PropertyBase*) NULL);
		FOREACH (std::list<PropertyBase*>, (*category)->properties, prop)
		{
			Gtk::TreeIter iter2 = store->append (iter->children ());
			iter2->set_value (cols.category, (Category*) NULL);
			iter2->set_value (cols.property, *prop);
			if (first_prop.empty ())
				first_prop = store->get_path (iter2);
		}
	}

	Gtk::ScrolledWindow *scroll = new Gtk::ScrolledWindow ();
	scroll->set_border_width (6);
	scroll->set_policy (Gtk::POLICY_NEVER, Gtk::POLICY_NEVER);
	scroll->set_shadow_type (Gtk::SHADOW_IN);
	hboxpar->pack_start(*Gtk::manage (scroll), Gtk::PACK_SHRINK);

	view.get_selection ()->set_select_function
		(sigc::mem_fun (*this, &PropertiesDialog::is_selectable));
	view.get_selection ()->signal_changed ().connect
		(sigc::mem_fun (*this, &PropertiesDialog::on_selection_changed));
	view.expand_all ();
	if (!first_prop.empty ())
		view.get_selection ()->select (first_prop);
	scroll->add (view);

	Gtk::CellRendererText *cell = new Gtk::CellRendererText ();
	Gtk::TreeViewColumn *col =
		new Gtk::TreeViewColumn (_("Property"), *Gtk::manage (cell));
	col->set_cell_data_func (*cell,
		sigc::mem_fun (*this, &PropertiesDialog::on_tree_cell_data));
	view.append_column (*Gtk::manage (col));

	Gtk::ScrolledWindow *scroll2 = new Gtk::ScrolledWindow ();
	scroll2->set_border_width (6);
	scroll2->set_policy (Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
	scroll2->set_shadow_type (Gtk::SHADOW_NONE);
	hboxpar->pack_end(*Gtk::manage (scroll2), Gtk::PACK_EXPAND_WIDGET);
	scroll2->add (hbox);
	
	Gtk::HButtonBox *bbox = new Gtk::HButtonBox (Gtk::BUTTONBOX_END);
	vbox->pack_start (*Gtk::manage (bbox), Gtk::PACK_SHRINK);
	
	Gtk::Button *close = new Gtk::Button (Gtk::Stock::CLOSE);
	close->add_accelerator ("clicked", get_accel_group (), GDK_Escape,
		Gdk::ModifierType (0), Gtk::AccelFlags (0));
	close->signal_clicked ().connect
		(sigc::mem_fun (*this, &Gtk::Window::hide));
	bbox->pack_end (*Gtk::manage (close), Gtk::PACK_SHRINK);

	show_all_children ();
	scroll->set_policy (Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);

	int width, height;
	get_size (width, height);
	set_size_request (-1, width * 2 / 3);
}


PropertiesDialog::~PropertiesDialog ()
{
	if (info != NULL)
	{
		hbox.remove (*info);
		delete info;
	}
}


void
PropertiesDialog::on_tree_cell_data (Gtk::CellRenderer* cell,
	const Gtk::TreeIter& iter)
{
	if cast (cell, Gtk::CellRendererText, text)
	{
		if (Category *category = iter->get_value (cols.category))
			text->property_text ().set_value (category->get_name ());
		else if (PropertyBase *prop = iter->get_value (cols.property))
			text->property_text ().set_value (prop->get_name ());
		else
			text->property_text ().set_value (ustring ());
	}
}


bool
PropertiesDialog::is_selectable (const RefPtr<Gtk::TreeModel>&,
	const Gtk::TreePath& tpath, bool)
{
	Gtk::TreeIter iter = store->get_iter (tpath);
	return iter ? iter->get_value (cols.property) : false;
}


void
PropertiesDialog::on_selection_changed ()
{
	if (info != NULL)
	{
		hbox.remove (*info);
		delete info;
		info = NULL;
	}
	
	Gtk::TreeIter iter = view.get_selection ()->get_selected ();
	if (!iter) return;

	PropertyBase *prop = iter->get_value (cols.property);
	if (prop == NULL) return;

	info = new EntriesTable (tips, false, false);
	info->set_border_width (6);

	info->header (prop->get_name ());
	info->entry (ustring (), prop->get_description ());

	if cast (prop, FloatProperty, float_prop)
		if (float_prop->is_scale_valid ())
		{
			info->header (_("Values"));
			info->entry (_("Minimum:"), Float (float_prop->get_minimum ())
				.get_string (float_prop->get_format ()));
			info->entry (_("Maximum:"), Float (float_prop->get_maximum ())
				.get_string (float_prop->get_format ()));
		}

	if (!prop->sources.empty ())
	{
		info->header (_("Sources"));
		int n = 1;
		FOREACH (std::list<const Message*>, prop->sources, source)
			(*source)->make_entry (*info, compose::ucompose ("%1.", n++));
	}
	
	hbox.pack_start (*info, Gtk::PACK_EXPAND_WIDGET);
	info->show ();
}

} // namespace gElemental
