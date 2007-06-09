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

// gimmage: FileManager.cpp
#include "FileManager.h"
#include "defines.h"

#include "../config.h"

extern "C" {
#include <unistd.h>
}

FileManager::FileManager() {}

FileManager::FileManager(int argc, char **argv)
	{
	// we have not initialised yet
	initialised = false;
	// no operations carried out yet
	last_op_next = false;
	last_op_previous = false;


	// initialise magic database
	if( (cookie = magic_open(MAGIC_SYMLINK) ) == NULL)
		std::cout << GT( "FILEMANAGER: ERROR during magic open" ) << std::endl;
	if( magic_load(cookie,NULL) != 0 )
		std::cout << GT( "FILEMANAGER: ERROR during magic load" ) << std::endl;

	numfiles=0;
	cwd_checked=0;
	/* let's call OpenFiles() to scan through the directories and set the initialised variable
	   to tell everyone we're up and running */
	if( OpenFiles(argc,argv) )
		initialised = true; 
	}

FileManager::~FileManager()
	{
	#ifdef DEBUG
	std::cout << "FILEMANAGER: Cleaning up ImageManager.\n";
	#endif
	
	magic_close(cookie);
	filenames.clear();
	}
	
bool FileManager::OpenFiles(int argc, char **argv)
	{
	#ifdef DEBUG
	std::cout << "OPENFILES: OpenFiles called \n";
	#endif // DEBUG

	
	//	if we have initialised already we're being called from the file selector and will
	//	accept new files now, let's set initialised and cwd_checked to 'false' and clear our file list
	if( initialised == true )
	{
	filenames.erase(filenames.begin(), filenames.end());
	numfiles = 0;
	cwd_checked = false;
	initialised = false;
	}
	#ifdef DEBUG
	std::cout << "OPENFILES: OpenFiles called \n";
	std::cout << "OPENFILES: get_current_dir_name(): " << get_current_dir_name() << std::endl;
	#endif // DEBUG
		
	if( argc > 1 )
		{
		for(int i = 1; i < argc; i++)
			{
			if( stat(argv[i], &filemode) != 0 )
				{
				std::cout << GT( "OPENFILES: Stat Error! Level 1 " ) << argv[i] << std::endl;
				}
			else
				{
				if(filemode.st_mode & S_IFREG )  // if the file is a regular file
				 {
				 if( filter_filename(argv[i]) ) // and seems to be a supported format
					{
					#ifdef DEBUG
					std::cout << "FILEMANAGER: curent argv: " << argv[i] << std::endl;
					std::cout << "FILEMANAGER: dirname from argv: " << Glib::path_get_dirname( argv[i] ) << std::endl;
					std::cout << "FILEMANAGER: curent dir: " << get_current_dir_name() << std::endl;
					#endif
					if( Glib::path_is_absolute( argv[i] ) )
					    filenames.insert( filenames.end(), argv[i] ); // insert into list
					else
					    filenames.insert( filenames.end(), (Glib::ustring)get_current_dir_name() + "/" + (Glib::ustring)argv[i] );
					numfiles++;
					}
				 }
				else if(filemode.st_mode & S_IFDIR) 			// if the file is a directory
					{
					if((currdir = opendir(argv[i])) != NULL) 	// open it
						{
						while( (dirinfo = readdir(currdir)) != NULL ) // run through it
							{
							// don't forget, we're doing some string to char* conversions here 
							// here to avoid memory errors.. Glib::ustring is nice!
							Glib::ustring currfile(argv[i]); 				// store the directory name
							currfile += '/'; 							// add a directory dash
							currfile += dirinfo->d_name; 				// add the filename
							if(stat(currfile.c_str(), &filemode) != 0)  // check for filetype
								{
								std::cout << GT( "OPENFILES: Stat Error! Level 2 " ) << currfile << std::endl;
								}
							else
								{
								if( filemode.st_mode & S_IFREG && filter_filename(currfile) ) // as above
									{
									if( Glib::path_is_absolute( currfile ) )
					                    filenames.insert( filenames.end(), currfile ); // insert into list
					                else
									    filenames.insert( filenames.end(), (Glib::ustring)get_current_dir_name() + "/" + (Glib::ustring)currfile );
									numfiles++;
									}
								}
							}
						closedir(currdir);
						}
					}
				}
			}
		filenames.sort();
		file_iterator = filenames.begin();
		}

	// when only one file was openened, we'll scan through its directory
	// and give the user control over the files
	if ( numfiles == 1 && cwd_checked == false )
		{
			cwd_checked = true;

			Glib::ustring tempfilename = *file_iterator;
			
			// prepare the file's directory name in such a way that we can pass
			// it to OpenFiles() which operates on argc/argv	
			char *dirname = new char[ (Glib::path_get_dirname( *file_iterator )).length() + 1  ];
			strcpy( dirname, (Glib::path_get_dirname( *file_iterator )).c_str() );

			char *arguments[] = { "empty" , dirname };

			// we erase the list of filenames as otherwise we would have one file twice
			filenames.erase(filenames.begin(),filenames.end());
			numfiles = 0;

			// now let's open the file's directory and check its contents
			OpenFiles( 2, arguments );

			// now we have a new set of files in the list, we have to find the file that
			// the user actually wanted to open!

			std::list<Glib::ustring>::iterator end = filenames.end();
			while( file_iterator != end )
				{
				#ifdef DEBUG
				std::cout << "OPENFILES: Looking for " << tempfilename << " in " << (*file_iterator) << std::endl;
				#endif
				if( (*file_iterator).find( tempfilename ) != Glib::ustring::npos )
					break; // break out of the while loop if we find the filename
				file_iterator++;
				}
			// when we have reached the end while looking, let's just load the first image!
			// as that means that either we didn't recognize the filename in our search above
			// or there was only one file in the directory after all!
			if( file_iterator == end )
				file_iterator = filenames.begin();

			#ifdef DEBUG
			std::cout << "OPENFILES: Deleting temporary dirname. \n";
			#endif
			
			delete[] dirname;
		}	
		
	// everything seems to have worked out alright, let's return true
	if ( numfiles > 0 )
		{
		#ifdef DEBUG
		std::cout << "OPENFILES: We're all set to go and we have " << numfiles << " files." << std::endl; 
		#endif // DEBUG
		return true;
		}
		// we did not have sufficient arguments or we have made some mistake, return false
	else
		{
		#ifdef DEBUG
		std::cout << "OPENFILES: We have " << numfiles << " files." << std::endl;
		#endif
		return false;
		}	

	}


