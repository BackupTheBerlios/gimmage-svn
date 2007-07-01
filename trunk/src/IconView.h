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

// gimmage: IconView.h

#include <glibmm/dispatcher.h>

#include <gtkmm/iconview.h>
#include <gtkmm/scrolledwindow.h>

#include <gtkmm/liststore.h>
#include <gtkmm/treemodel.h>

#include <gdkmm/pixbuf.h>

#include "ThreadLoadThumbs.h"

#include "defines.h"
#include "../config.h"

// the tree model for the iconview in the preview widget
class CImageListColumns : public Gtk::TreeModelColumnRecord
{
public:
	CImageListColumns()
		{ add(basenames_column); add(thumbnails_column); add(filenames_column); }

	Gtk::TreeModelColumn<Glib::ustring> basenames_column;
	Gtk::TreeModelColumn<Glib::ustring> filenames_column;
	Gtk::TreeModelColumn< Glib::RefPtr<Gdk::Pixbuf> > thumbnails_column;
};

/* 	This is a very contrived class as it makes use of a threaded thumbnail loader
	ThreadLoadThumbs to load the thumbnails for the iconview. 
	Basic operation is as follows:
	* The IconView is created in the program but empty.
	* load_new_thumbs is called once the program has finished loading an image set
	* load_new_thumbs makes ThreadLoadThumbs execute a new thread which starts loading the image
	  thumbnails
	* When ThreadLoadThumbs is finished loading one thumbnail, it emits the cross-thread signal
	  signal_new_thumb_ready_, to which we connect in the constructor of CIconView
	* ThreadLoadThumbs has a queue of pixbufs ready for use and when the signal is emitted
	  we use on_new_thumb_ready() to get a pixbuf and add the pixbuf and the filename into
	  the iconview */

class CIconView : public Gtk::ScrolledWindow
{
public:
	CIconView();
	CIconView( Glib::ustring&,
		std::list<Glib::ustring>&);
	
	~CIconView() {}

	void load_new_thumbs( const std::list<Glib::ustring>& );
	
	bool is_loaded();
	
  // Drag Targets:
 	 std::list<Gtk::TargetEntry> listTargets;


protected:
	// event handlers
	void on_new_thumb_ready();
	void on_terminating();
	void on_terminated();
	void on_done();
	
	// drag data sender
	virtual void on_drag_data_get(const Glib::RefPtr<Gdk::DragContext>& context, Gtk::SelectionData& selection_data, guint info, guint time);
	
	bool loaded;

	Gtk::IconView	Thumbnails;
	std::list<Glib::ustring> image_filenames;

	CThreadLoadThumbs ThreadLoadThumbs;
	bool terminating;

	CImageListColumns ImageListColumns;
	Glib::RefPtr<Gtk::ListStore> refImageList;
	Gtk::IconView ImageIconView;
	
	std::vector< Glib::RefPtr<Gdk::Pixbuf> > thumbnails;

	std::list<Glib::ustring> image_filelist;
	Glib::ustring image_filename;
};
