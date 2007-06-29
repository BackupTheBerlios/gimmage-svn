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

// gimmage: FileManager.h

#include <iostream>
#include <list>
// #include <string>
#include <glibmm.h>


extern "C" {
#include <dirent.h>
#include <sys/types.h>
#include <magic.h>
#include <sys/stat.h>
}

// FileManager:
// the class stores a list of filenames given at the command line of the main program or
// passed with the parameters int argc and char **argv which should resemble the command line
// and can navigate through them sequently returning strings by the functions get_next_file,
// get_previous_file and get_current_file and most importantly get_first_file

class FileManager
	{
	public:
		FileManager();
		FileManager(int argc, char **argv);
		virtual ~FileManager();
		
		// methods to receive filenames, self-descriptive
		Glib::ustring get_first_file();
		Glib::ustring get_current_file();
		
		// increment/decrement the iterator and return the filename
		Glib::ustring get_next_file();
		Glib::ustring get_previous_file();
		
		// return a copy of the filenames list
		const std::list<Glib::ustring>& get_file_list();

		// this returns the current working directory (where the file that is currently
		// being displayed resides
		Glib::ustring get_current_dir();

		// this returns true if multiple files have been openened, it triggers
		// whether the arrows are active or not
		bool have_multiple_files(void);
		
		// opens a new set of images
		void OpenNewSet(int,char**);
		void OpenNewSet( std::list<Glib::ustring>& );

		// this checks (using libmagic) whether the file is supported
		bool filter_filename(Glib::ustring filename);

		// this returns whether the filename that we are trying to save
		// is supported for saving. note: this is called using the filenames
		// of the ORIGINAL, not the file that is about to be saved!
		Glib::ustring filter_save_filename(Glib::ustring);

	protected:
	// std::cout's the list of files
		void showfiles();
		

		// method to open list of filenames
		bool OpenFiles( int argc, char **argv );

		Glib::ustring result;

	private:
		// keeps track whether we have a list of files yet
		bool initialised;

		// keep track of the last operation
		bool last_op_next;
		bool last_op_previous;

		// our list of files and its iterator
		std::list <Glib::ustring> filenames;
		std::list<Glib::ustring>::iterator file_iterator;
		int numfiles; // store how many files we have loaded
		
		// when we open only one file, we check the cwd for more, we use this to prevent fatal loops!
		bool cwd_checked; 

		magic_t cookie;

		// internal variables to "determine" filetype and directory
		// structure
		struct stat filemode;
		DIR *currdir;
		dirent *dirinfo;
	};