// wrapper for OpenFiles
// the while loop checks whether the file we wish to open is in the currently
// open file list
// we have to prevent it from opening directories above the one that might
// be passed here
void FileManager::OpenNewSet(int argc,char **argv)
	{
	file_iterator = filenames.begin(); // we have to check whether we're not opening a directory
	std::list<Glib::ustring>::iterator end = filenames.end();
	if( initialised && filter_filename( argv[1] ) )
		{
		while( file_iterator != end )
			{
			#ifdef DEBUG		
			std::cout << "OPENNEWSET: Looking for: " << argv[1] << " in " << *file_iterator << std::endl;
			#endif
			if( (*file_iterator).find( argv[1] ) != Glib::ustring::npos )
				break;
			file_iterator++;
			}
		}

	// if we have a file which we cannot open, it must be a directory
	if( !initialised || file_iterator == end || !filter_filename( argv[1] ) )
		{		
		if( OpenFiles(argc,argv) )
			initialised = true;
		else
			initialised = false;
		}
	else
		initialised = true;

	}

bool FileManager::have_multiple_files(void)
	{
	if( numfiles > 1 )
		return true;
	else
		return false;
	}


// note: the navigation methods are quite messy but they work as required.

Glib::ustring FileManager::get_first_file()
	{
	if( !initialised )
		return GT( "No file to load." );
	file_iterator = filenames.begin();
	return *file_iterator;
	}

