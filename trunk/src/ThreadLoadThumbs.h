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

// gimmage: ThreadLoadThumbs.h

#include <queue>
#include <iostream>

#include <glibmm/dispatcher.h>
#include <glibmm/thread.h>
#include <gdkmm/pixbuf.h>

#include "defines.h"
#include "../config.h"

// derive from Object to support RefPtr
class thumbnail : public Glib::Object
{
public:
	thumbnail( Glib::ustring in_basename, 
			   Glib::RefPtr<Gdk::Pixbuf> &in_pixbuf ) : 
			   basename( in_basename ),
			   pixbuf( in_pixbuf ) {}
			   
	Glib::ustring basename;
	Glib::RefPtr<Gdk::Pixbuf> pixbuf;
};

class CThreadLoadThumbs : public sigc::trackable
{
public:
	CThreadLoadThumbs( const std::list<Glib::ustring>& );
	CThreadLoadThumbs();
	virtual ~CThreadLoadThumbs();
	  
	void load_new( const std::list<Glib::ustring>& );
	
	bool is_terminating();

	Glib::Dispatcher    signal_new_thumb_ready_;
	Glib::Dispatcher	signal_terminating_;
	Glib::Dispatcher	signal_terminated_;
	Glib::RefPtr<thumbnail> get_next_thumb();
	
	std::queue< Glib::RefPtr<thumbnail> > thumbs_queue_;
	
	Glib::Mutex thumbs_mutex_;

private:
	// we keep a local copy for safety
	std::list<Glib::ustring> image_filelist;

	Glib::Cond	reading_;
	Glib::Cond terminating_;
	Glib::Thread*	thread_;

	bool terminate;

	void thread_function_load_thumbs(); // create thumbs , add to queue
	 

	
};	

