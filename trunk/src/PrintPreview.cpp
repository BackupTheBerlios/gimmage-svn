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

CPrintPreview::CPrintPreview() :
	Page(), // */
	PageContext( Page.get_window()->create_cairo_context() )
	{
	//PageContext = Page.get_window()->create_cairo_context();
	//PageContext->set_source_rgb(1.0,1.0,1.0);
	//PageContext->set_line_width(1.0);
	
	add(Page);
	show_all_children();
	}
	
CPrintPreview::~CPrintPreview() {}

