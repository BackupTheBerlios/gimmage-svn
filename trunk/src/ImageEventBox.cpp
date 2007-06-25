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

// gimmage : ImageEventBox.cpp

#include "ImageEventBox.h"
#include "defines.h"

#include "../config.h"

extern "C" {
#include <libintl.h>
}

ImageEventBox::ImageEventBox() :
	//EventBox(),
	Image()
	{
	// place the image in the image alignment and that in the eventbox
	set_visible_window(false);
	set_above_child(false);
	add(Image);
	loaded = false;
	}

ImageEventBox::~ImageEventBox()
	{
	#ifdef DEBUG
	std::cout << "IMAGEEVENTBOX: Cleaning up ImageEventBox.\n";
	#endif
	clear_image();
	}

void ImageEventBox::LoadImage(	const Glib::ustring & filename,
								double * scalefactor,
								int width,
								int height,
								Gdk::InterpType interp_type)
	{
#ifdef DEBUG
		std::cout << "LOADIMAGE: Loading: " << filename << std::endl;
#endif // DEBUG
	try
		{
		ImagePixbuf_Original = Gdk::Pixbuf::create_from_file(filename);
		loaded = true; // makes sure we don't operate on empty pixbufs
		// if the image to be loaded is being loaded oversized, load it at scalefactor 1.0
		if( width <= ImagePixbuf_Original->get_width() || height <= ImagePixbuf_Original->get_height() )
			ScaleImage(width, height, scalefactor, interp_type);
		else
			ScaleImage(1.0, interp_type), *scalefactor = 1.0;
		}
	catch(Gdk::PixbufError & error)
		{
		if( filename != GT( "No file to load." ) )
			{
			std::cerr << GT( "LOADIMAGE: There has been an error allocating " \
					"memory or understanding the image format. \n\t" ) << filename << std::endl;
			loaded = false;
			ImagePixbuf = Gdk::Pixbuf::create_from_file( iconpath + (Glib::ustring) "unreadable.png" );
			Image.set(ImagePixbuf);
			}
		else
			{
			clear_image();
			}
		}
	catch(Glib::FileError & error)
		{
		if( filename != GT( "No file to load." ) )
			{
			std::cerr << GT( "LOADIMAGE: There has been an error loading the file. \n\t" ) << filename << std::endl;
			loaded = false;
			ImagePixbuf = Gdk::Pixbuf::create_from_file( iconpath + (Glib::ustring) "unreadable.png" );
			Image.set(ImagePixbuf);
			}
		else
			{
			clear_image();
			}
		}
	}
	

void ImageEventBox::LoadImage(	const Glib::ustring & filename,
								double scalefactor,
								Gdk::InterpType interp_type)
	{
#ifdef DEBUG
		std::cout << "LOADIMAGE: Loading: " << filename << std::endl;
#endif // DEBUG	
	try
		{
		ImagePixbuf_Original = Gdk::Pixbuf::create_from_file(filename);
		loaded = true;	
		ScaleImage(scalefactor, interp_type);

		}
	catch(Gdk::PixbufError & error)
		{
		if( filename != GT( "No file to load." ) )
			{
			std::cerr << GT( "LOADIMAGE: There has been an error allocating " \
					"memory or understanding the image format. \n\t" ) << filename << std::endl;
			loaded = false;
			ImagePixbuf = Gdk::Pixbuf::create_from_file( iconpath + (Glib::ustring) "unreadable.png" );
			Image.set(ImagePixbuf);
			}
		else
			{
			clear_image();
			}
		}
	catch(Glib::FileError & error)
		{
		if( filename != GT( "No file to load." ) )
			{
			std::cerr << GT( "LOADIMAGE: There has been an error loading the file. \n\t" ) << filename << std::endl;
			loaded = false;
			ImagePixbuf = Gdk::Pixbuf::create_from_file( iconpath + (Glib::ustring) "unreadable.png" );
			Image.set(ImagePixbuf);
			}
		else
			{
			clear_image();
			}
		}
	}


// scales the image to the disired scalefactor
void ImageEventBox::ScaleImage( double scalefactor,
								Gdk::InterpType interp_type)
	{
	if(ImagePixbuf_Original != 0 && loaded == true)
		{
		int new_width   = (int)(scalefactor * (double)ImagePixbuf_Original->get_width());
		int new_height  = (int)(scalefactor * (double)ImagePixbuf_Original->get_height());
		
		try
			{
			ImagePixbuf =	ImagePixbuf_Original->scale_simple(
								new_width, new_height, interp_type);
			Image.set(ImagePixbuf);
			}
		catch(Gdk::PixbufError & error)
			{
			std::cerr << GT( "SCALEIMAGE(sclf): There has been a problem scaling the image. "\
						 "We've probably run out of memory.\n" );
			}
		}
	}	


