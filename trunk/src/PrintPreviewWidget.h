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

#include <glibmm/dispatcher.h>

#include <gtkmm/printoperation.h>

#include <gtkmm/button.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/box.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/frame.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/treeview.h>


#include "PagePreview.h"

#include "defines.h"
#include "../config.h"

class CPrintPreviewWidget : public Gtk::VBox
{
public:
	void hello() { std::cout << "hello\n"; }

	// needs to take all relevant data at time 
	CPrintPreviewWidget( Glib::ustring&,
		std::list<Glib::ustring>&,
		CPrintPreviewWidget**);
	~CPrintPreviewWidget();

	Glib::ustring& get_image_filename();
	
	// sets up the members that could not be set during creation due to
	// asynchronity issues
	void set_members( const Glib::RefPtr<Gtk::PageSetup>&, 
		const Glib::RefPtr<Gtk::PrintSettings>& );
		
	void on_page_setup_done(const Glib::RefPtr<Gtk::PageSetup>&);
	void on_button_page_setup(void);
	
	void populate_iconview(void);
	
protected:
	// object used to pass messages from the loader thread
	Glib::Dispatcher	imageReady;
	
	Glib::RefPtr<Gtk::PageSetup> refPageSetup;
	Glib::RefPtr<Gtk::PrintSettings> refPrintSettings;

	CPagePreview	Page;
	
	Gtk::HBox		TopHBox;
	Gtk::HButtonBox	HButtonBox;
	
	Gtk::Button	PageSetupButton;
	
	Gtk::Frame	PagePreviewFrame;
	Gtk::Frame	PageSettingsFrame;
	Gtk::Frame  ImageListFrame;
	
	Gtk::ScrolledWindow	ImageListScroller;

private:

};
