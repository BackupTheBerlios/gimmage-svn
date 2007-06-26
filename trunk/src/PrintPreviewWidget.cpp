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
#include <gdkmm/pixbuf.h>

CPrintPreviewWidget::CPrintPreviewWidget() :
	Page()
	{
	image_filename = "/home/bartek/Dokumenter/Biller/butzi1.jpg";
	
	PagePreviewFrame.add(Page);
	TopHBox.pack_start(PagePreviewFrame,Gtk::PACK_SHRINK);
	TopHBox.pack_start(ImageListFrame,Gtk::PACK_EXPAND_WIDGET);
	
	pack_start(TopHBox,Gtk::PACK_SHRINK);
	pack_start(PageSettingsFrame,Gtk::PACK_SHRINK);
	pack_start(HButtonBox,Gtk::PACK_SHRINK);
	
	Page.load( image_filename );
	
	show_all_children();
	}
	
CPrintPreviewWidget::~CPrintPreviewWidget() {}
	
void CPrintPreviewWidget::set_image_filename( Glib::ustring filename )
	{
	image_filename = filename;
	}




