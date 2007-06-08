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

// gimmage: SaveDialog.cpp
/* 	implementation of dialog which is shown
	when user clicks save button */

#include <libintl.h>
#include "SaveDialog.h"
#include "defines.h"

#include "../config.h"

CSaveDialog::CSaveDialog(
	const Glib::ustring& title,
	Gtk::Window& parent,
	bool modal,
	bool use_separator) : Gtk::Dialog::Dialog(title,parent,modal,use_separator)
	{
	( get_vbox() )->pack_start(Label,Gtk::PACK_EXPAND_PADDING,10);
	Label.set_text(	GT( "If you save the current file will be overwritten " \
					"which can result in quality losses. "\
					"Do you still want to save? " \
					"You can also specify a new filename below. " ) );
	Label.set_line_wrap();
	FileEntry.set_max_length(1023);
	( get_vbox() )->pack_start(FileEntry,Gtk::PACK_EXPAND_PADDING,5);
	add_button(Gtk::StockID("gtk-ok"),1);
	add_button(Gtk::StockID("gtk-cancel"),0);
	show_all_children();
	}

CSaveDialog::~CSaveDialog()
	{
	}