Glib::ustring FileManager::get_next_file()
	{
	if( !initialised )
		return GT( "No file to load." );

	std::list<Glib::ustring>::iterator end = filenames.end();
	std::list<Glib::ustring>::iterator begin = filenames.begin();

	// no next/prev operations carried out yet, load the second image
	if( !last_op_next && !last_op_previous && file_iterator == begin )
		{
		last_op_next = true;
		last_op_previous = false;
		file_iterator++;
		if( file_iterator == end ) // if we have only one file open, this will prevent segfault
			file_iterator = begin;
		return *file_iterator;
		}

	// we've hit the end() marker going forwards, end() is invalid so let's jump
	// back to the beginning
	if( last_op_next && file_iterator == end )
		{
		last_op_next = true;
		last_op_previous = false;
		file_iterator = begin;
		return *file_iterator;
		}

	// we're going backwards in the pictures and we reach the first pictures
	// the iterator is automatically set to end() by previous and if we now click
	// next we'd show the first image twice, so show the second one instead
	if( last_op_previous && file_iterator == end )
		{
		last_op_next = true;
		last_op_previous = false;
		file_iterator = begin;
		file_iterator++;
		return *file_iterator;
		}

	// we pass past the first picture going backwards to the last picture
	// if we increment now as below (else) we'll be at end(), get_next_file() will
	// be called again and we skip the first picture, to prevent that we'll
	// just show the first picture now
	if( last_op_previous && file_iterator != end )
		{
		last_op_next = true;
		last_op_previous = false;
		file_iterator++;
		if( file_iterator == end )
			{
			file_iterator = begin;
			return *file_iterator;
			}
		else
			return *file_iterator;
		}
		
	else
		{
		last_op_next = true;
		last_op_previous = false;
		file_iterator++;
		// safeguard against end() errors
		if( file_iterator == end )
			get_next_file(); // when we're at the end, recurse
		return *file_iterator;
		}

	}

Glib::ustring FileManager::get_previous_file()
	{
	if( !initialised )
		return GT( "No file to load." );

	std::list<Glib::ustring>::iterator end = filenames.end();
	std::list<Glib::ustring>::iterator begin = filenames.begin();
		
	// no next/prev operations carried out yet, load the last image
	if( !last_op_previous && !last_op_next && file_iterator == begin )
		{
		last_op_previous = true;
		last_op_next = false;
		file_iterator = end; // don't forget, end() points to a position after the last image
		file_iterator--;
		return *file_iterator;
		}

	// we're going through the pictures backwards and hit the first one
	// we'll jump to the last one in the list
	// OR
	// we've hit the first picture going forwards and now want to go backwards
	// we display the last picture
	if( last_op_previous && file_iterator == begin || last_op_next && file_iterator == begin )
		{
		last_op_previous = true;
		last_op_next = false;
		file_iterator = end;
		file_iterator--;
		return *file_iterator;
		}
	// valid for all pictures except the first one ->
	// we decrement the iterator and display the picture
	else if( file_iterator != begin )
		{
		last_op_previous = true;
		last_op_next = false;
		file_iterator--;
		return *file_iterator;
		}

	}
		
Glib::ustring FileManager::get_current_file(void)
	{
	if( !initialised )
		return GT( "No file to load." );
	return *file_iterator;
	}

// returns true if the filename indicates a valid image format
// it is not a problem if it really isn't because gdk-pixbuf
// is safe from that perspective and we even display an errors
// image informing the user

