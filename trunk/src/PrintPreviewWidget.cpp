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

// gimmage: PrintPreviewWidget.cpp

#include "PrintPreviewWidget.h"

CPrintPreviewWidget::CPrintPreviewWidget( Glib::ustring &filename,
		std::list<Glib::ustring> &filelist,
		CPrintPreviewWidget **t_ptrPrintPreviewWidget) :
	Page()
	{
	set_flags(Gtk::NO_WINDOW);
	
	// tell our parent print operation who we are
	*t_ptrPrintPreviewWidget = this;

#ifdef DEBUG
	std::cout << "CPRINTPREVIEWWIDGET: constructor\n";	
	std::cout << "CPRINTPREVIEWWIDGET: pointer address: " << t_ptrPrintPreviewWidget << std::endl;
	std::cout << "CPRINTPREVIEWWIDGET: address pointed to by pointer: " << *t_ptrPrintPreviewWidget << std::endl;
#endif // DEBUG

	image_filelist = filelist;
	image_filename = filename;
	
	refImageList = Gtk::ListStore::create( ImageListColumns );
	
	/*refPageSetup = _refPageSetup;
	refPrintSettings = _refPrintSettings;*/
	
	PagePreviewFrame.add(Page);
	TopHBox.pack_start(PagePreviewFrame,Gtk::PACK_SHRINK);
	
	ImageIconView.set_model(refImageList);
	ImageIconView.set_text_column( ImageListColumns.filenames_column );
	ImageIconView.set_pixbuf_column( ImageListColumns.thumbnails_column );
	ImageIconView.set_selection_mode(Gtk::SELECTION_MULTIPLE);
	
	Glib::Thread* const thread = Glib::Thread::create( 
		sigc::mem_fun(*this,&CPrintPreviewWidget::populate_iconview),
		false);

	ImageListScroller.add(ImageIconView);
	ImageListScroller.set_size_request(350,0);
	ImageListScroller.set_policy(Gtk::POLICY_AUTOMATIC,Gtk::POLICY_AUTOMATIC);
	ImageListFrame.add(ImageListScroller);
	TopHBox.pack_start(ImageListFrame,Gtk::PACK_EXPAND_WIDGET);
	
	pack_start(TopHBox,Gtk::PACK_SHRINK);
	pack_start(PageSettingsFrame,Gtk::PACK_SHRINK);
	
	PageSetupButton.set_label( "Page Size and Orientation" );
	HButtonBox.pack_start( PageSetupButton, Gtk::PACK_SHRINK );
	pack_start(HButtonBox,Gtk::PACK_SHRINK);
	
	Page.load( image_filename, true );
	
	show_all_children();
	
	PageSetupButton.signal_clicked().connect(
		sigc::mem_fun(*this,&CPrintPreviewWidget::on_button_page_setup));
	}
	
CPrintPreviewWidget::~CPrintPreviewWidget()
	{
#ifdef DEBUG
std::cout << "~CPRINTPREVIEWWIDGET: PrintPreviewWidget destroyed\n";
#endif // DEBUG
	refPageSetup.clear();
	refPrintSettings.clear();	
	}	
		
void CPrintPreviewWidget::set_members( 
	const Glib::RefPtr<Gtk::PageSetup> &t_refPageSetup,
	const Glib::RefPtr<Gtk::PrintSettings> &t_refPrintSettings )
	{
#ifdef DEBUG
std::cout << "CPRINTPREVIEWWIDGET::SET_MEMBERS: refPageSetup " << refPageSetup << std::endl;
std::cout << "CPRINTPREVIEWWIDGET::SET_MEMBERS: refPrintSettings " << refPrintSettings << std::endl;
#endif // DEBUG
	
		refPageSetup = t_refPageSetup;
		refPrintSettings = t_refPrintSettings;
		
#ifdef DEBUG
std::cout << "CPRINTPREVIEWWIDGET::SET_MEMBERS: refPageSetup " << refPageSetup << std::endl;
std::cout << "CPRINTPREVIEWWIDGET::SET_MEMBERS: refPrintSettings " << refPrintSettings << std::endl;
#endif // DEBUG	

	}	
	
void CPrintPreviewWidget::populate_iconview(void)
	{
	std::list<Glib::ustring>::iterator f_iterator = image_filelist.begin();
	std::list<Glib::ustring>::iterator f_end = image_filelist.end();
	
	Gtk::TreeModel::iterator iter;
	Gtk::TreeModel::Row row;
	
	while( f_iterator != f_end )
		{
		iter = refImageList->append();
		row = *iter;
		
		row[ ImageListColumns.filenames_column ] = Glib::path_get_basename( *f_iterator );
		
		row[ ImageListColumns.thumbnails_column ] =
			Gdk::Pixbuf::create_from_file( *f_iterator,
			 72,
			 72 );
		
		std::cout << "populating: " << *f_iterator << std::endl;
			
		f_iterator++;
		}
	}

void CPrintPreviewWidget::on_button_page_setup()
	{
	Gtk::TreeModel::Children children = refImageList->children();
	Gtk::TreeModel::iterator iter = children.begin();
	Gtk::TreeModel::iterator end = children.end();
	Gtk::TreeModel::Row row;

	std::cout << "ImageList contains: \n";
	while (	iter != end )
		{
		row = *iter;
		
		std::cout << row[ ImageListColumns.filenames_column ] << std::endl;
		iter++;
		}
	
	/* //temporary window to fool run_page_setup_dialog(...) :)
	Gtk::Window window;
	Glib::RefPtr<Gtk::PageSetup> tempPageSetup = Gtk::run_page_setup_dialog( 
		window, 
		refPageSetup, 
		refPrintSettings);
	
	refPageSetup->set_paper_size( tempPageSetup->get_paper_size() );
	refPageSetup->set_orientation( tempPageSetup->get_orientation() ); */
	}
