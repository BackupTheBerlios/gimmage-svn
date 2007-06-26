/* This file is part of gimmage

Copyright 2006 Bartek Kostrzewa

    gimmage is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301
    USA   */

// gimmage: SaveErrorDialog.cpp

#include <libintl.h>
#include "SaveErrorDialog.h"
#include "defines.h"

#include "../config.h"

CSaveErrorDialog::CSaveErrorDialog(
	const Glib::ustring& title,
	Gtk::Window& parent,
	bool modal,
	bool use_separator) : Gtk::Dialog::Dialog(title,parent,modal,use_separator)
	{
	add_button(Gtk::StockID("gtk-ok"),1);
	( get_vbox() )->pack_start(Label, Gtk::PACK_EXPAND_PADDING,10);
	Label.set_text( GT( "Saving failed. Please check whether you have sufficient "\
					"rights to the file and possibly try again. The image format "\
					"might be unsupported for saving or we may have run out of "\
					"memory. Currently we can save only JPEG, PNG and BMP."\
	                "( as limited by gdk-pixbuf )" ));
	Label.set_line_wrap();
	show_all_children();
	}

CSaveErrorDialog::~CSaveErrorDialog()
	{
	}
