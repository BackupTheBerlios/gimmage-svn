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

// gimmage: PrintOperation.h

#include <pangomm.h>
#include <gtkmm/printoperation.h>

#include "PrintPreviewWidget.h"

#include "defines.h"
#include "../config.h"

class CPrintOperation : public Gtk::PrintOperation
{
public:
	static Glib::RefPtr<CPrintOperation> create( 
		Glib::ustring , 
		std::list<Glib::ustring>,
		const Glib::RefPtr<Gtk::PageSetup> &,
		const Glib::RefPtr<Gtk::PrintSettings> &);
		
	~CPrintOperation();
	
	// this pointer is set from within the CPrintPreviewWidget to tell us 
	// its addreess, so that we can give it our PageSetup and PrintSettings
	
	CPrintPreviewWidget *ptrPrintPreviewWidget;
	void set_members_print_preview_widget( 
		const Glib::RefPtr<Gtk::PageSetup>&, 
		const Glib::RefPtr<Gtk::PrintSettings>& );

protected:

	Glib::RefPtr<Gtk::PageSetup> refPageSetup;
	Glib::RefPtr<Gtk::PrintSettings> refPrintSettings;	

	CPrintOperation( 
		Glib::ustring, 
		std::list<Glib::ustring>,
		const Glib::RefPtr<Gtk::PageSetup> &,
		const Glib::RefPtr<Gtk::PrintSettings> &);
	
	
	virtual void on_begin_print( const Glib::RefPtr<Gtk::PrintContext>& context );
	virtual void on_draw_page( const Glib::RefPtr<Gtk::PrintContext>& context, int page_nr );

	// Custom tab for preview page
	virtual Gtk::Widget* on_create_custom_widget();
	virtual void on_custom_widget_apply(Gtk::Widget* widget); // */
	
	Glib::ustring image_filename;
	std::list<Glib::ustring> image_filelist;

	Glib::RefPtr<Pango::Layout> m_refLayout;
};
	

	
