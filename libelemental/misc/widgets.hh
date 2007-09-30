//! \file widgets.hh
//! An assortment of GTK+ widgets for reuse.
/*
 * This file is part of libmisc, an assortment of code for reuse.
 *
 * Copyright (C) 2006-2007 Kevin Daughtridge <kevin@kdau.com>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBMISC__WIDGETS_HH
#define LIBMISC__WIDGETS_HH

#include <gtkmm/box.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/table.h>

//******************************************************************************

namespace misc {

using Glib::ustring;

//******************************************************************************

//! A widget that arranges text and child widgets according to a format string.
/*! This widget parallels the compose::ucompose() functions, with child widgets
 * substituted for the \c %n parameters. It allows widgets to appear in the
 * middle of label text with the flexibility of format-string layout.
 *
 * If mnemonics are enabled, each single underline in the format string will
 * precede a mnemonic character for the next following widget. */
class ComposeBox
:	public Gtk::HBox
{
public:

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	class Param;
#endif

	//! A specifier for a parameter, in the form of a positive integer.
	typedef std::vector<Param*>::size_type ParamSpec;

	ComposeBox (const ustring& format = ustring (), bool use_mnemonic = false,
		bool use_markup = false);

	virtual ~ComposeBox ();

	//! Returns the current format string.
	ustring get_format () const;

	//! Returns whether underlines in the format indicate mnemonics.
	bool get_use_mnemonic () const;

	//! Returns whether the format contains Pango markup.
	bool get_use_markup () const;

	//! Sets the format string to be be used for box layout.
	/*! \param format A compose-style UTF-8 format string.
	 * \param use_mnemonic Whether underlines in the format indicate mnemonics.
	 * \param use_markup Whether the format contains Pango markup.
	 * \warning If use_markup is \c true, the included markup tags must not
	 * cross over parameter specifiers. All open tags must be closed before a
	 * specifier occurs. Percent signs (\%) inside tag attribute vales must be
	 * escaped (\%\%). */
	void set_format (const ustring& format, bool use_mnemonic = false,
		bool use_markup = false);

	//! Returns the widget for the identified parameter.
	//! Returns NULL if no widget is set for the parameter.
	Gtk::Widget* get_param (ParamSpec id);

	//! Returns the widget for the identified parameter.
	/*! If a widget is set for the parameter, sets \p options to the widget's
	 * Gtk::PackOptions, else returns NULL and does not set \p options. */
	Gtk::Widget* get_param (ParamSpec id, Gtk::PackOptions& options);

	//! Sets the widget to be substituted for the identified parameter.
	/*! \param id The specifier for a parameter.
	 * \param widget The widget to be substituted. Must not have a parent.
	 * \param options The box options with which the widget will be packed. */
	void set_param (ParamSpec id, Gtk::Widget& widget,
		Gtk::PackOptions options = Gtk::PACK_SHRINK);

	//! Sets the box options with which the widget will be packed.
	//! If no widget is set for the identified parameter, nothing is set.
	void set_param_options (ParamSpec id, Gtk::PackOptions options);

	//! Unsets the identified parameter.
	//! No widget will be substituted for it.
	void clear_param (ParamSpec id);

	//! Unsets all parameters.
	void clear_params ();

private:

	typedef std::pair<ustring, ParamSpec> PatternUnit;

	void update ();
	
	static void on_destroy (GtkWidget*, ComposeBox* self);
	void on_param_destroy (Param* param);

	ustring format;
	bool use_mnemonic, use_markup;
	
	std::vector<PatternUnit> pattern;
	std::vector<Param*> params;
};

//******************************************************************************

//! A widget that presents label text along with an icon.
class IconLabel
:	public Gtk::HBox
{
public:

	IconLabel (const ustring& icon_name, const ustring& text,
		const Gtk::IconSize& size = Gtk::ICON_SIZE_MENU,
		bool use_markup = false, bool use_underline = false);

	IconLabel (const Gtk::StockID& stock_id, const ustring& text,
		const Gtk::IconSize& size = Gtk::ICON_SIZE_MENU,
		bool use_markup = false, bool use_underline = false);

	IconLabel (Gtk::MessageType message_type, const ustring& text,
		const Gtk::IconSize& size = Gtk::ICON_SIZE_MENU,
		bool use_markup = false, bool use_underline = false);

	virtual ~IconLabel ();
	
	//! Sets the displayed icon to an icon from the current theme.
	void set_icon (const ustring& icon_name,
		const Gtk::IconSize& size = Gtk::ICON_SIZE_MENU);
		
	//! Sets the displayed icon to a GTK+ stock icon.
	void set_icon (const Gtk::StockID& stock_id,
		const Gtk::IconSize& size = Gtk::ICON_SIZE_MENU);
		
	//! Sets the displayed icon to correspond with a Gtk::MessageType.
	void set_icon (Gtk::MessageType message_type,
		const Gtk::IconSize& size = Gtk::ICON_SIZE_MENU);
	
	//! Returns the label text.
	ustring get_text () const;
	
	//! Returns whether the label text contains Pango markup.
	bool get_use_markup () const;

	//! Returns whether the label text contains mnemonic underlines.
	bool get_use_underline () const;

	//! Sets the text displayed in the label.
	/*! \param text The UTF-8 string to be displayed.
	 * \param use_markup Whether the text contains Pango markup.
	 * \param use_underline Whether the text contains mnemonic underlines. */
	void set_text (const ustring& text, bool use_markup = false,
		bool use_underline = false);
	
	//! Sets the widget to be activated by the mnemonic key, if enabled.
	void set_mnemonic_widget (Gtk::Widget& target);

private:

	void init_common ();

	Gtk::Image image;
	Gtk::Label label;
};