// for speed we assume that correctly named files are actually
// images and invoke magic only if the filename is inconclusive
bool FileManager::filter_filename(Glib::ustring filename)
	{
	// check the filename first
	if( filename.find(".jpg") != Glib::ustring::npos ||
		filename.find(".JPG") != Glib::ustring::npos ||
		filename.find(".gif") != Glib::ustring::npos ||
		filename.find(".GIF") != Glib::ustring::npos ||
		filename.find(".png") != Glib::ustring::npos ||
		filename.find(".PNG") != Glib::ustring::npos ||
		filename.find(".bmp") != Glib::ustring::npos ||
		filename.find(".BMP") != Glib::ustring::npos ||
		filename.find(".pcx") != Glib::ustring::npos ||
		filename.find(".PCX") != Glib::ustring::npos ||
		filename.find(".pgm") != Glib::ustring::npos ||
		filename.find(".PGM") != Glib::ustring::npos ||
		filename.find(".ppm") != Glib::ustring::npos ||
		filename.find(".PPM") != Glib::ustring::npos ||
		filename.find(".tiff") != Glib::ustring::npos ||
		filename.find(".TIFF") != Glib::ustring::npos ||
		filename.find(".tif") != Glib::ustring::npos ||
		filename.find(".TIF") != Glib::ustring::npos ||
		filename.find(".tga") != Glib::ustring::npos ||
		filename.find(".TGA") != Glib::ustring::npos ||
		filename.find(".pbm") != Glib::ustring::npos ||
		filename.find(".PBM") != Glib::ustring::npos ||
		filename.find(".ppm") != Glib::ustring::npos ||
		filename.find(".PPM") != Glib::ustring::npos ||
		filename.find(".svg") != Glib::ustring::npos ||
		filename.find(".SVG") != Glib::ustring::npos )
		return true;
	
	// if we haven't returned yet, do file magic
	if( magic_file( cookie, filename.c_str() ) != NULL )
		{
		result = magic_file(cookie, filename.c_str() );
#ifdef DEBUG
		std::cout << filename << " " << result << std::endl;
#endif // DEBUG
		}
	else
		std::cout << GT( "FILTER_FILENAME: The file type could not be determined: " ) << filename << std::endl;

	// SVG is an xml format, libmagic gives XML, so we check the filename!
	// EPS sometimes holds TIFF images, we prevent the program from opening them!

	if( result.find("JPEG") != Glib::ustring::npos 	||
		result.find("GIF") != Glib::ustring::npos	||
		result.find("PNG") != Glib::ustring::npos	||
	    result.find("PC bitmap") != Glib::ustring::npos ||
	    result.find("PCX") != Glib::ustring::npos ||
	    result.find("PGM") != Glib::ustring::npos	||
	    result.find("PPM") != Glib::ustring::npos	||
		result.find("TIFF") != Glib::ustring::npos && result.find("EPS") == Glib::ustring::npos	||
		result.find("X pixmap image text") != Glib::ustring::npos	||
		result.find("Targa") != Glib::ustring::npos	||
		result.find("PBM") != Glib::ustring::npos	||
		result.find("SVG") != Glib::ustring::npos	||
		result.find("PPM") != Glib::ustring::npos ||
		filename.find(".svg") != Glib::ustring::npos ||
		filename.find(".SVG") != Glib::ustring::npos        )
		return true;
	else
		return false;
	}

// returns the current file's directory name if we have initialised already
// otherwise it returns the home directory
Glib::ustring FileManager::get_current_dir(void)
{
	if( initialised )
		return ( Glib::path_get_dirname( *file_iterator ) );
	else
		return( Glib::get_home_dir() );
}



// saving is only supported for png, jpeg and bmp, let's see whether we are allowed
// to save whatever the user is trying to save
Glib::ustring FileManager::filter_save_filename(Glib::ustring filename)
	{
	if( magic_file( cookie, filename.c_str() ) != NULL )
		{
		result = magic_file(cookie, filename.c_str() );
#ifdef DEBUG
		std::cout << result << std::endl;
#endif // DEBUG
		}
	else
		{
		std::cerr << GT( "FILTER_SAVE_FILENAME: The file type could not be determined: " ) << filename << std::endl;
		return "unsupported";
		}

	if( result.find("JPEG") != Glib::ustring::npos )
		return "jpeg";

	else if( result.find("PNG") != Glib::ustring::npos )
		return "png";

	else if( result.find("PC bitmap") != Glib::ustring::npos )
		return "bmp";

	else
		return  "unsupported";
	
	}

// DEBUG if you're ever wondering what we have stored!		
void FileManager::showfiles(void)
	{
	while(file_iterator != filenames.end())
		{
		std::cout << *file_iterator << std::endl;
		file_iterator++;
		}
	}
