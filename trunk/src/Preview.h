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

// gimmage: Preview.h

#include <iostream>

#include <gdkmm/pixbuf.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/eventbox.h>
#include <cairomm/cairomm.h>

#include "defines.h"
#include "../config.h"

class CPreview : public Gtk::DrawingArea
{
public:
	CPreview();
	~CPreview();
	void load( Glib::ustring, bool );
		
protected:
	Glib::RefPtr<Gdk::Pixbuf> ImagePixbuf;
	
	virtual bool on_expose_event(GdkEventExpose*);
	
	void PixbufLoad( Glib::ustring,int,int );

	
private:
};
