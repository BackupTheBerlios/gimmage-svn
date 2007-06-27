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

// gimmage: PrintPreviewWidget.h

#include <iostream>

#include <gdkmm/pixbuf.h>

#include <gtkmm/printoperation.h>

#include <gtkmm/button.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/box.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/frame.h>
#include <gtkmm/iconview.h>

#include <gtkmm/liststore.h>
#include <gtkmm/treemodel.h>


#include "PagePreview.h"
#include "../config.h"

// the tree model for the iconview in the preview widget
class CImageListColumns : public Gtk::TreeModelColumnRecord
{
public:
	CImageListColumns()
		{ add(filenames_column); add(thumbnails_column); }

	Gtk::TreeModelColumn<Glib::ustring> filenames_column;
	Gtk::TreeModelColumn< Glib::RefPtr<Gdk::Pixbuf> > thumbnails_column;
};

class CPrintPreviewWidget : public Gtk::VBox
{
public:
	void hello() { std::cout << "hello\n"; }

	// needs to take all relevant data at time 
	CPrintPreviewWidget( Glib::ustring&,
		std::list<Glib::ustring>&,
		CPrintPreviewWidget** );
	~CPrintPreviewWidget();

	Glib::ustring& get_image_filename();
	
	// sets up the members that could not be set during creation due to
	// asynchronity issues
	void set_members( const Glib::RefPtr<Gtk::PageSetup>&, 
		const Glib::RefPtr<Gtk::PrintSettings>& );
		
	void on_page_setup_done(const Glib::RefPtr<Gtk::PageSetup>&);
	void on_button_page_setup(void);
	
protected:

	void populate_iconview();

	Glib::RefPtr<Gtk::PageSetup> refPageSetup;
	Glib::RefPtr<Gtk::PrintSettings> refPrintSettings;

	std::list<Glib::ustring> image_filelist;
	Glib::ustring image_filename;
	CPagePreview	Page;
	
	Gtk::HBox		TopHBox;
	Gtk::HButtonBox	HButtonBox;
	
	Gtk::Button	PageSetupButton;
	
	Gtk::Frame	PagePreviewFrame;
	Gtk::Frame	PageSettingsFrame;
	Gtk::Frame  ImageListFrame;
	
	Gtk::ScrolledWindow	ImageListScroller;
	
	CImageListColumns ImageListColumns;
	Glib::RefPtr<Gtk::ListStore> refImageList;
	Gtk::IconView ImageIconView;

private:

};
