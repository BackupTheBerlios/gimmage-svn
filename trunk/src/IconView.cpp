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

// gimmage: IconView.cpp

#include "IconView.h"

CIconView::CIconView () :
	ThreadLoadThumbs()
	{
	// connect to the cross-thread signal which tells us to fetch a new thumbnail
	ThreadLoadThumbs.signal_new_thumb_ready_.connect( sigc::mem_fun( *this, &CIconView::on_new_thumb_ready ) );
	ThreadLoadThumbs.signal_terminating_.connect( sigc::mem_fun( *this, &CIconView::on_terminating ) );
	ThreadLoadThumbs.signal_terminated_.connect( sigc::mem_fun( *this, &CIconView::on_terminated ) );
	
	add( myIconView );
	set_policy( Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC );
	
	// set up the treemodel for the iconview and connect the view to the model
	refImageList = Gtk::ListStore::create( ImageListColumns );
	myIconView.set_model(refImageList);
	myIconView.set_text_column( ImageListColumns.filenames_column );
	myIconView.set_pixbuf_column( ImageListColumns.thumbnails_column );
	myIconView.set_selection_mode(Gtk::SELECTION_MULTIPLE);	
	
	f_iterator = image_filelist.begin();
	f_end = image_filelist.end();
	terminating = false;
	}

void CIconView::on_new_thumb_ready()
	{
	if( !ThreadLoadThumbs.is_terminating() )
		{
		Glib::RefPtr<thumbnail> next_thumbnail = ThreadLoadThumbs.get_next_thumb();

		Gtk::TreeModel::iterator iter;
		Gtk::TreeModel::Row row;
	
		iter = refImageList->append();
		row = *iter;
		
		row[ ImageListColumns.filenames_column ] = next_thumbnail->basename;
		row[ ImageListColumns.thumbnails_column ] = next_thumbnail->pixbuf;
		}
	}
	
void CIconView::on_terminating()
	{
	std::cout << "ON_TERMINATING: whoops the loader is terminating!\n";
	terminating = true;
	}

void CIconView::on_terminated()
	{
	std::cout << "ON_TERMINATING: whoops the loader has terminated!\n";
	terminating = false;
	}		
	
void CIconView::load_new_thumbs( const std::list<Glib::ustring> &filelist )
	{
	// clear the current iconview model	
	image_filelist.clear();
	image_filelist = filelist;
	refImageList->clear();
	
	ThreadLoadThumbs.load_new( image_filelist );
	}
	

