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

CPrintPreview::CPrintPreview()
	{
	}
	
bool CPrintPreview::on_expose_event(GdkEventExpose *event)
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
		
		// draw the page and the image preview
		}
	}
	
CPrintPreview::~CPrintPreview() {}

