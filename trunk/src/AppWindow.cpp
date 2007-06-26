/* Copyright 2006 Bartek Kostrzewa
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

// gimmage : AppWindow.cpp

#include <iostream>

#include "AppWindow.h"
#include "defines.h"

#include "../config.h"

extern "C" {
#include <libintl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// we need this to unescape uris!
#include <curl/curl.h>
#include <gtk-2.0/gdk/gdkkeysyms.h>
}



AppWindow::AppWindow(int argnum, char **argcon) :
	Hand(Gdk::FLEUR),
	Watch(Gdk::WATCH),
	FileChooser(Gtk::FILE_CHOOSER_ACTION_OPEN),
	Button_Tooltips(),
	ImageScroller(),
	SaveDialog( GT( "Save Image?" ),*this,true,true),
	SaveErrorDialog( GT( "Saving failed." ),*this,true,true),
	ImageManager(argnum,argcon),
	NavButtonHBox_Alignment(Gtk::ALIGN_CENTER,Gtk::ALIGN_CENTER),
	I_Button_Previous(Gtk::StockID("gtk-go-back"), Gtk::IconSize(1)),
	I_Button_Next(Gtk::StockID("gtk-go-forward"), Gtk::IconSize(1)),
	I_Button_ZoomIn(Gtk::StockID("gtk-zoom-in"), Gtk::IconSize(1)),
	I_Button_ZoomOut(Gtk::StockID("gtk-zoom-out"), Gtk::IconSize(1)),
	I_Button_Zoom1to1( iconpath + (Glib::ustring) "1to1.png" ),
	I_Button_RotateAntiClockwise( iconpath + (Glib::ustring) "anticlockwise.png" ),
	I_Button_RotateClockwise( iconpath + (Glib::ustring) "clockwise.png" ),
	I_Button_Save(Gtk::StockID("gtk-floppy"), Gtk::IconSize(1)),
	I_Button_Print(Gtk::StockID("gtk-print"), Gtk::IconSize(1)),
	I_ToggleButton_ShowFileChooser(Gtk::StockID("gtk-find"), Gtk::IconSize(1)),
	I_Button_Quit(Gtk::StockID("gtk-quit"), Gtk::IconSize(1))
	{
	// initialise printing system
	refPageSetup = Gtk::PageSetup::create();
	refPrintSettings = Gtk::PrintSettings::create();
	
	// copy command line
	argc = argnum;
	argv = argcon;

	load_config();
	
	set_title("gimmage");
	set_border_width(0);
	resize(w_width,w_height);

	// note, this is accessed through a pointer by some functions in ImageEventBox, be careful
	scalefactor = 0.6;

	// we will use these to use *_scroller->set_value() and get_value
	h_scroller = ImageScroller.get_hadjustment();
	v_scroller = ImageScroller.get_vadjustment();
	
	h_scroller->set_step_increment( 10 );
	v_scroller->set_step_increment( 10 );
	
	ImageScroller.set_shadow_type(Gtk::SHADOW_NONE);


	// Image objects for the navigation buttons
	Button_Previous.add(I_Button_Previous);
	Button_Next.add(I_Button_Next);
	Button_ZoomIn.add(I_Button_ZoomIn);
	Button_ZoomOut.add(I_Button_ZoomOut);
	Button_Zoom1to1.add(I_Button_Zoom1to1);
	Button_RotateAntiClockwise.add(I_Button_RotateAntiClockwise);
	Button_RotateClockwise.add(I_Button_RotateClockwise);
	Button_Save.add(I_Button_Save);
	Button_Print.add(I_Button_Print);
	ToggleButton_ShowFileChooser.add(I_ToggleButton_ShowFileChooser);
	Button_Quit.add(I_Button_Quit);

	// Initialize Tooltips for buttons
	Button_Tooltips.set_tip( Button_Previous			, GT( "Previous Image (Backspace)" ) );
	Button_Tooltips.set_tip( Button_Next				, GT( "Next Image (Space)" ));
	Button_Tooltips.set_tip( Button_ZoomIn				, GT( "Zoom In (+)" ) );
	Button_Tooltips.set_tip( Button_ZoomOut				, GT( "Zoom Out (-)" ) );
	Button_Tooltips.set_tip( Button_Zoom1to1			, GT( "Zoom to Original Size (1)" ) );
	Button_Tooltips.set_tip( Button_RotateAntiClockwise	, GT( "Rotate 90 Degrees Anticlockwise (a)" ) );
	Button_Tooltips.set_tip( Button_RotateClockwise		, GT( "Rotate 90 Degrees Clockwise (c)" ) );
	Button_Tooltips.set_tip( Button_Save				, GT( "Save Modified Image (s)" ) );
	Button_Tooltips.set_tip( Button_Print				, GT( "Display Print Dialog (p)" ) );
	Button_Tooltips.set_tip( ToggleButton_ShowFileChooser, GT( "Show File Browser (f)" ) );
	Button_Tooltips.set_tip( Button_Quit				, GT( "Quit. (Escape)" ) );
	
	// the navigation buttons themselves
	NavButtonHBox.set_homogeneous(true);
	NavButtonHBox.pack_start(Button_Previous, Gtk::PACK_SHRINK, 5);
	NavButtonHBox.pack_start(Button_Next, Gtk::PACK_SHRINK, 5);
	NavButtonHBox.pack_start(Button_ZoomOut, Gtk::PACK_SHRINK, 5);
	NavButtonHBox.pack_start(Button_ZoomIn, Gtk::PACK_SHRINK, 5);
	NavButtonHBox.pack_start(Button_Zoom1to1, Gtk::PACK_SHRINK, 5);
	NavButtonHBox.pack_start(Button_RotateAntiClockwise, Gtk::PACK_SHRINK, 5);
	NavButtonHBox.pack_start(Button_RotateClockwise, Gtk::PACK_SHRINK, 5);
	NavButtonHBox.pack_start(Button_Save, Gtk::PACK_SHRINK, 5);
	NavButtonHBox.pack_start(Button_Print, Gtk::PACK_SHRINK, 5);
	NavButtonHBox.pack_start(ToggleButton_ShowFileChooser, Gtk::PACK_SHRINK, 5);
	NavButtonHBox.pack_start(Button_Quit, Gtk::PACK_SHRINK, 5);

	// put the nav buttons into an alignment so they're always nicely centered!
	NavButtonHBox_Alignment_HBox.pack_start(NavButtonHBox,Gtk::PACK_EXPAND_PADDING,0);
	NavButtonHBox_Alignment.add(NavButtonHBox_Alignment_HBox);

	// make the scrollbars dissapear when the image fits the window
	ImageScroller.set_policy(Gtk::POLICY_AUTOMATIC,Gtk::POLICY_AUTOMATIC);
	// add the imagebox to the scrolledwindow
	ImageScroller.add(ImageBox);
	VBox_Top.pack_start(ImageScroller, Gtk::PACK_EXPAND_WIDGET,0);
	VBox_Top.pack_start(NavButtonHBox_Alignment, Gtk::PACK_SHRINK,5);
	VPaned.pack1(VBox_Top);
	VPaned.pack2(FileChooser, Gtk::PACK_SHRINK, 10);

	// add toplevel container to window
	add(VPaned);

	// set up the file filter for pixbuf types
	ImageFilter.add_pixbuf_formats();
	ImageFilter.set_name( GT( "Image Formats" ) );
	FileChooser.add_filter(ImageFilter);
	FileChooser.set_select_multiple();
	FileChooser.set_preview_widget( Preview );
	FileChooser.set_preview_widget_active();
	FileChooser.set_use_preview_label(false);
	FileChooser.signal_file_activated().connect( sigc::mem_fun(*this,&AppWindow::on_file_activated) );
	
	// connect the preview widget to the update_preview signal
	FileChooser.signal_update_preview().connect( sigc::mem_fun(*this,&AppWindow::on_update_preview) );
		

	// set up event mask for ImageBox (this doesn't really seem to work but hey... )
	ImageBox.set_events( Gdk::ALL_EVENTS_MASK & ~Gdk::ALL_EVENTS_MASK );
	ImageBox.add_events(  Gdk::EXPOSURE_MASK |
		Gdk::BUTTON_MOTION_MASK|Gdk::BUTTON_PRESS_MASK|
		Gdk::BUTTON_RELEASE_MASK|Gdk::SCROLL_MASK );

	// set up drag source and drop target for filenames from filechooser filechooser
	std::list<Gtk::TargetEntry> dragndrop;
	dragndrop.push_back( Gtk::TargetEntry("text/plain") );
	dragndrop.push_back( Gtk::TargetEntry("text/uri-list") );

	ImageBox.drag_dest_set(dragndrop);
	ImageBox.signal_drag_data_received().connect( sigc::mem_fun(*this,&AppWindow::on_drag_data_received) );

	// Drag scrolling the image
	ImageBox.signal_motion_notify_event().connect(sigc::mem_fun(*this,&AppWindow::on_button1_pressed_motion));
	ImageBox.signal_button_press_event().connect(sigc::mem_fun(*this,&AppWindow::on_button_press_event));
	ImageBox.signal_button_release_event().connect(sigc::mem_fun(*this,&AppWindow::on_button_release_event));

	// connect signal handlers	
	Button_Next.signal_clicked().connect(sigc::mem_fun(*this,&AppWindow::on_button_next) );
	Button_Previous.signal_clicked().connect(sigc::mem_fun(*this,&AppWindow::on_button_previous) );

	Button_ZoomIn.signal_clicked().connect(sigc::mem_fun(*this,&AppWindow::on_button_zoom_in) );
	Button_ZoomOut.signal_clicked().connect(sigc::mem_fun(*this,&AppWindow::on_button_zoom_out) );
	Button_Zoom1to1.signal_clicked().connect(sigc::mem_fun(*this,&AppWindow::on_button_zoom_1to1) );

	Button_RotateAntiClockwise.signal_clicked().connect(
							sigc::mem_fun(*this,&AppWindow::on_button_rotate_anticlockwise) );
	Button_RotateClockwise.signal_clicked().connect(
							sigc::mem_fun(*this,&AppWindow::on_button_rotate_clockwise) );

	Button_Save.signal_clicked().connect( sigc::mem_fun(*this,&AppWindow::on_button_save) );
	SaveDialog.signal_response().connect(sigc::mem_fun(*this,&AppWindow::on_save_dialog_response) );
	SaveErrorDialog.signal_response().connect(sigc::mem_fun(*this,&AppWindow::on_save_error_dialog_response) );

	Button_Print.signal_clicked().connect(sigc::mem_fun(*this,&AppWindow::on_button_print) );

	
	
	ToggleButton_ShowFileChooser.signal_clicked().
							connect(sigc::mem_fun(*this, &AppWindow::on_button_show_filechooser) );

	Button_Quit.signal_clicked().connect(sigc::mem_fun(*this, &AppWindow::on_button_quit) );
	signal_delete_event().connect(sigc::mem_fun(*this, &AppWindow::on_delete_event) );
	
	// this is also triggered when the image is resized, unforunately (evenbox)
	//signal_check_resize().connect(sigc::mem_fun(*this,&AppWindow::on_resize));

	// let's catch keystrokes
	ImageBox.signal_key_press_event().connect(sigc::mem_fun(*this,&AppWindow::on_key_press_event) );
	
	// show the window in all its glory
	show_all_children();

	// now hide the filebrowser, i'm lazy to type show for all children!
	// 220 as a height is appropriate for three rows in the filechooser	
	FileChooser.set_size_request( FileChooser.get_width(), 220 );
	FileChooser.hide();
	// make the save button insensitive, it will be usable once it is needed.
	Button_Save.set_sensitive(false);
	Button_Print.set_sensitive(false);

	// we don't want to cause ruckus do we? let's check whether we have more than 1 file
	// and disable the next and previous button if we don't
	if( !ImageManager.have_multiple_files() )
		{
		Button_Next.set_sensitive(false);
		Button_Previous.set_sensitive(false);
		}
		
	// clamp the scrollbar adjustments
	if( ImageBox.is_loaded() )
		{
		v_scroller->set_upper( ImageBox.get_height() - v_scroller->get_page_size() );
		h_scroller->set_upper( ImageBox.get_width() - h_scroller->get_page_size() );
		}

	// load the first image
	set_title( "gimmage: " + ImageManager.get_current_file() );
	ImageBox.LoadImage(	ImageManager.get_current_file(),
						&scalefactor, 
						v_width,
						v_height);
						
	set_buttons_active( ImageBox.is_loaded() );

	}

AppWindow::~AppWindow()
	{
	}

void AppWindow::set_buttons_active(bool active)
	{
	Button_Print.set_sensitive(active);
	Button_ZoomIn.set_sensitive(active);
	Button_ZoomOut.set_sensitive(active);
	Button_Zoom1to1.set_sensitive(active);
	Button_RotateClockwise.set_sensitive(active);
	Button_RotateAntiClockwise.set_sensitive(active);
	}

bool AppWindow::on_button_press_event(GdkEventButton *event)
 	{
 		switch( event->button )
 		{
 		case 1:
			int x, y;
			ImageBox.get_window()->set_cursor(Hand);	
			ImageScroller.get_pointer(x, y);
			dragoldx = x;
			dragoldy = y;
			return true;
			break; //just to be sure
			
		// mouse wheel events are handled as buttons 4 and 5
		// we set dragoldx and dragoldy to -999999 as a condition
		// for drag scrolling, so we only drag scroll on mouse
		// button 1
		case 4:
			on_button_next();
			dragoldx = dragoldy = -999999;
			return true;
			break;
			
		case 5:
			on_button_previous();
			dragoldx = dragoldy = -999999;
			return true;
			break;
		
		default:
			dragoldx = dragoldy = -999999;
			return false;
			break;
		}
 	}

bool AppWindow::on_button_release_event(GdkEventButton *event)
	{
	#ifdef DEBUG
	std::cout << "ON_BUTTON_RELEASE_EVENT: released! \n";
	#endif
		// set_cursor() sets the cursor to the parent window's cursor, which should
		// properly handle right- and left-pointing cursors
		ImageBox.get_window()->set_cursor();
		return true;
	}


// this drags the image around
// big thanks to Ramax Lo for fixing this!!
bool AppWindow::on_button1_pressed_motion(GdkEventMotion *event)
	{
	if( dragoldx != -999999 && dragoldy != -999999 )
		{
		if ( ImageBox.is_loaded() )
			{
			int x,y;
			ImageScroller.get_pointer(x, y);
			
#ifdef DEBUG
std::cout << "Y = \t" << event->y << '\t';
std::cout << "Oldy = \t" << dragoldy << std::endl;
std::cout << "Pointer x = " << x << std::endl;
std::cout << "Pointer y = " << y << std::endl;
#endif // DEBUG
	
			if(h_scroller->get_page_size() < ImageBox.get_image_width())
				{
				if( (h_scroller->get_value() - (x - dragoldx)) >= 0  &&
					(h_scroller->get_value() - (x - dragoldx)) <=
					( ImageBox.get_image_width() - h_scroller->get_page_size() ) )
					{
					h_scroller->set_value(h_scroller->get_value() - (x - dragoldx));
					dragoldx = x;
					}
					
				// snap to end	
				else if(	(h_scroller->get_value() - (x - dragoldx)) < 0 )
					h_scroller->set_value(0);
				else if(	(h_scroller->get_value() - (x - dragoldx)) >
					( ImageBox.get_image_width() - h_scroller->get_page_size() ) )
					h_scroller->set_value( ImageBox.get_image_width() - h_scroller->get_page_size() );
				}
	
			if(v_scroller->get_page_size() < ImageBox.get_image_height())
				{
				if( (v_scroller->get_value() - (y-dragoldy)) >= 0  &&
					(v_scroller->get_value() - (y-dragoldy)) <=
					(ImageBox.get_image_height() - 	v_scroller->get_page_size() ))
					{
					v_scroller->set_value(v_scroller->get_value() - (y-dragoldy));
					dragoldy = y;
					}
				// 	snap to end
				else if(	(v_scroller->get_value() - (y - dragoldy)) < 0 )
					v_scroller->set_value(0);
				else if(	(v_scroller->get_value() - (y - dragoldy)) >
					( ImageBox.get_image_height() - v_scroller->get_page_size() ) )
					v_scroller->set_value( ImageBox.get_image_height() - v_scroller->get_page_size() );
				}
	
			return true;
			}
		}
	return false;
	}




void AppWindow::on_drag_data_received(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, const Gtk::SelectionData& selection_data, guint info, guint time)
	{  
    // this is totally ridiculous, but so be it!
	// we erase the protocol in front of the filename
	// using std::string because Glib::ustring throws a conversion error for some reason
	std::string string_filename = selection_data.get_data_as_string();
	string_filename.erase(0, string_filename.find(':')+3); 

	//convert to char* and unescape
	char * temp_filename = curl_unescape( string_filename.c_str(), 0);
	string_filename = temp_filename;
	// curl requires this to be freed
	curl_free( temp_filename );
	
	//let's remove any \n and \r that are typical of URI's
	if ( string_filename.find('\n') != Glib::ustring::npos )
		string_filename.erase( string_filename.find('\n'), 1 );
	if ( string_filename.find('\r') != Glib::ustring::npos )
		string_filename.erase( string_filename.find('\r'), 1 );

	open_new_file( string_filename );
	context->drag_finish(false, false, time);
	}


void AppWindow::busy(bool showwatch)
	{
	if( showwatch )
		{
		get_window()->set_cursor(Watch); // indicate that we're working
		ImageBox.get_window()->set_cursor(Watch);
		get_display()->flush();
		}
	else
		{
    	get_window()->set_cursor();
    	ImageBox.get_window()->set_cursor();
		}
	}
		

void AppWindow::open_new_file( Glib::ustring string_filename )
	{
	busy(true);
	char * filename = new char[string_filename.length() + 1 ];

	strcpy ( filename , string_filename.c_str() );

#ifdef DEBUG
	std::cout << "OPEN_NEW_FILE: AppWindow opens " << filename << std::endl;
#endif // DEBUG

	// this is a neat trick to use our argv reader internally, we will never drag more
	// than one URI (folder or file)!
	char *arguments[] = { "empty", filename };
	ImageManager.OpenNewSet(2, arguments);

	#ifdef DEBUG
	std::cout << "OPEN_NEW_FILE(AppWindow): Deleting the allocated memory for temporary filename.\n";
	#endif 
	delete[] filename;

	// if the buttons were not active, let's make them active if we have multiple files
	// if we don't have multiple files, let's make them inactive
	if( ImageManager.have_multiple_files() )
		{ 
		Button_Next.set_sensitive(true);
		Button_Previous.set_sensitive(true);
		}
	else
		{
		Button_Next.set_sensitive(false);
		Button_Previous.set_sensitive(false);
		}

	// set the title, load the image
	set_title( "gimmage: " + ImageManager.get_current_file() );
	ImageBox.LoadImage(	ImageManager.get_current_file(),
						&scalefactor, 
						(int)h_scroller->get_page_size(),
						(int)v_scroller->get_page_size());

    // finally, set the filechooser to the right dirname
    set_filechooser_dir();

    // activate the buttons
    set_buttons_active( ImageBox.is_loaded() );
	busy(false);

	// since we now have a new image, let's make sure the save button is off!
	Button_Save.set_sensitive( false );
}

// we're told to update the preview widget, so we do
void AppWindow::on_update_preview(void)
	{
	//std::cout << "update preview\n";
	Preview.load( FileChooser.get_preview_filename(), 
		ImageManager.filter_filename( FileChooser.get_preview_filename() )  );
	}

void AppWindow::on_file_activated(void)
{
	open_new_file( FileChooser.get_filename() );
}

void AppWindow::set_filechooser_dir(void)
	{
	if( FileChooser.is_visible() )
		{
		
#ifdef DEBUG
std::cout << "SET_FILECHOOSER_DIR: Before set_file_chooser_dir()\n";
std::cout << "SET_FILECHOOSER_DIR: get_filename(): " << FileChooser.get_filename() << std::endl;
std::cout << "SET_FILECHOOSER_DIR: get_current_file(): " << ImageManager.get_current_file() << std::endl;
#endif // DEBUG
		
		Glib::ustring path;
		
		if( FileChooser.get_filename() != ImageManager.get_current_file() &&
			ImageBox.is_loaded() )
			{
				if( !Glib::path_is_absolute( ImageManager.get_current_file() ) )
					path = ImageManager.get_current_dir() + ImageManager.get_current_file();
				else
					path = ImageManager.get_current_file();

				FileChooser.set_filename( path ); // this doesn't seem to have any effect whatsoever

#ifdef DEBUG
std::cout << "SET_FILECHOOSER_DIR: After set_file_chooser_dir()\n";
std::cout << "SET_FILECHOOSER_DIR: get_filename(): " << FileChooser.get_filename() << std::endl;
std::cout << "SET_FILECHOOSER_DIR: get_current_file(): " << ImageManager.get_current_file() << std::endl;
std::cout << "SET_FILECHOOSER_DIR: path: " << path << std::endl;
#endif // DEBUG

			}
			
		// if loading failed, set to the user's home dir
		else if( !ImageBox.is_loaded() )
			FileChooser.set_current_folder( Glib::get_home_dir() );
			
		// workaround for (i think) a bug in the filechooser widget signalling
		Preview.load( ImageManager.get_current_file(), 
			ImageManager.filter_filename( ImageManager.get_current_file() ) );
   	 	}
	}

// scales an image using the scalefactor only
void AppWindow::set_scalefactor(double scale)
	{
	scalefactor = scale;
	ImageBox.ScaleImage(scalefactor,INTERPTYPE);
	}

// Event Handlers

void AppWindow::on_button_next(void)
	{
	#ifdef DEBUG
	std::cout << "ON_BUTTON_NEXT: Next file \n";
	#endif
	busy(true);
	ImageBox.LoadImage(	ImageManager.get_next_file(), 
						&scalefactor,
						(int)h_scroller->get_page_size(), 
						(int)v_scroller->get_page_size());

	set_title( "gimmage: " + ImageManager.get_current_file() );
	set_buttons_active( ImageBox.is_loaded() );


	set_filechooser_dir();
	
	// flush the queue so that the image is displayed
	// even if the user presses and holds the space bar
	get_display()->flush();

	// since we now have a new image, let's make sure the save button is off!
	Button_Save.set_sensitive( false );
	busy(false);
	} 
	
void AppWindow::on_button_previous(void)
	{
	#ifdef DEBUG
	std::cout << "ON_BUTTON_PREVIOUS: Previous file \n";
	#endif
	busy(true);
	ImageBox.LoadImage(	ImageManager.get_previous_file(),
						&scalefactor,
						(int)h_scroller->get_page_size(), 
						(int)v_scroller->get_page_size());
						
	
	set_title( "gimmage: " + ImageManager.get_current_file() );
	set_buttons_active( ImageBox.is_loaded() );

	set_filechooser_dir();
	
	// flush the queue so that the image is displayed
	// even if the user presses and holds the backspace key
	get_display()->flush();

	// since we now have a new image, let's make sure the save button is off!
	Button_Save.set_sensitive( false );
	busy(false);
	}

void AppWindow::on_button_zoom_in(void)
	{
	double oldscale = scalefactor;
	if(scalefactor < 3.0 && ImageBox.is_loaded())
		{
		busy(true);
		(scalefactor > 0.2) ?	scalefactor += scalefactor*0.2 : scalefactor *= 1.5;
		ImageBox.ScaleImage(scalefactor,INTERPTYPE);
		adjust_adjustment_on_zoom(oldscale);
		busy(false);
		}
	}

void AppWindow::on_button_zoom_out(void)
	{
	double oldscale = scalefactor;
	if(scalefactor > 0.05 && ImageBox.is_loaded())
		{
		busy(true);
		(scalefactor > 0.2) ? scalefactor -= scalefactor*0.2 : scalefactor *= 0.667;
		ImageBox.ScaleImage(scalefactor,INTERPTYPE);
		adjust_adjustment_on_zoom(oldscale);
		busy(false);
		}
	}

// return the image to 1:1 scale
void AppWindow::on_button_zoom_1to1(void)
	{
	double oldscale = scalefactor;
	if(scalefactor != 1.0 && ImageBox.is_loaded())
		{
		busy(true);
		scalefactor = 1.0;
		ImageBox.ScaleImage(scalefactor,INTERPTYPE);
		adjust_adjustment_on_zoom(oldscale);
		busy(false);
		}
	}

void AppWindow::adjust_adjustment_on_zoom(double oldscale)
	{
	// clamp the scrollbar adjustments
	if( ImageBox.is_loaded() )
		{
		v_scroller->set_upper( ImageBox.get_height() - v_scroller->get_page_size() );
		h_scroller->set_upper( ImageBox.get_width() - h_scroller->get_page_size() );
		}	
	
	double ratio = scalefactor / oldscale;
	// adjust the scrollbar to keep the image centered
		h_scroller->set_value( ( ratio-1.0 ) * h_scroller->get_page_size()/2.0 +
								h_scroller->get_value()*ratio );

		v_scroller->set_value( ( ratio-1.0 ) * v_scroller->get_page_size()/2.0 +
									v_scroller->get_value()*ratio );
	}

void AppWindow::on_button_rotate_anticlockwise(void)
	{
	if( ImageBox.is_loaded() )
		{
		busy(true);
		double h_temp = h_scroller->get_value();
		double v_temp = v_scroller->get_value();
		ImageBox.RotateImage(Gdk::PIXBUF_ROTATE_COUNTERCLOCKWISE);
		adjust_adjustment_on_rotate(-90,h_temp,v_temp);
		busy(false);
		}
	}

void AppWindow::on_button_rotate_clockwise(void)
	{
	if( ImageBox.is_loaded() )
		{
		busy(true);
		double h_temp = h_scroller->get_value();
		double v_temp = v_scroller->get_value();
		ImageBox.RotateImage(Gdk::PIXBUF_ROTATE_CLOCKWISE);
		adjust_adjustment_on_rotate(90,h_temp,v_temp);
		busy(false);
		}
	}

// this keeps the same midpoint upon rotation, it might not be the best method, but it
// works adequately
// the function also makes the save button appear as it is called by both rotators
// it will be hidden when the user clicks ok and stay when cancel is clicked
void AppWindow::adjust_adjustment_on_rotate(int angle, double h_old, double v_old)
	{
	if( ImageManager.filter_save_filename( ImageManager.get_current_file() ) != "unsupported" )
		Button_Save.set_sensitive(true);
		
	if		( angle == 90 )
		{
		v_scroller->set_value( h_old );
		h_scroller->set_value(	ImageBox.get_image_width() -
								v_old -
								h_scroller->get_page_size() );
		}
	else if ( angle == -90)
		{
		h_scroller->set_value( v_old );
		v_scroller->set_value(	ImageBox.get_image_height() -
								h_old -
								v_scroller->get_page_size() );
		}
	}


// saves the file but with the new orientation
// and makes the save button insensitive, but only after checking with
// the save dialog!
void AppWindow::on_button_save(void)
	{
	// set the right filename!
	SaveDialog.FileEntry.set_text( ImageManager.get_current_file() );
	SaveDialog.show();
	}


// this calls ImageManager.filter_save_filename() to check whether the current file is a format
// supported for saving, filter_save_filename() returns the file format that is then taken by
// ImageBox.save_image() to save in the correct format.
void AppWindow::on_save_dialog_response(int response)
	{
	SaveDialog.hide();

	if(response == 1)
		{
		if( ImageBox.save_image( SaveDialog.FileEntry.get_text(),
			ImageManager.filter_save_filename( ImageManager.get_current_file() )  ) )
			{
			Button_Save.set_sensitive(false);
			}
		else
			{
			SaveErrorDialog.show();
			}
		}
	}

void AppWindow::on_save_error_dialog_response(int response)
	{
	SaveErrorDialog.hide();
	}

void AppWindow::on_button_print(void)
	{
		print(Gtk::PRINT_OPERATION_ACTION_PRINT_DIALOG);
	}
	
void AppWindow::print(Gtk::PrintOperationAction print_action)
	{
	Glib::RefPtr<CPrint> print = CPrint::create();
	
	print->set_track_print_status();
	print->set_default_page_setup(refPageSetup);
	print->set_print_settings(refPrintSettings);
	
	//connect to print done
	
	try
		{
		print->run(print_action, *this);
		}
	catch(const Gtk::PrintError& error)
		{
		std::cerr << "An error occured while trying to run print(): " << error.what() << std::endl; 
		}
	}	

/* hide() destroys the underlying GObject (i think), so we'd like to use 
   resize instead, otherwise the filechooser is not updated
   properly (Bug http://developer.berlios.de/bugs/?func=detailbug&bug_id=11277&group_id=5973) 
   However, that does not work either because once the user changes the size of the filechooser,
   it all breaks...
*/
void AppWindow::on_button_show_filechooser(void)
	{
	FileChooser.is_visible() ? FileChooser.hide() : FileChooser.show();
	
	// attempt to get the filechooser displayed before altering it's path
	// doesn't always work which results in the wrong path being
	// selected in the filechooser if you press f repeatedly quickly
	while(Gtk::Main::events_pending()) Gtk::Main::iteration();

	ImageBox.ScaleImage2((int)h_scroller->get_page_size(),
						(int)v_scroller->get_page_size(),
						&scalefactor);
	
	set_filechooser_dir();
	}


