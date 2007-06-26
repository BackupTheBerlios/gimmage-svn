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

// gimmage: Preview.cpp

#include "Preview.h"

CPreview::CPreview()
	{
	// no need to waste space if we are not loaded
	if( !ImagePixbuf ) 
		set_size_request(0,0);
	}
	
CPreview::~CPreview() 
	{
	ImagePixbuf.clear();
	}


// cause a new image to be loaded
void CPreview::load( Glib::ustring filename, bool loadable )
	{
	if( !loadable )
		{
		set_size_request(0,0);
		ImagePixbuf.clear();
		
		// force a redraw
		Glib::RefPtr<Gdk::Window> window = get_window();
		if (window)
			{
			Gdk::Rectangle r(0, 0, get_allocation().get_width(),
					get_allocation().get_height());
			window->invalidate_rect(r, false);
			}
		return;
		}

	else
		{
		set_size_request( 100,100 );	
		int width;
		int height;
		

		get_size_request( width, height );
		
		// we don't have any padding so let's add a little something
		width *= 0.98;
		height *= 0.98;	
				
		PixbufLoad( filename, width, height );
			

		// force a redraw
		Glib::RefPtr<Gdk::Window> window = get_window();
		if (window)
			{
			Gdk::Rectangle r(0, 0, get_allocation().get_width(),
					get_allocation().get_height());
			window->invalidate_rect(r, false);
			}
		}
	}


// do the actual file loading	
void CPreview::PixbufLoad( Glib::ustring filename, int width, int height )
	{
	try
		{
		ImagePixbuf.clear();
		ImagePixbuf = Gdk::Pixbuf::create_from_file( 
			filename,
			width,
			height );
		}
	catch(Gdk::PixbufError & error)
		{
			std::cerr << "PIXBUFLOAD: PixbufError\n";
		}
	catch(Glib::FileError & error)
		{
			std::cerr << "PIXBUFLOAD: FileError\n";
		}
	}

bool CPreview::on_expose_event(GdkEventExpose *event)
	{			
	Glib::RefPtr<Gdk::Window> window = get_window();
	if(window)
		{
		// get the cairo context amd allocation
		Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
		Gtk::Allocation allocation = get_allocation();
		const int width = allocation.get_width();
		const int height = allocation.get_height();
		
		// coordinates for the center of the window
		int xc, yc;
		xc = width / 2;
		yc = height / 2;

		
		// clip to the area indicated by the expose event so that we only redraw
		// the portion of the window that needs to be redrawn
		cr->rectangle(event->area.x, event->area.y,
				event->area.width, event->area.height);
		cr->clip();
		
		double offset = 4.0;
		
		
		//drawing a shadow is mighty cool, but it wastes screen real estate
		/* draw a neat shadow
		cr->set_source_rgba(0.0,0.0,0.0,0.6);
		cr->begin_new_path();
		cr->move_to( (width-ImagePixbuf->get_width())/2+offset,
			(height-ImagePixbuf->get_height())/2+offset );
		
		cr->line_to( (width+ImagePixbuf->get_width())/2+offset,
			(height-ImagePixbuf->get_height())/2+offset );
		
		cr->line_to( (width+ImagePixbuf->get_width())/2+offset,
			(height+ImagePixbuf->get_height())/2+offset );
		
		cr->line_to( (width-ImagePixbuf->get_width())/2+offset,
			(height+ImagePixbuf->get_height())/2+offset );
			
		cr->close_path();
		cr->fill(); // */

		// and the image preview, but only if the image is loaded
		if( ImagePixbuf )
			{
			ImagePixbuf->render_to_drawable( get_window(),
										get_style()->get_black_gc(),
										0,
										0,
										(width-ImagePixbuf->get_width())/2,
										(height-ImagePixbuf->get_height())/2,
										ImagePixbuf->get_width(), //image->get_width(),
										ImagePixbuf->get_height(), //image->get_height(),
										Gdk::RGB_DITHER_NONE,0,0 ); // */
			}
									
		return true;
		}
	}
