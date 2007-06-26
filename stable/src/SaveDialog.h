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

// gimmage: SaveDialog.h

#include <gtkmm/dialog.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>

class CSaveDialog : public Gtk::Dialog
{
public:
	CSaveDialog(	const Glib::ustring& title,
				Gtk::Window& parent,
				bool modal = true,
				bool use_separator = true);
	~CSaveDialog();

	// we make the entry public to make accessing it more convenient
	// one uses methods to set and get its text anyway
	Gtk::Entry	FileEntry;

protected:
	Gtk::Label 	Label;
};
