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

// gimmage: PrintOperation.cpp

/* Implementation of PrintOperation */

#include "Print.h"

CPrintOperation::CPrintOperation( Glib::ustring filename, std::list<Glib::ustring> filenames ) 
	{
	// nothing else can be set yet because references are still missing
	image_filenames = filenames;
	image_filename = filename;
	}

CPrintOperation::~CPrintOperation() 
	{
	delete previewwidget;
	}

Glib::RefPtr<CPrintOperation> CPrintOperation::create( Glib::ustring filename, std::list<Glib::ustring> filenames )
	{
	return Glib::RefPtr<CPrintOperation>(new CPrintOperation( filename, filenames ) );
	}
	
void CPrintOperation::on_begin_print(const Glib::RefPtr<Gtk::PrintContext>& print_context)
	{
	std::cout << "on_begin_print\n";
	}

void CPrintOperation::on_draw_page(const Glib::RefPtr<Gtk::PrintContext>& print_context, int page_nr)
	{
	}


Gtk::Widget* CPrintOperation::on_create_custom_widget()
	{
	get_print_settings()->set_orientation(Gtk::PAGE_ORIENTATION_LANDSCAPE);
	
	//Create a custom tab in the print dialog titled "Other"
	set_custom_tab_label("Image Placement");

	// create the preview widget 
	previewwidget = new 
		CPrintPreviewWidget( get_default_page_setup(),
		get_print_settings(),
		image_filename,
		image_filenames );
	
  /* Create a custom tab in the print dialog titled "Other"
  set_custom_tab_label("Other");

  Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox());
  vbox->set_border_width(12);

  Gtk::HBox* hbox = Gtk::manage(new Gtk::HBox(false, 8));
  vbox->pack_start(*hbox, false, false);
  hbox->set_border_width(6);
  hbox->show();

  Gtk::Label* label = Gtk::manage(new Gtk::Label("Choose a font: "));
  hbox->pack_start(*label, false, false);
  label->show();

  m_FontButton.set_font_name(m_Font);
  hbox->pack_start(m_FontButton, false, false);
  m_FontButton.show();

  return vbox;	// */

	return previewwidget;
	}
	
void CPrintOperation::on_custom_widget_apply(Gtk::Widget* widget)
	{
	}
