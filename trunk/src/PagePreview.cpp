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

// gimmage: PagePreview.cpp

#include "PagePreview.h"


/* The pagepreview needs to be fed with the dimensions of the page to be displayed
/* and has to be continuously updated as the interface changes values, it will therefore
/* need some accessor functions for the position and image_*_ratio and should probably
/* implement either a signal or a forced redraw in the accessor (although the signal
/* would be prettier! */
CPagePreview::CPagePreview() :
	Hand(Gdk::FLEUR)
	{
	int hrequest = 250;
	int wrequest = 180;
	
	image_width_ratio = 0.8;
	image_height_ratio = 0.8;	

	borderratio = 0.05;
	offset = 2.0;
	xborder = wrequest*borderratio;
	yborder = hrequest*borderratio;
	
	page_width = wrequest-2*xborder;
	page_height = hrequest-2*yborder;	

	lmargin_ratio = rmargin_ratio = bmargin_ratio = tmargin_ratio = 0;
	lmargin = rmargin = page_width * rmargin_ratio;
	bmargin = tmargin = page_height * tmargin_ratio;
	
	lmargin = bmargin = rmargin = tmargin = 10;

	uleft.x = xborder-offset+lmargin;
	uleft.y = yborder-offset+tmargin;
	lright.x = uleft.x+(page_width-rmargin-lmargin);
	lright.y = uleft.y+(page_height-bmargin-tmargin);
	xpos = uleft.x;
	ypos = uleft.y;	
	
	//std::cout << uleft.x << " " << lright.x << " " << page_width << " " << lright.x - uleft.x << " " << lmargin + rmargin << std::endl;
	
	set_size_request(wrequest,hrequest);
	
	// set up event mask
	set_events( 
		Gdk::EXPOSURE_MASK | 
		Gdk::BUTTON1_MOTION_MASK | 
		Gdk::BUTTON_PRESS_MASK | 
		Gdk::BUTTON_RELEASE_MASK );
		
	signal_motion_notify_event().connect(sigc::mem_fun(*this,&CPagePreview::on_button1_pressed_motion));
	signal_button_press_event().connect(sigc::mem_fun(*this,&CPagePreview::on_button_press_event));
	signal_button_release_event().connect(sigc::mem_fun(*this,&CPagePreview::on_button_release_event));	
	}
	
CPagePreview::~CPagePreview()
	{
	ImagePixbuf.clear();
	}

void CPagePreview::load( Glib::ustring filename, bool aspect )
	{
	PixbufLoad( 
		filename,
		(int)((lright.x-uleft.x)*image_width_ratio),
		(int)((lright.y-uleft.y)*image_height_ratio),
		aspect );
	if(ImagePixbuf)
		{
		xpos = (uleft.x+lright.x - ImagePixbuf->get_width())/2;
		ypos = (uleft.y+lright.y - ImagePixbuf->get_height())/2;
		}
	}
	
// do the actual file loading	
void CPagePreview::PixbufLoad( Glib::ustring filename, int width, int height, bool aspect )
	{
	try
		{
		ImagePixbuf.clear();
		ImagePixbuf = Gdk::Pixbuf::create_from_file( 
			filename,
			width,
			height,
			aspect );
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

bool CPagePreview::on_expose_event(GdkEventExpose *event)
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
		
		// draw a neat shadow
		cr->set_source_rgba(0.0,0.0,0.0,0.4);
		cr->begin_new_path();
		cr->move_to( xborder+offset,yborder+offset );
		cr->line_to( width-xborder+offset,yborder+offset );
		cr->line_to( width-xborder+offset,height-yborder+offset );
		cr->line_to( xborder+offset,height-yborder+offset );
		cr->close_path();
		cr->fill();

		// draw the page outline
		cr->set_source_rgb(0.0,0.0,0.0); // black
		cr->set_line_width( 2.0 );
		cr->begin_new_path();
		cr->move_to( xborder-offset,yborder-offset );
		cr->line_to( width-xborder-offset,yborder-offset );
		cr->line_to( width-xborder-offset,height-yborder-offset );
		cr->line_to( xborder-offset,height-yborder-offset );
		cr->close_path();
		cr->stroke_preserve();
		
		// fill the page with white
		cr->save();
		cr->set_source_rgb(1.0,1.0,1.0); // white
		cr->fill_preserve();
		cr->restore();
		
		// draw the page margin
		cr->set_source_rgb( 0.8,0.8,0.8 ); // light grey
		cr->set_line_width( 1.0 );
		cr->begin_new_path();
		cr->move_to( uleft.x, uleft.y );
		cr->line_to( lright.x, uleft.y );
		cr->line_to( lright.x, lright.y );
		cr->line_to( uleft.x, lright.y );
		cr->close_path();
		cr->stroke();

		// and the image preview
		ImagePixbuf->render_to_drawable( get_window(),
									get_style()->get_black_gc(),
									0,
									0,
									xpos,
									ypos,
									ImagePixbuf->get_width(), //image->get_width(),
									ImagePixbuf->get_height(), //image->get_height(),
									Gdk::RGB_DITHER_NONE,0,0 ); // */
									
		return true;
		}
	}