// Quit button pressed; quit program.
void AppWindow::on_button_quit(void)
	{
	save_config();
	hide();
	}

// catch delete events and save correctly before closing
bool AppWindow::on_delete_event( GdkEventAny *event )
	{
	on_button_quit();
	return true;
	}

	// arrows don't work, KP arrows work
bool AppWindow::on_key_press_event(GdkEventKey * key)
	{
	int increment = 20;
	
	switch( key->keyval )
		{
		case GDK_Tab:
			break;
			
		case GDK_Escape:
			#ifdef DEBUG
			std::cout << "KEY_EVENT: Escape was pressed. Quitting... \n";
			#endif
			Button_Quit.activate();
			break;
			
		case GDK_BackSpace:
			#ifdef DEBUG
			std::cout << "KEY_EVENT: BackSpace was pressed. \n";
			#endif
			if( Button_Previous.is_sensitive() )
				Button_Previous.activate();
			return false;
			break;
			
		//case GDK_KP_Space:
		case GDK_space:
			#ifdef DEBUG
			std::cout << "KEY_EVENT: Space was pressed. \n";
			#endif
			if( Button_Next.is_sensitive() )
				Button_Next.activate();
			break;
			
		case GDK_KP_Add:
		case GDK_plus:
			#ifdef DEBUG
			std::cout << "KEY_EVENT: Plus was pressed. \n";
			#endif
			if( Button_ZoomIn.is_sensitive() )
				Button_ZoomIn.activate();
			break;
			
		case GDK_KP_Subtract:
		case GDK_minus:
			#ifdef DEBUG
			std::cout << "KEY_EVENT: Minus was pressed. \n";
			#endif
			if( Button_ZoomOut.is_sensitive() )
				Button_ZoomOut.activate();
			break;
			
		case GDK_Left:	
		case GDK_KP_Left:
		case GDK_leftarrow:
			#ifdef DEBUG
			std::cout << "KEY_EVENT: Left was pressed. \n";
			#endif
			if( ImageBox.is_loaded() )
				{
				if( h_scroller->get_value()-increment >= 0 )
					h_scroller->set_value( h_scroller->get_value()-increment );
				else // subtract the few pixels that are missing
					h_scroller->set_value(0);
				}
			return false;
			break;
			
		case GDK_Right:	
		case GDK_KP_Right:
		case GDK_rightarrow:
			#ifdef DEBUG
			std::cout << "KEY_EVENT: Right was pressed. \n";
			#endif
			if( ImageBox.is_loaded() )
				{			
				if ( (h_scroller->get_value() + increment) <=
				(ImageBox.get_image_width() - h_scroller->get_page_size() ) )
					h_scroller->set_value( h_scroller->get_value()+increment);
				else // add that extra bit
					h_scroller->set_value( 	ImageBox.get_image_width() -
											h_scroller->get_page_size() );
				}
			break;
			
		case GDK_KP_Down:
		case GDK_Down:
		case GDK_downarrow:
			#ifdef DEBUG
			std::cout << "KEY_EVENT: Down was pressed. \n";
			#endif
			if( ImageBox.is_loaded() )
				{
				if ( (v_scroller->get_value() + increment) <=
					(ImageBox.get_image_height() - v_scroller->get_page_size() ) )
						v_scroller->set_value( v_scroller->get_value()+increment );
				else // add the missing bit
					v_scroller->set_value( 	ImageBox.get_image_height() -
										v_scroller->get_page_size() );
				}
			break;
			
		case GDK_KP_Up:
		case GDK_Up:
		case GDK_uparrow:
			#ifdef DEBUG
			std::cout << "KEY_EVENT: Up was pressed. \n";
			#endif
			if( ImageBox.is_loaded() )
				{
				if( v_scroller->get_value() - increment >= 0 )
					v_scroller->set_value( v_scroller->get_value()-increment );
				else // subtract the missing bit
					v_scroller->set_value(0);
				}
			break; //*/
			
		case GDK_s:
		case GDK_S:
			#ifdef DEBUG
			std::cout << "KEY_EVENT: S was pressed. \n";
			#endif
			if( Button_Save.is_sensitive() )
				Button_Save.activate();
			break;
			
		case GDK_1:
		case GDK_KP_1:
			#ifdef DEBUG
			std::cout << "KEY_EVENT: 1 was pressed. \n";
			#endif
			if( Button_Zoom1to1.is_sensitive() )
				Button_Zoom1to1.activate();
			break;
			
		case GDK_A:
		case GDK_a:
			#ifdef DEBUG
			std::cout << "KEY_EVENT: A was pressed. \n";
			#endif
			if( Button_RotateAntiClockwise.is_sensitive() )
				Button_RotateAntiClockwise.activate();
			break;
			
		case GDK_c:
		case GDK_C:
			#ifdef DEBUG
			std::cout << "KEY_EVENT: C was pressed. \n";
			#endif
			if( Button_RotateClockwise.is_sensitive() )
				Button_RotateClockwise.activate();
			break;

		case GDK_p:
		case GDK_P:
			#ifdef DEBUG
			std::cout << "KEY_EVENT: P was pressed. \n";
			#endif
			break; // no printing yet!

		case GDK_f:
		case GDK_F:
			#ifdef DEBUG
			std::cout << "KEY_EVENT: F was pressed. \n";
			#endif
			ToggleButton_ShowFileChooser.get_active() ? 
			    ToggleButton_ShowFileChooser.set_active( false ) : ToggleButton_ShowFileChooser.set_active( true );
			break;

		// i find this confusing, not sure what other people think!
		case GDK_h:
		case GDK_H:
			#ifdef DEBUG
			std::cout << "KEY_EVENT: H was pressed. \n";
			#endif
			NavButtonHBox.is_visible() ? NavButtonHBox.hide() : NavButtonHBox.show();
			break; // */
		
		default:
			return false;
			break;
		}
	return true;
	}


