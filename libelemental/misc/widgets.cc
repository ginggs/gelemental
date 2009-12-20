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

#include "macros.hh"
#include "widgets.hh"

#include <glib/gmessages.h>
#include <gtkmm/alignment.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <gtkmm/stock.h>
#include <gtkmm/togglebutton.h>

namespace misc {


//******************************************************************************


class ComposeBox::Param
{
public:

	Param (Gtk::Widget& widget, Gtk::PackOptions options);
	~Param ();
	
	Gtk::Widget &widget;
	Gtk::PackOptions options;
	
	sigc::signal<void> signal_destroy;

private:

	GtkWidget *gobj;
	gulong conn;

	static void destroy (GtkWidget*, Param* self);
};


ComposeBox::Param::Param (Gtk::Widget& widget_, Gtk::PackOptions options_)
:	widget (widget_), options (options_), gobj (widget.gobj ()),
	conn (g_signal_connect (gobj, "destroy", G_CALLBACK (destroy), this))
{
	g_object_ref (gobj);
}


ComposeBox::Param::~Param ()
{
	g_signal_handler_disconnect (gobj, conn);
	g_object_unref (gobj);
}


void
ComposeBox::Param::destroy (GtkWidget*, Param* self)
{
	if (!self) return;
	self->signal_destroy.emit ();
	delete self;
}


//******************************************************************************
// class ComposeBox


ComposeBox::ComposeBox (const ustring& format_, bool use_mnemonic_,
	bool use_markup_)
:	Gtk::HBox (false, 0), format (), use_mnemonic (false), use_markup (false)
{
	g_signal_connect (gobj (), "destroy", G_CALLBACK (on_destroy), this);
	set_format (format_, use_mnemonic_, use_markup_);
}


ComposeBox::~ComposeBox ()
{
	clear_params ();
}


ustring
ComposeBox::get_format () const
{
	return format;
}


bool
ComposeBox::get_use_mnemonic () const
{
	return use_mnemonic;
}


bool
ComposeBox::get_use_markup () const
{
	return use_markup;
}


void
ComposeBox::set_format (const ustring& format_, bool use_mnemonic_,
	bool use_markup_)
{
	ustring fmt = format = format_;
	ustring::size_type b = 0, i = 0;
	
	pattern.clear ();
	
	while (i < fmt.length ())
	{
		if (fmt[i] == '%' && i + 1 < fmt.length ())
		{
			if (fmt[i + 1] == '%')
				fmt.replace (i++, 2, "%");
			else if (Glib::Unicode::isdigit (fmt[i + 1]))
			{
				ustring text = (i > b) ? fmt.substr (b, i - b) : ustring ();
				
				ustring::size_type n = 1;
				ParamSpec spec = 0;
				
				do
				{
					spec += Glib::Unicode::digit_value (fmt[i + n]);
					spec *= 10;
					++n;
				}
				while (i + n < fmt.length () &&
					Glib::Unicode::isdigit (fmt[i + n]));
				
				spec /= 10;
				
				pattern.push_back (PatternUnit (text, spec));
				
				i += n;
				b = i;
			}
		}
		else
			++i;
	}
	
	if (i > b)
		pattern.push_back (PatternUnit (fmt.substr (b, i - b), 0));

	use_mnemonic = use_mnemonic_;
	use_markup = use_markup_;
	update ();
}


Gtk::Widget*
ComposeBox::get_param (ParamSpec id)
{
	if (id <= 0 || id >= params.size () || !params.at (id))
		return NULL;

	return &params.at (id)->widget;
}


Gtk::Widget*
ComposeBox::get_param (ParamSpec id, Gtk::PackOptions& options)
{
	if (id <= 0 || id >= params.size () || !params.at (id))
		return NULL;

	options = params.at (id)->options;
	return &params.at (id)->widget;
}


void
ComposeBox::set_param (ParamSpec id, Gtk::Widget& widget,
	Gtk::PackOptions options)
{
	if (id <= 0) return;
	if (id >= params.size ())
		params.resize (id + 1, NULL);
	if (params.at (id))
		delete params.at (id);

	Param *param = new Param (widget, options);
	param->signal_destroy.connect (sigc::bind
		(sigc::mem_fun (*this, &ComposeBox::on_param_destroy), param));
	params.at (id) = param;
	update ();
}


void
ComposeBox::set_param_options (ParamSpec id, Gtk::PackOptions options)
{
	if (id <= 0 || id >= params.size () || !params.at (id)) return;

	params.at (id)->options = options;
	update ();
}


void
ComposeBox::clear_param (ParamSpec id)
{
	if (id <= 0 || id >= params.size () || !params.at (id)) return;

	delete params.at (id);
	params.at (id) = NULL;
	update ();
}


void
ComposeBox::clear_params ()
{
	FOREACH (std::vector<Param*>, params, param)
		if (*param)
		{
			delete *param;
			*param = NULL;
		}

	params.clear ();
	update ();
}


void
ComposeBox::update ()
{
	while (!params.empty () && !params.back ())
		params.erase (--params.end ());

	if (!gobj ()) return;

	std::vector<Param*> prms = params;
	children ().erase (children ().begin (), children ().end ());
	push_composite_child ();

	FOREACH (std::vector<PatternUnit>, pattern, unit)
	{
		Gtk::Label *label = unit->first.empty () ? NULL :
			new Gtk::Label (unit->first, use_mnemonic);
		
		if (label)
		{
			label->set_use_markup (use_markup);
			pack_start (*Gtk::manage (label), Gtk::PACK_SHRINK);
		}

		if (unit->second <= 0) continue;

		if (unit->second < prms.size () && prms.at (unit->second))
		{
			Gtk::Widget &widget = prms.at (unit->second)->widget;
			pack_start (widget, prms.at (unit->second)->options);
			prms.at (unit->second) = NULL;
			if (use_mnemonic && label)
				label->set_mnemonic_widget (widget);
		}
		else
			pack_start (*Gtk::manage (new Gtk::Image (Gtk::Stock::
				MISSING_IMAGE, Gtk::ICON_SIZE_MENU)), Gtk::PACK_SHRINK);
	}

	pop_composite_child ();
}


void
ComposeBox::on_destroy (GtkWidget*, ComposeBox* self)
{
	self->clear_params ();
}


void
ComposeBox::on_param_destroy (Param* param)
{
	std::vector<Param*>::iterator iter =
		std::find (params.begin (), params.end (), param);

	if (iter != params.end ())
	{
		*iter = NULL;
		update ();
	}
}


//******************************************************************************
// class IconLabel


IconLabel::IconLabel (const ustring& icon_name, const ustring& text,
	const Gtk::IconSize& size, bool use_markup, bool use_underline)
{
	init_common ();
	set_icon (icon_name, size);
	set_text (text, use_markup, use_underline);
}


IconLabel::IconLabel (const Gtk::StockID& stock_id, const ustring& text,
	const Gtk::IconSize& size, bool use_markup, bool use_underline)
{
	init_common ();
	set_icon (stock_id, size);
	set_text (text, use_markup, use_underline);
}


IconLabel::IconLabel (Gtk::MessageType message_type, const ustring& text,
	const Gtk::IconSize& size, bool use_markup, bool use_underline)
{
	init_common ();
	set_icon (message_type, size);
	set_text (text, use_markup, use_underline);
}


void
IconLabel::init_common ()
{
	set_spacing (6);

	pack_start (image, Gtk::PACK_SHRINK, 0);
	
	label.set_alignment (0.0, 0.5);
	label.set_line_wrap (true);
	pack_start (label, Gtk::PACK_EXPAND_WIDGET, 0);
	
	show_all_children ();
}


IconLabel::~IconLabel ()
{}


void
IconLabel::set_icon (const ustring& icon_name, const Gtk::IconSize& size)
{
	image.set_from_icon_name (icon_name, size);
}


void
IconLabel::set_icon (const Gtk::StockID& stock_id, const Gtk::IconSize& size)
{
	image.set (stock_id, size);
}


void
IconLabel::set_icon (Gtk::MessageType message_type, const Gtk::IconSize& size)
{
	switch (message_type) 
	{
	case Gtk::MESSAGE_INFO:
		set_icon (Gtk::Stock::DIALOG_INFO, size);
		break;
	case Gtk::MESSAGE_QUESTION:
		set_icon (Gtk::Stock::DIALOG_QUESTION, size);
		break;
	case Gtk::MESSAGE_WARNING:
		set_icon (Gtk::Stock::DIALOG_WARNING, size);
		break;
	case Gtk::MESSAGE_ERROR:
		set_icon (Gtk::Stock::DIALOG_ERROR, size);
		break;
	case Gtk::MESSAGE_OTHER:
	default:
		set_icon (Gtk::Stock::MISSING_IMAGE, size);
		break;
	}
}


ustring
IconLabel::get_text () const
{
	return label.get_label ();
}


bool
IconLabel::get_use_markup () const
{
	return label.get_use_markup ();
}


bool
IconLabel::get_use_underline () const
{
	return label.get_use_underline ();
}


void
IconLabel::set_text (const ustring& text, bool use_markup, bool use_underline)
{
	if (use_markup) {
		if (use_underline)
			label.set_markup_with_mnemonic (text);
		else
			label.set_markup (text);
	} else {
		if (use_underline)
			label.set_text_with_mnemonic (text);
		else
			label.set_text (text);
	}
}


void
IconLabel::set_mnemonic_widget (Gtk::Widget& target)
{
	label.set_mnemonic_widget (target);
}


//******************************************************************************
// class InfoTable


InfoTable::InfoTable (Gtk::AlignmentEnum labels_yalign_,
	unsigned int heading_spacing_)
:	Gtk::Table (1, 5, false), row (0), col (0),
	labels_yalign (labels_yalign_), heading_spacing (heading_spacing_)
{
	set_col_spacings (18);
	set_col_spacing (3, 12);
	set_row_spacings (6);
}


InfoTable::~InfoTable ()
{
	while (!item_labels.empty ())
		forget_label (item_labels.begin ());
}


void
InfoTable::indent ()
{
	if (col < 3)
		++col;
}


void
InfoTable::unindent ()
{
	if (col > 0)
		--col;
}


Gtk::Label*
InfoTable::add_heading (const ustring& heading_, bool use_markup, bool indent_)
{
	push_composite_child ();
	Gtk::Label *heading = new Gtk::Label (use_markup ? heading_
		: "<b>" + Glib::Markup::escape_text (heading_) + "</b>",
		Gtk::ALIGN_LEFT, labels_yalign, false);
	heading->set_use_markup (true);
	heading->show ();
	pop_composite_child ();
	
	add_heading (*Gtk::manage (heading), indent_);
	return heading;
}


void
InfoTable::add_heading (Gtk::Widget& heading, bool indent_)
{
	if (row > 0)
		set_row_spacing (row - 1, heading_spacing);

	attach (heading, col, 5, row, row + 1,
		Gtk::EXPAND | Gtk::FILL, Gtk::SHRINK | Gtk::FILL);
	++row;

	if (indent_)
		indent ();
}


Gtk::Label*
InfoTable::add_item (const ustring& label, Gtk::Widget& item,
	Gtk::AttachOptions xoptions, Gtk::AttachOptions yoptions)
{
	Gtk::Widget *target = &item;

	if (!item.get_can_focus ())
		if cast (&item, Gtk::Container, container)
			if (!container->get_children ().empty ())
				target = *container->get_children ().begin ();

	return add_item (label, item, *target, xoptions, yoptions);
}


Gtk::Label*
InfoTable::add_item (const ustring& label_, Gtk::Widget& item,
	Gtk::Widget& target, Gtk::AttachOptions xoptions,
	Gtk::AttachOptions yoptions)
{
	push_composite_child ();
	Gtk::Label *label =
		new Gtk::Label (label_, Gtk::ALIGN_LEFT, labels_yalign, true);
	label->set_mnemonic_widget (target);
	pop_composite_child ();
	
	add_item (*Gtk::manage (label), item, xoptions, yoptions);
	return label;
}


void
InfoTable::add_item (Gtk::Widget& label, Gtk::Widget& item,
	Gtk::AttachOptions xoptions, Gtk::AttachOptions yoptions)
{
	if (item_labels.find (&item) != item_labels.end () ||
		item_labels.find (&label) != item_labels.end ())
	{
		g_warning ("Attempting to add an item with type %s to a "
			"misc::InfoTable, but the item or label is already in the table.",
			g_type_name (G_OBJECT_TYPE (item.gobj ())));
		return;
	}

	attach (label, col, 4, row, row + 1,
		Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK | Gtk::FILL);

	Gtk::Widget &final = rectify_attach (item, xoptions, yoptions);
	attach (final, 4, 5, row, row + 1, xoptions, yoptions);
	++row;

	ItemLabel *data = new ItemLabel ();
	data->item = &item;
	data->label = &label;
	data->notify =
		item.property_visible ().signal_changed ().connect (sigc::bind
			(sigc::mem_fun (*this, &InfoTable::update_label_visible), &item));

	item_labels[&item] = data;
	item_labels[&label] = data;
	update_label_visible (&item);
}


void
InfoTable::add_item (Gtk::Widget& item, Gtk::AttachOptions xoptions,
	Gtk::AttachOptions yoptions)
{
	Gtk::Widget &final = rectify_attach (item, xoptions, yoptions);
	attach (final, col, 5, row, row + 1, xoptions, yoptions);
	++row;
}


Gtk::Widget&
InfoTable::rectify_attach (Gtk::Widget& item, Gtk::AttachOptions& xoptions,
	Gtk::AttachOptions& yoptions) const
{
	if ((xoptions & Gtk::FILL) && (yoptions & Gtk::FILL))
		return item;
	else
	{
		push_composite_child ();
		Gtk::Alignment *align =
			new Gtk::Alignment (Gtk::ALIGN_LEFT, Gtk::ALIGN_TOP,
				(xoptions & Gtk::FILL) ? 1.0 : 0.0,
				(yoptions & Gtk::FILL) ? 1.0 : 0.0);
		align->add (item);
		pop_composite_child ();

		xoptions |= Gtk::FILL;
		yoptions |= Gtk::FILL;
		return *Gtk::manage (align);
	}
}


void
InfoTable::update_label_visible (Gtk::Widget* item)
{
	ItemLabelMap::iterator iter = item_labels.find (item);
	if (iter != item_labels.end ())
		iter->second->label->property_visible () =
			item->property_visible ().get_value ();
}


void
InfoTable::forget_label (const ItemLabelMap::iterator& iter)
{
	if (iter == item_labels.end ()) return;
	ItemLabel *data = iter->second;
	
	item_labels.erase (data->item);
	item_labels.erase (data->label);
	data->notify.disconnect ();
	delete data;
}


void
InfoTable::on_remove (Gtk::Widget* widget)
{
	Gtk::Table::on_remove (widget);
	forget_label (item_labels.find (widget));
}


} // namespace misc
