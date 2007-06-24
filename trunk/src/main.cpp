/* This file is part of gimmage

Copyright 2005 Bartek Kostrzewa

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

// gimmage - the image viewer - main.cpp

#include <gtkmm/main.h>
#include "AppWindow.h"

#include "defines.h"
#include "../config.h"

extern "C" {
#include <sys/stat.h>
#include <libintl.h>
}

// this variable is global... yes it sucks, but it works well, and it's the only one
// it holds information where AppWindow can find the icons for the custom buttons
Glib::ustring iconpath;
Glib::ustring app_icon_path;


// looks for the possible path to gimmage's icons
void find_iconpath( Glib::ustring progname )
	{
	struct stat filemode;
	iconpath = Glib::find_program_in_path( progname );
	app_icon_path = iconpath;

	if( iconpath != "" && iconpath.find("bin") != std::string::npos )
		{	
		iconpath.erase( iconpath.find("bin") );
		iconpath += "share/gimmage/pixmaps/";
		app_icon_path += "pixmaps/gimmage.png";
		}

	if( iconpath == "" )
		iconpath = "/usr/share/gimmage/pixmaps/"; // try to prevent errors from PATH lacking the executable
		
	if( app_icon_path == "" )	
		app_icon_path = "/usr/share/pixmaps/gimmage.png";
		
	if( stat( iconpath.c_str(), &filemode) != 0 )
		iconpath = "/usr/local/share/gimmage/pixmaps/";
		
	if( stat( app_icon_path.c_str(), &filemode) != 0 )
		app_icon_path = "/usr/local/share/pixmaps/gimmage.png";
	
	// last resort to current working directory for pixmaps (when running
	// gimmage from the extraction directory to try it out for instance
	if( stat( iconpath.c_str(), &filemode) != 0 ) 
		iconpath = Glib::get_current_dir() + (Glib::ustring)"/pixmaps/";
	
	if( stat( app_icon_path.c_str(), &filemode) != 0 ) 
		app_icon_path = Glib::get_current_dir() + (Glib::ustring)"/pixmaps/icon/gimmage.png";
		
	if( stat( iconpath.c_str(), &filemode) != 0 ) 
		iconpath = Glib::get_current_dir() + (Glib::ustring)"/../pixmaps/";
		
	if( stat( app_icon_path.c_str(), &filemode) != 0 ) 
		app_icon_path = Glib::get_current_dir() + (Glib::ustring)"/../pixmaps/icon/gimmage.png";

	#ifdef DEBUG
	std::cout << "FIND_ICONPATH: " << iconpath << std::endl;
	std::cout << "APP_ICON_PATH: " << app_icon_path << std::endl;
	#endif	

	if( stat( iconpath.c_str(), &filemode) != 0 )
		std::cout << GT("Gimmage pixmaps could not be found! This might cause a segfault.\n");

	if( stat( app_icon_path.c_str(), &filemode) != 0 )
		std::cout << GT("Gimmage icon could not be found! This WILL cause a segfault.\n");

	}
	

int main(int argc, char *argv[])
	{
	
#ifdef DEBUG
	std::cout << "LOCALE: " << LC_MESSAGES << " " << PACKAGE << " " << LOCALEDIR << std::endl;
#endif // DEBUG 	

	//setlocale (LC_CTYPE, "");
	//setlocale (LC_MESSAGES, "");
	//setlocale (LC_ALL, "");
  	bindtextdomain (PACKAGE, LOCALEDIR);
  	bind_textdomain_codeset(PACKAGE, "UTF-8");
  	textdomain(PACKAGE);

	// depending on where we are, we must try to locate our pixmaps! I know this won't
	// work very well if the executable is not the path of the user
	// IE: root launching absolute /usr/local/bin/gimmage but not having /usr/local in his path
	// any solutions?

	find_iconpath( "gimmage" );

#ifdef DEBUG
	std::cout << "Icons in: " << iconpath << std::endl;
#endif // DEBUG

	Glib::init();
	Glib::set_prgname("gimmage");
	Glib::set_application_name("Image Viewer");
		
	// set up command line help
	Glib::OptionContext options( "[dirname] ... [filename]"
				"\n\n  gimmage will scan directories given up to one level deep\n"
				"  and add any filenames given to its list of images to be displayed\n\n"
				"    example: gimmage file1 file2 dir1 dir2\n\n"
				"  will display all images in dir1 and dir2 and file1 and file2\n" );

	Gtk::Main kit(argc,argv,options);

	AppWindow gimmage(argc, argv);
	gimmage.set_icon_from_file( app_icon_path );	
	
	Gtk::Main::run(gimmage);

	return 0;
	}