// this drags the image around
// big thanks to Ramax Lo for fixing this!!
bool CPagePreview::on_button1_pressed_motion(GdkEventMotion *event)
	{
	// get the cairo context amd allocation
	Gtk::Allocation allocation = get_allocation();
	const int width = allocation.get_width();
	const int height = allocation.get_height();		
	
	static int x,y;
	get_pointer(x, y);
	
#ifdef DEBUG
std::cout << "PAGE_PREVIEW::PRESSED_MOTION: x\ty :  " << x << '\t' << y << std::endl;
#endif // DEBUG		
	
	if(ImagePixbuf->get_width() < page_width-lmargin-rmargin )
		{
		if( (xpos + (x - dragoldx)) >= uleft.x  &&
			(xpos + (x - dragoldx)) <=
			( lright.x - ImagePixbuf->get_width() ) )
			{
			xpos = xpos + (x - dragoldx);
			dragoldx = x;
			}
		//snap to end
		else if( (xpos + (x - dragoldx)) < uleft.x	)
			{
			xpos = uleft.x;
			dragoldx = x;
			}
		else if( (xpos + (x - dragoldx)) >
			( lright.x - ImagePixbuf->get_width() ) )
			{
			xpos = lright.x - ImagePixbuf->get_width();
			dragoldx = x;
			}
		}
	
	if(ImagePixbuf->get_height() < page_height-lmargin-rmargin )
		{
		if( (ypos + (y - dragoldy)) >= uleft.y  &&
			(ypos + (y - dragoldy)) <=
			( lright.y - ImagePixbuf->get_height() ) )
			{
			ypos = ypos + (y - dragoldy);
			dragoldy = y;
			}
		//snap to end
		else if( (ypos + (y - dragoldy)) < uleft.y	)
			{
			ypos = uleft.y;
			dragoldy = y;
			}
		else if( (ypos + (y - dragoldy)) >
			( lright.y - ImagePixbuf->get_height() ) )
			{
			ypos = lright.y - ImagePixbuf->get_height();
			dragoldy = y;
			}
		}	
	
	// force redraw	
	Glib::RefPtr<Gdk::Window> window = get_window();
	if (window)
		{
		Gdk::Rectangle r(0, 0, get_allocation().get_width(),
				get_allocation().get_height());
		window->invalidate_rect(r, false);
		}	
	return true;	
	}		

bool CPagePreview::on_button_press_event(GdkEventButton *event)
 	{
#ifdef DEBUG
std::cout << "ON_BUTTON_PRESS_EVENT: pressed button " << event->button << std::endl;
#endif 	
 		switch( event->button )
 		{
 		case 1:
			int x, y;
			get_window()->set_cursor(Hand);	
			get_pointer(x, y);
			dragoldx = x;
			dragoldy = y;
			return false;
			break; //just to be sure

		//for other buttons, set dragoldx/dragoldy to -999999
		//so that the cursor doesn't change and we don't drag scroll
		//(on_button1_pressed_motion)
		default:
			//dragoldx = dragoldy = -999999;
			return false;
			break;
		}
 	}
 	
 bool CPagePreview::on_button_release_event(GdkEventButton *event)
	{
#ifdef DEBUG
std::cout << "ON_BUTTON_RELEASE_EVENT: released button " << event->button << std::endl;
#endif
	switch( event->button )
		{
		case 1:
			// set_cursor() sets the cursor to the parent window's cursor, which should
			// properly handle right- and left-pointing cursors
			get_window()->set_cursor();
			return false;
			break;
		
		default:
			return false;
			break;
		}
	}		