// loads the configuration file $HOME/.gimmagerc
void AppWindow::load_config(void)
	{
	FILE * config;
	w_width = 0;
	w_height = 0;
	v_width = 0;
	v_height = 0;

	char line[100];
	Glib::ustring home;

	Glib::ustring configfile;

	if( (home = Glib::get_home_dir() ) == "" )
		std::cerr << GT( "LOAD_CONFIG: Failed to find home directory" ) << std::endl;
	else
		configfile = home + (Glib::ustring)  "/.gimmagerc";
		
	
	if( (config = fopen(configfile.c_str(),"r")) == NULL )
		{
		std::cerr << GT( "LOAD_CONFIG: Failed to open config file! (~/.gimmagerc)\nWill create on exit." ) << std::endl;
		w_width = 640;
		w_height = 480;
		v_width = 620;
		v_height = 420;
		}
	else
		{
		if( fgets( line, 99, config ) != NULL )
			{
			sscanf(line,"w_width=%d",&w_width);
			// blank out the line
			for(int i = 0; i<100; i++) { line[i] = '\0'; }
			}
			
		if( fgets( line, 99, config ) != NULL )
			{
			sscanf(line,"w_height=%d",&w_height);
			for(int i = 0; i<100; i++) { line[i] = '\0'; }
			}
			
		if( fgets( line, 99, config ) != NULL )
			{
			sscanf(line,"v_width=%d",&v_width);
			for(int i = 0; i<100; i++) { line[i] = '\0'; }
			}
			
		if( fgets( line, 99, config ) != NULL )
			{
			sscanf(line,"v_height=%d",&v_height);
			for(int i = 0; i<100; i++) { line[i] = '\0'; }
			}

		if( v_height == 0 )
			v_height = 420;
		if( v_width == 0 )
			v_width = 420;
		if( w_height == 0 )
			w_height = 480;
		if( w_width == 0)
			w_width = 640;

		fclose(config);
		}			
		
	}


