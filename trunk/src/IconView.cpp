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
	//ThreadLoadThumbs.signal_terminating_.connect( sigc::mem_fun( *this, &CIconView::on_terminating ) );
	//ThreadLoadThumbs.signal_terminated_.connect( sigc::mem_fun( *this, &CIconView::on_terminated ) );
	
	// when the iconview has loaded completely, select the current image and set the bool
	ThreadLoadThumbs.signal_done_.connect( sigc::mem_fun( *this, &CIconView::on_done ) );
	
	add( myIconView );
	set_policy( Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC );
	
	// set up the treemodel for the iconview and connect the view to the model
	refImageList = Gtk::ListStore::create( ImageListColumns );
	myIconView.set_model(refImageList);
	myIconView.set_text_column( ImageListColumns.filenames_column );
	myIconView.set_pixbuf_column( ImageListColumns.thumbnails_column );
	myIconView.set_selection_mode(Gtk::SELECTION_MULTIPLE);	
	
	terminating = false;
	loaded = false;
	}

void CIconView::on_new_thumb_ready()
	{
	if( !ThreadLoadThumbs.is_terminating() )
		{
		Glib::RefPtr<thumbnail> next_thumbnail = ThreadLoadThumbs.get_next_thumb();

		if( next_thumbnail->basename != "" )
			{
			Gtk::TreeModel::iterator iter;
			Gtk::TreeModel::Row row;
	
			iter = refImageList->append();
			row = *iter;
		
			row[ ImageListColumns.filenames_column ] = next_thumbnail->basename;
			row[ ImageListColumns.thumbnails_column ] = next_thumbnail->pixbuf;
			}
		}
	}
	
void CIconView::on_terminating()
	{
#ifdef DEBUG	
	std::cout << "ON_TERMINATING: whoops the loader is terminating!\n";
#endif // DEBUG	
	terminating = true;
	}

void CIconView::on_terminated()
	{
#ifdef DEBUG	
	std::cout << "ON_TERMINATING: whoops the loader has terminated!\n";
#endif // DEBUG	
	terminating = false;
	}	
	
void CIconView::on_done()
	{
#ifdef DEBUG	
	std::cout << "ON_DONE: iconview has loade all thumbnails!\n";
#endif // DEBUG	
	loaded = true;
	}			
	
bool CIconView::is_loaded()
	{
	return loaded;
	}	
	
void CIconView::load_new_thumbs( const std::list<Glib::ustring> &filelist )
	{
	if( image_filelist != filelist )
		{
		loaded = false;
		// clear the current iconview model	
		image_filelist.clear();
		image_filelist = filelist;
		refImageList->clear();
		
		ThreadLoadThumbs.load_new( image_filelist );
		}
	else
		{
		// TODO: select the right current thumbnail
		}
	}
	

