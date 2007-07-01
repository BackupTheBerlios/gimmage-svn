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

// gimmage: ThreadLoadThumbs.cpp

#include "ThreadLoadThumbs.h"

CThreadLoadThumbs::CThreadLoadThumbs( const std::list<Glib::ustring> &filelist )
	{
	image_filelist = filelist;
	terminate = false;
	thread_ = NULL;
	
	// launch the new thread
	thread_ = Glib::Thread::create(sigc::mem_fun(*this,&CThreadLoadThumbs::thread_function_load_thumbs), false);
	thread_->set_priority(Glib::THREAD_PRIORITY_LOW);
	}
	
// initialise without launching a thread	
CThreadLoadThumbs::CThreadLoadThumbs()
	{
	thread_ = NULL;
	terminate = false;
	}
	
void CThreadLoadThumbs::thread_function_load_thumbs()
	{
#ifdef DEBUG
	std::cout << "THREAD_FUNCTION_LOAD_THUMBS: called\n";
#endif //DEBUG	 
	
	std::list<Glib::ustring>::iterator f_iterator = image_filelist.begin();
	std::list<Glib::ustring>::iterator f_end = image_filelist.end();
	
	while( f_iterator != f_end )
		{
		if( terminate )
			{
#ifdef DEBUG			
			std::cout << "THREAD_FUNCTION_LOAD_THUMBS: we're being terminated\n";
#endif // DEBUG			
			// emit a dispatcher signal that we're being terminated
			signal_terminating_.emit();
			
			//block access to class elements
			Glib::Mutex::Lock lock (thumbs_mutex_);
			// empty the queue of thumbnails
			while( !thumbs_queue_.empty() )
				thumbs_queue_.pop();
	
			image_filelist.clear();
			terminating_.broadcast(); // broadcast a signal to the methods waiting for our termination
			signal_terminated_.emit(); // emit a dispatcher signal that we're done
			throw Glib::Thread::Exit();
			}
		try
			{
			Glib::RefPtr<Gdk::Pixbuf> pixbuf =
			Gdk::Pixbuf::create_from_file( *f_iterator,
			 48,
			 48 );
			 
			if( pixbuf )
				{
				Glib::Mutex::Lock lock( thumbs_mutex_ );
				
				thumbs_queue_.push(	Glib::RefPtr<thumbnail>( new thumbnail( 
						*f_iterator,
						pixbuf ) ) ); 
#ifdef DEBUG								
	std::cout << "THREAD_FUNCTION_LOAD_THUMBS: thumbnail ready: " << *f_iterator << std::endl;
#endif // DEBUG
				signal_new_thumb_ready_.emit();
				}
			else
				{}
			
			f_iterator++;	
			}
		
		// when catching exceptions wee need to make sure to still increment the 
		// file iterator or we'll end up in an infinite loop	
		catch(Gdk::PixbufError & error)
			{
				f_iterator++;
				std::cerr << "THREAD_FUNCTION_LOAD_THUMBS: PixbufError\n";
			}
		catch(Glib::FileError & error)
			{
				f_iterator++;
				std::cerr << "THREAD_FUNCTION_LOAD_THUMBS: FileError\n";
			}
		}
	
	// set thread_ to NULL when done successfully
	thread_ = NULL;
	
	// emit a dispatcher signal that we completed loading successfully
	signal_done_.emit();
	}
	
Glib::RefPtr<thumbnail> CThreadLoadThumbs::get_next_thumb()
	{
	Glib::Mutex::Lock lock (thumbs_mutex_);
	if( !thumbs_queue_.empty() )
		{
		Glib::RefPtr<thumbnail> next_thumbnail = thumbs_queue_.front();
		thumbs_queue_.pop();
		
#ifdef DEBUG
	std::cout << "GET_NEXT_THUMB: sending next thumbnail to iconview: " << next_thumbnail->basename << std::endl;
#endif // DEBUG			

		return next_thumbnail;
		}
	else
		{
#ifdef DEBUG
	std::cout << "GET_NEXT_THUMB: sending empty thumbnail to iconview\n";
#endif // DEBUG						
		Glib::RefPtr<Gdk::Pixbuf> pixbuf;
		return Glib::RefPtr<thumbnail>( new thumbnail( "", pixbuf ) );
		}
	}

bool CThreadLoadThumbs::is_terminating()
	{
	return terminate;
	}

void CThreadLoadThumbs::load_new( const std::list< Glib::ustring> &filelist )
	{
	if( thread_ != NULL )
		{
		terminate = true;
		std::cout << "CTHREADLOADTHUMBS::LOAD_NEW : waiting for thread to finish\n";
		terminating_.wait( thumbs_mutex_ );
		std::cout << "CTHREADLOADTHUMBS::LOAD_NEW : we're good to go\n";
		}
	image_filelist = filelist;
	thumbs_mutex_.unlock();

#ifdef DEBUG
	std::cout << "CTHREADLOADTHUMBS::LOAD_NEW : launching thread\n";
#endif // DEBUG
		
	// launch new loader thread
	terminate = false;
	thread_ = NULL;
	thread_ = Glib::Thread::create(sigc::mem_fun(*this,&CThreadLoadThumbs::thread_function_load_thumbs),false);
	thread_->set_priority(Glib::THREAD_PRIORITY_LOW);
	}	