//******************************************************************************

//! A widget that arranges headings and labelled widgets in a HIG-based table.
class InfoTable
:	public Gtk::Table
{
public:

	//!
	/*! \param labels_yalign The Y-axis alignment of labels created internally.
	 * \param heading_spacing The vertical spacing to be used above headings. */
	InfoTable (Gtk::AlignmentEnum labels_yalign = Gtk::ALIGN_CENTER,
		unsigned int heading_spacing = 18);
		
	virtual ~InfoTable ();
	
	//! Increases indentation by one level
	/*! The maximum indentation is three levels from left. Applies to all
	 * subsequently added heading and items. */
	void indent ();

	//! Decreases indentation by one level.
	/*! The minimum indentation is zero levels from left. Applies to all
	 * subsequently added heading and items. */
	void unindent ();

	//! Adds a UTF-8 string as a heading label, spanning the table horizontally.
	/*! \param heading The text with which the heading label will be created.
	 * \param use_markup Whether the heading text contains markup in Pango's
	 * markup language.
	 * \param indent Whether to increase indentation after the heading. */
	Gtk::Label* add_heading (const ustring& heading, bool use_markup = false,
		bool indent = true);

	//! Adds a widget as a heading, spanning the table horizontally.
	/*! \param heading The widget to be added.
	 * \param indent Whether to increase indentation after the heading. */
	void add_heading (Gtk::Widget& heading, bool indent = true);
	
	//! Adds an item to the table with a text label.
	/*! The item is used as a target if possible. Otherwise, its first child
	 * widget is used. See the next function for details. */
	Gtk::Label* add_item (const ustring& label, Gtk::Widget& item,
		Gtk::AttachOptions xoptions = Gtk::EXPAND | Gtk::FILL,
		Gtk::AttachOptions yoptions = Gtk::SHRINK | Gtk::FILL);

	//! Adds an item to the table with a text label.
	/*! \param label The text with which the label will be created. Mnemonic
	 * underlines may be used.
	 * \param item The widget to be added.
	 * \param target The widget which will be activated by any mnemonic
	 * underline. May or may not be the same as item.
	 * \param xoptions For item; see Gtk::Table::attach.
	 * \param yoptions For item; see Gtk::Table::attach.
	 * \return The Gtk::Label widget created. */
	Gtk::Label* add_item (const ustring& label, Gtk::Widget& item,
		Gtk::Widget& target,
		Gtk::AttachOptions xoptions = Gtk::EXPAND | Gtk::FILL,
		Gtk::AttachOptions yoptions = Gtk::SHRINK | Gtk::FILL);
	
	//! Adds a labelled item to the table.
	//! xoptions and yoptions are for item; see Gtk::Table::attach.
	void add_item (Gtk::Widget& label, Gtk::Widget& item,
		Gtk::AttachOptions xoptions = Gtk::EXPAND | Gtk::FILL,
		Gtk::AttachOptions yoptions = Gtk::SHRINK | Gtk::FILL);
		
	//! Adds an unlabelled item to the table.
	//! For xoptions and yoptions, see Gtk::Table::attach.
	void add_item (Gtk::Widget& item,
		Gtk::AttachOptions xoptions = Gtk::EXPAND | Gtk::FILL,
		Gtk::AttachOptions yoptions = Gtk::SHRINK | Gtk::FILL);

private:

	unsigned int row, col;
	Gtk::AlignmentEnum labels_yalign;
	unsigned int heading_spacing;
	
	Gtk::Widget& rectify_attach (Gtk::Widget& item,
		Gtk::AttachOptions& xoptions, Gtk::AttachOptions& yoptions) const;
	
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	struct ItemLabel
	{
		Gtk::Widget *item, *label;
		sigc::connection notify;
	};
#endif

	typedef std::map<Gtk::Widget*, ItemLabel*> ItemLabelMap;
	ItemLabelMap item_labels;

	void update_label_visible (Gtk::Widget* item);
	void forget_label (const ItemLabelMap::iterator& iter);
	virtual void on_remove (Gtk::Widget* widget);
};

} // namespace misc

#endif // LIBMISC__WIDGETS_HH
