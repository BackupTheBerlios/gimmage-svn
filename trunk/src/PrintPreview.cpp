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

// gimmage: PrintPreview.cpp

#include "PrintPreview.h"
#include <gdkmm/pixbuf.h>

CPrintPreview::CPrintPreview() :
	Page(),
	PageEventBox()
	{
	page_width = 200;
	page_height = 300;
	image_width_ratio = 0.5;
	image_height_ratio = 0.5;
	
	image_filename = "/home/bartek/Dokumenter/Biller/butzi1.jpg";
	PageEventBox.add(Page);
	pack_start(PageEventBox);
	}
	
CPrintPreview::~CPrintPreview() {}
	
void CPrintPreview::load( Glib::ustring filename )
	{
	ImagePixbuf = Gdk::Pixbuf::create_from_file( 
		"/home/bartek/Dokumenter/Biller/butzi1.jpg",
		(int)(page_width*image_width_ratio),
		(int)(page_height*image_width_ratio) );
	if(ImagePixbuf)
		std::cout << "Yes, there's something \n";
	else
		std::cout << "ney, empty \n";	
	}

void CPrintPreview::set_image_filename( Glib::ustring filename )
	{
	image_filename = filename;
	}
	
bool CPrintPreview::on_expose_event(GdkEventExpose *event)
	{
	//static bool loaded = false;
	//if( !loaded )
	//	{
	
	//	}

			
	Glib::RefPtr<Gdk::Window> window = Page.get_window();
	if(window)
		{
		// get the cairo context amd allocation
		Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
		Gtk::Allocation allocation = Page.get_allocation();
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
		
		double border = 0.05;
		double offset = 4.0;
		
		// draw a neat shadow
		cr->begin_new_path();
		cr->move_to( width*border+offset,height*border+offset );
		cr->line_to( width*(1.0-border)+offset,height*border+offset );
		cr->line_to( width*(1.0-border)+offset,height*(1.0-border)+offset );
		cr->line_to( width*border+offset,height*(1.0-border)+offset );
		cr->close_path();
		cr->fill();

		// draw the page outline
		cr->set_source_rgb(0.0,0.0,0.0); // black
		cr->set_line_width( 1.0 );
		cr->begin_new_sub_path();
		cr->move_to( width*border,height*border );
		cr->line_to( width*(1.0-border),height*border );
		cr->line_to( width*(1.0-border),height*(1.0-border) );
		cr->line_to( width*border,height*(1.0-border) );
		cr->close_path();
		cr->stroke_preserve();
		
		// fill the page with white
		cr->save();
		cr->set_source_rgb(1.0,1.0,1.0); // white
		cr->fill_preserve();
		cr->restore();

		// and the image preview
		ImagePixbuf->render_to_drawable( Page.get_window(),
									Page.get_style()->get_black_gc(),
									0,
									0,
									(width-ImagePixbuf->get_width())/2,
									(height-ImagePixbuf->get_height())/2,
									ImagePixbuf->get_width(), //image->get_width(),
									ImagePixbuf->get_height(), //image->get_height(),
									Gdk::RGB_DITHER_NONE,0,0 ); // */
									
		return true;
		}
	}
	


