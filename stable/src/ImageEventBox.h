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

// gimmage : ImageEventBox.h

#include <string>
#include <iostream>
#include <gtkmm/image.h>
#include <gdkmm/pixbuf.h>
#include <gtkmm/eventbox.h>

extern Glib::ustring iconpath;

// this event box and image object will hold the image and provide a context
// for user interaction (that's why it inherits EventBox)

class ImageEventBox : public Gtk::EventBox
	{
	public:
		ImageEventBox();
		virtual ~ImageEventBox();
	
		// note: scalefactor is passed to the scaling function as a pointer from the main
		// application so that if ScaleImage is called with a fixed width and height
		// it will be possible to actually change the scalefactor in the main app after scaling
		
		// function which loads an image from file, references it to ImagePixbuf and puts it into Image
		// at the same time it changes the scalefactor variable to reflect the width and height that
		// it was told to load at
		void LoadImage(	const Glib::ustring & filename,
						double * scalefactor,
						int width = 640,
						int height = 480,
						Gdk::InterpType interp_type = Gdk::INTERP_BILINEAR);
		// as above but it's given scalefactor directly 
		void LoadImage(	const Glib::ustring & filename,
						double scalefactor,
						Gdk::InterpType interp_type = Gdk::INTERP_BILINEAR);
	
		
		void ScaleImage(int width,
						int height,
						double * scalefactor,
						Gdk::InterpType interp_type = Gdk::INTERP_BILINEAR);
						
		// a special scaler which checks whether the image will be bigger than its original size
		// if that is the case, it is loaded at original size.				
		void ScaleImage2(int width,
						int height,
						double * scalefactor,
						Gdk::InterpType interp_type = Gdk::INTERP_BILINEAR);
						
		void ScaleImage(double scalefactor,
						Gdk::InterpType interp_type = Gdk::INTERP_BILINEAR);
	
		void RotateImage(Gdk::PixbufRotation rotateby);
	
		int get_image_height(void);
		int get_image_width(void);
		int get_image_angle(void);
		bool save_image(Glib::ustring filename, Glib::ustring filetype); // returns bool to indicate whether saving was successful
		bool is_loaded(void);


	
	protected:
		Gtk::Image Image;

		int angle; // keeps track of the angle of rotation of the image in a clockwise manner
		bool loaded; // keeps track of whether we actually have an image in the window
		bool modified; // keeps track whether a rotation has been done enablind saving the new file

		// reference pointer to the pixbuf which will be loaded on request
		Glib::RefPtr<Gdk::Pixbuf> ImagePixbuf;
		Glib::RefPtr<Gdk::Pixbuf> ImagePixbuf_Original;

		void clear_image( void ); // clears all image buffers
	};