// this one needs to obtain a reference to scalefactor in order to modify it
// according to the given width and height
void ImageEventBox::ScaleImage(	int width,
								int height,
								double * scalefactor,
								Gdk::InterpType interp_type)
	{
	if(ImagePixbuf_Original != 0 && loaded == true)
		{
		double ratioh = (double)height/(double)ImagePixbuf_Original->get_height();
		double ratiow = (double)width/(double)ImagePixbuf_Original->get_width();

		#ifdef DEBUG
		std::cout << "SCALEIMAGE: ratioh: " << ratioh << std::endl;
		std::cout << "SCALEIMAGE: ratiow: " << ratiow << std::endl;
		std::cout << "SCALEIMAGE: height: " << height << std::endl;
		std::cout << "SCALEIMAGE: width: " << width << std::endl;
		#endif
		
		*scalefactor = ratioh;

		if( (*scalefactor) * (double)ImagePixbuf_Original->get_width() > width )
			*scalefactor = ratiow;

		if( (*scalefactor) * (double)ImagePixbuf_Original->get_height() > height )
			*scalefactor = ratioh;

		// now that we have figured out the new scalefactor, let's just use our old function to scale it
		ScaleImage(*scalefactor,interp_type);
		}
	}	

void ImageEventBox::ScaleImage2(	int width,
								int height,
								double * scalefactor,
								Gdk::InterpType interp_type)
	{
	if(ImagePixbuf_Original != 0 && loaded == true)
		{
		if( width <= ImagePixbuf_Original->get_width() || height <= ImagePixbuf_Original->get_height() )
		    {
    		double ratioh = (double)height/(double)ImagePixbuf_Original->get_height();
    		double ratiow = (double)width/(double)ImagePixbuf_Original->get_width();

    		#ifdef DEBUG
    		std::cout << "SCALEIMAGE: ratioh: " << ratioh << std::endl;
    		std::cout << "SCALEIMAGE: ratiow: " << ratiow << std::endl;
    		std::cout << "SCALEIMAGE: height: " << height << std::endl;
    		std::cout << "SCALEIMAGE: width: " << width << std::endl;
    		#endif
    		
    		*scalefactor = ratioh;

    		if( (*scalefactor) * (double)ImagePixbuf_Original->get_width() > width )
    			*scalefactor = ratiow;

    		if( (*scalefactor) * (double)ImagePixbuf_Original->get_height() > height )
    			*scalefactor = ratioh;

    		// now that we have figured out the new scalefactor, let's just use our old function to scale it
    		ScaleImage(*scalefactor,interp_type);
    	    }
    	else
        	ScaleImage(1.0, interp_type), *scalefactor = 1.0;
    	        
		}
	}	

// note that we take care of the current angle of rotation with the "angle" variable,
// this helps us to keep the scrollbars adjusted to the right place depending on
// the rotation of the image
void ImageEventBox::RotateImage(Gdk::PixbufRotation rotateby)
	{
	if(ImagePixbuf_Original != 0 && loaded == true)
		{
		try
			{
			ImagePixbuf = ImagePixbuf->rotate_simple(rotateby);
			ImagePixbuf_Original = ImagePixbuf_Original->rotate_simple(rotateby);

			Image.set(ImagePixbuf);
			if		(angle == 0 && rotateby == Gdk::PIXBUF_ROTATE_COUNTERCLOCKWISE)
				angle = 270;
			else if	(angle == 270 && rotateby == Gdk::PIXBUF_ROTATE_CLOCKWISE)
				angle = 0;
			else
				{
				if( rotateby == Gdk::PIXBUF_ROTATE_CLOCKWISE )
					angle += 90;
				else if( rotateby == Gdk::PIXBUF_ROTATE_COUNTERCLOCKWISE )
					angle -= 90;
				} 
			}
		catch(Gdk::PixbufError & error)
			{
			std::cout << GT( "ROTATEIMAGE: There's been an error rotating the image. "\
						 "We've probably run out of memory.\n" );	
			}
		}
	}

void ImageEventBox::clear_image( void )
	{
	loaded = false;
	Image.clear();
	ImagePixbuf_Original.clear();
	ImagePixbuf.clear();
	}

// saves the current image to the current file, it does simple typechecking by extension...
// returns false if saving failed or illegal file type
// note: the filtering is done from AppWindow, which calls ImageManager.filter_save_filename
bool ImageEventBox::save_image(Glib::ustring filename, Glib::ustring filetype)
	{
	if ( filetype == "unsupported" )
		return false;

	// handle exceptions
	try
		{
		ImagePixbuf_Original->save(	filename, filetype );
		return true;
		}
	catch(Gdk::PixbufError & error)
		{
		return false;
		}
	catch(Glib::FileError & error)
		{
		return false;
		}
		
	}	
	
bool ImageEventBox::is_loaded(void)
	{
	return loaded;
	}

int ImageEventBox::get_image_height(void)
	{
	return ImagePixbuf->get_height();
	}

int ImageEventBox::get_image_width(void)
	{
	return ImagePixbuf->get_width();
	}

int ImageEventBox::get_image_angle(void)
	{
	return angle;
	}
