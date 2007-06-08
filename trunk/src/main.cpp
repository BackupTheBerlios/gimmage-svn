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


// looks for the possible path to gimmage's icons
void find_iconpath( Glib::ustring progname )
	{
	struct stat filemode;
	iconpath = Glib::find_program_in_path( progname );

	if( iconpath != "" && iconpath.find("bin") != std::string::npos )
		{	
		iconpath.erase( iconpath.find("bin") );
		iconpath += "share/gimmage/pixmaps/";
		}

	#ifdef DEBUG
	std::cout << "FIND_ICONPATH1: trying: " << iconpath << std::endl;
	#endif
	


	if( iconpath == "" ) 
		iconpath = "/usr/share/gimmage/pixmaps/"; // try to prevent errors from PATH lacking the executable
	else
		return;

	#ifdef DEBUG
	std::cout << "FIND_ICONPATH2: trying: " << iconpath << std::endl;
	#endif
		
	if( stat( iconpath.c_str(), &filemode) != 0 ) 
		iconpath = "/usr/local/share/gimmage/pixmaps/";
		
	
	// last resort to current working directory for pixmaps (when running
	// gimmage from the extraction directory to try it out for instance
	if( stat( iconpath.c_str(), &filemode) != 0 ) 
		iconpath = "pixmaps/";	

	#ifdef DEBUG
	std::cout << "FIND_ICONPATH3: trying: " << iconpath << std::endl;
	#endif	

	if( stat( iconpath.c_str(), &filemode) != 0 )
		std::cout << GT("Gimmage pixmaps could not be found! This will cause a segfault.\n");
		
		
	}
	

int main(int argc, char *argv[])
	{
	// depending on where we are, we must try to local our pixmaps! I know this won't
	// work very well if the executable is not the path of the user
	// IE: root launching absolute /usr/local/bin/gimmage but not having /usr/local in his path
	// any solutions?

	setlocale (LC_ALL, "");
  	bindtextdomain (PACKAGE, LOCALEDIR);
  	textdomain (PACKAGE);

#ifdef DEBUG
	find_iconpath( "gimmage-debug" );
#else
	find_iconpath( "gimmage" );
#endif

#ifdef DEBUG
	std::cout << "Icons in: " << iconpath << std::endl;
#endif

	Gtk::Main kit(argc,argv);

	AppWindow gimmage(argc, argv);
	Gtk::Main::run(gimmage);

	return 0;
	}