// save the configuration file
void AppWindow::save_config(void)
	{
	#ifdef DEBUG
	std::cout << GT( "SAVE_CONFIG: Saving config file to $HOME/.gimmagerc\n" );
	#endif
	
	FILE * config;

	Glib::ustring home;
	Glib::ustring configfile;

	if( (home = Glib::get_home_dir() ) == "" )
		std::cerr << GT( "SAVE_CONFIG: Failed to find home directory" ) << std::endl;
	else
		configfile = home + (Glib::ustring) "/.gimmagerc";
	
	if( (config = fopen(configfile.c_str(),"w")) == NULL )
		{
		std::cerr << GT( "SAVE_CONFIG: Failed to write config file." ) << std::endl;
		}
	else
		{
		// alow the filechooser to hide before saving the sizes
		FileChooser.hide();
		while(Gtk::Main::events_pending()) Gtk::Main::iteration();
		
		w_width = get_width();
		w_height = get_height();
		v_width = (int)h_scroller->get_page_size();
		v_height = (int)v_scroller->get_page_size();

		if( v_height == 0 )
			v_height = 420;
		if( v_width == 0 )
			v_width = 420;
		if( w_height == 0 )
			w_height = 480;
		if( w_width == 0)
			w_width = 640;
		
		fprintf(config,"w_width=%d\nw_height=%d\nv_width=%d\nv_height=%d\n",
						w_width, w_height, v_width, v_height);
		fclose(config);
		}
	}

// when resizing the window, fit the image to screen
void AppWindow::on_resize( void )
	{
	while(Gtk::Main::events_pending()) Gtk::Main::iteration(); // makes it more fluid
	ImageBox.ScaleImage2((int)h_scroller->get_page_size(),
						(int)v_scroller->get_page_size(),&scalefactor);
	}

void AppWindow::on_mouse_wheel_up(void)	{}
void AppWindow::on_mouse_wheel_down(void) {}
void AppWindow::on_right_click(void) {}
void AppWindow::on_left_click(void) {}
void AppWindow::on_spinbutton_digits_changed(void) {}
