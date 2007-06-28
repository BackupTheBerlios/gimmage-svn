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

// gimmage: PagePreview.h

#include <iostream>

#include <gdkmm/pixbuf.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/eventbox.h>
#include <cairomm/cairomm.h>
#include <gdkmm/cursor.h>

struct pos { double x,y; };

class CPagePreview : public Gtk::DrawingArea
{
public:
	CPagePreview();
	~CPagePreview();
	void load( Glib::ustring, bool );
		
protected:
	Glib::RefPtr<Gdk::Pixbuf> ImagePixbuf;
	
	virtual bool on_expose_event(GdkEventExpose*);
	
	void PixbufLoad( Glib::ustring filename, int width, int height, bool );
	
private:
	// the dragging cursor
	Gdk::Cursor Hand;

	// event handlers for dragging the image around the page
	bool on_button1_pressed_motion(GdkEventMotion*);
	bool on_button_press_event(GdkEventButton*);
	bool on_button_release_event(GdkEventButton*);

	// page dimensions
	double page_width,page_height;
	
	// printer margins and their ratios with respect to page size
	double lmargin,rmargin,tmargin,bmargin;
	double lmargin_ratio,rmargin_ratio,
		   tmargin_ratio,bmargin_ratio;
	
	// the zoom ratio of the image
	double image_width_ratio,image_height_ratio;
	
	// a border we add for aestetic reasons
	double xborder,yborder;
	// the ratio of the border to the drawingarea size
	double borderratio;
	// the offset of the shadow
	double offset;
	// the position of the 
	double xpos,ypos;
	
	// page corners - margins
	pos uleft,lright;
	
	// helper variables for dragging the image around the page
	int dragoldx,dragoldy;
};
