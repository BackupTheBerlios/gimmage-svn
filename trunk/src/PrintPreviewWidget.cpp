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

CPrintPreviewWidget::CPrintPreviewWidget(const Glib::RefPtr<Gtk::PageSetup> _refPageSetup,
		const Glib::RefPtr<Gtk::PrintSettings> _refPrintSettings,
		Glib::ustring &filename,
		std::list<Glib::ustring> &filenames ) :
	Page()
	{
	std::cout << "CPRINTPREVIEWWIDGET: constructor\n";
	image_filenames = filenames;
	image_filename = filename;
	
	refImageList = Gtk::ListStore::create( ImageListColumns );
	
	refPageSetup = _refPageSetup;
	refPrintSettings = _refPrintSettings;
	
	PagePreviewFrame.add(Page);
	TopHBox.pack_start(PagePreviewFrame,Gtk::PACK_SHRINK);
	
	ImageIconView.set_model(refImageList);
	ImageIconView.set_text_column( ImageListColumns.filenames_column );
	ImageIconView.set_pixbuf_column( ImageListColumns.thumbnails_column );
	ImageIconView.set_selection_mode(Gtk::SELECTION_MULTIPLE);
	
	populate_iconview();

	ImageListScroller.add(ImageIconView);
	ImageListScroller.set_size_request(350,0);
	ImageListScroller.set_policy(Gtk::POLICY_AUTOMATIC,Gtk::POLICY_AUTOMATIC);
	ImageListFrame.add(ImageListScroller);
	TopHBox.pack_start(ImageListFrame,Gtk::PACK_EXPAND_WIDGET);
	
	pack_start(TopHBox,Gtk::PACK_SHRINK);
	pack_start(PageSettingsFrame,Gtk::PACK_SHRINK);
	pack_start(HButtonBox,Gtk::PACK_SHRINK);
	
	Page.load( image_filename );
	
	show_all_children();
	}
	
CPrintPreviewWidget::~CPrintPreviewWidget() 
	{
	std::cout << "CPRINTPREVIEWWIDGET: destructor\n";
	
	// clear the pixbuf refptrs
	Gtk::TreeModel::Children children = refImageList->children();
	Glib::RefPtr<Gdk::Pixbuf> icon;
	
	Gtk::TreeModel::Row row;
	for(Gtk::TreeModel::Children::iterator iter = children.begin();
    	iter != children.end(); ++iter)
		{
  		row = *iter;
 		icon = row[ ImageListColumns.thumbnails_column ];
 		icon.clear();
		}
	}

void CPrintPreviewWidget::populate_iconview()
	{
	std::list<Glib::ustring>::iterator f_iterator = image_filenames.begin();
	std::list<Glib::ustring>::iterator f_end = image_filenames.end();
	
	Gtk::TreeModel::iterator iter;
	Gtk::TreeModel::Row row;
	
	while( f_iterator != f_end )
		{
		iter = refImageList->append();
		row = *iter;
		
		row[ ImageListColumns.filenames_column ] = Glib::path_get_basename( *f_iterator );
		
		row[ ImageListColumns.thumbnails_column ] =
			Gdk::Pixbuf::create_from_file( *f_iterator,
			 200,
			 200 );
			
		f_iterator++;
		}
	
	}



