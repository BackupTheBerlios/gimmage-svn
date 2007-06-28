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

#include "PrintOperation.h"

CPrintOperation::CPrintOperation( 
	Glib::ustring filename, 
	std::list<Glib::ustring> filelist,
	const Glib::RefPtr<Gtk::PageSetup>& t_refPageSetup,
	const Glib::RefPtr<Gtk::PrintSettings>& t_refPrintSettings ) 
	{
	// nothing else can be set yet because references are still missing
	// store the refPageSetup which will become our PageSetup and the 
	// relevant PrintSettings
	refPageSetup = t_refPageSetup;
	refPrintSettings = t_refPrintSettings;
	image_filelist = filelist;
	image_filename = filename;
	ptrPrintPreviewWidget = NULL;
	
	signal_custom_widget_apply().connect( sigc::mem_fun( *this, &CPrintOperation::on_custom_widget_apply ) );
	}

CPrintOperation::~CPrintOperation() 
	{
#ifdef DEBUG
std::cout << "~CPRINTOPERATION: PrintOperation destroyed\n";
#endif // DEBUG
	//delete ptrPrintPreviewWidget;
	}

Glib::RefPtr<CPrintOperation> CPrintOperation::create( 
	Glib::ustring filename, 
	std::list<Glib::ustring> filelist,
	const Glib::RefPtr<Gtk::PageSetup> &t_refPageSetup,
	const Glib::RefPtr<Gtk::PrintSettings> &t_refPrintSettings )
	{
	return Glib::RefPtr<CPrintOperation>(new CPrintOperation( 
		filename, 
		filelist, 
		t_refPageSetup, 
		t_refPrintSettings ) );
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
	//Create a custom tab in the print dialog
	set_custom_tab_label("Image Placement");

	// create the preview widget, note that we send the address of the 
	// pointer so that we can change the pointer remotely
	Gtk::Widget* previewwidget = Gtk::manage( new CPrintPreviewWidget( 
		image_filename, 
		image_filelist,
		&ptrPrintPreviewWidget ));

	// if the pointer has been set, give the preview widget the page setup
	// and print_settings 	
	if( ptrPrintPreviewWidget != NULL )
		{
#ifdef DEBUG
std::cout << "ON_CREATE_CUSTOM_WIDGET: set_members: " << ptrPrintPreviewWidget << std::endl;
#endif // DEBUG
		ptrPrintPreviewWidget->set_members( refPageSetup, refPrintSettings );
		}			 
	
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
	
	// on create_custom_widget takes this pointer and inserts it into
	// the printing dialog
	return previewwidget;
	}
	
void CPrintOperation::on_custom_widget_apply(Gtk::Widget* widget)
	{
	std::cout << "custom widget apply called\n";
	}
