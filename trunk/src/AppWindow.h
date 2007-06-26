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

// gimmage - AppWindow.h

#include <gtkmm/main.h>
#include <gtkmm/paned.h>
#include <gtkmm/window.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/filechooserwidget.h>
#include <gtkmm/box.h>
#include <gtkmm/dialog.h>
#include <gtkmm/button.h>
#include <gtkmm/togglebutton.h>
#include <gtkmm/tooltips.h>
#include <gtkmm/alignment.h>
#include <gtkmm/adjustment.h>
#include <gtkmm/scrollbar.h>
#include <gdkmm/cursor.h>
#include <gtkmm/stockid.h>
#include <gdkmm/cursor.h>

#include "ImageEventBox.h"
#include "FileManager.h"

#include "Preview.h"

#include "Print.h"

#include "SaveDialog.h"
#include "SaveErrorDialog.h"

// the AppWindow class for the image viewer gimmage

extern Glib::ustring iconpath;


class AppWindow : public Gtk::Window
	{
	public:
		AppWindow(int argnum = 0, char **argcon = NULL);
		virtual ~AppWindow();

		
	protected:
		// these will take the command line options, otherwise we can't access them
		// from our methods
		int argc;
		char **argv;

		// these are parsed from the config file by load_config and saved at exit by save_config
		int w_width;
		int w_height;
		int v_width;
		int v_height;

	// Child Widgets
		// toplevel VBox
		Gtk::VBox VBox_Top;
		Gtk::VPaned	VPaned;

		Gtk::Adjustment * h_scroller;
		Gtk::Adjustment * v_scroller;
		Gtk::ScrolledWindow ImageScroller;
		ImageEventBox ImageBox;
	
	// filechooser to let the user choose files!
		Gtk::FileChooserWidget FileChooser;
		CPreview Preview;
		Gtk::FileFilter	ImageFilter;
		FileManager ImageManager;

		// The dialog appears when the user wants to save a file.
		CSaveDialog		SaveDialog;
		// appears when saving fails for some reason
		CSaveErrorDialog SaveErrorDialog;
	
		// navigation buttons
		Gtk::Alignment NavButtonHBox_Alignment;
		// we need to use an extra hbox to do what we want! A ButtonBox does not cut it!
		Gtk::HBox NavButtonHBox_Alignment_HBox; 
		Gtk::HBox NavButtonHBox;
		Gtk::Button Button_Previous;
		Gtk::Button Button_Next;
		Gtk::Button Button_ZoomIn;
		Gtk::Button Button_ZoomOut;
		Gtk::Button Button_Zoom1to1;
		Gtk::Button Button_RotateAntiClockwise;
		Gtk::Button Button_RotateClockwise;
		Gtk::Button Button_Save;
		Gtk::Button Button_Print;
		Gtk::Button Button_Quit;
		Gtk::ToggleButton ToggleButton_ShowFileChooser;
	
		// image objects to be used as stock button labels
		Gtk::Image I_Button_Previous;
		Gtk::Image I_Button_Next;
		Gtk::Image I_Button_ZoomIn;
		Gtk::Image I_Button_ZoomOut;
		Gtk::Image I_Button_Zoom1to1;
		Gtk::Image I_Button_RotateAntiClockwise;
		Gtk::Image I_Button_RotateClockwise;
		Gtk::Image I_Button_Save;
		Gtk::Image I_Button_Print;
		Gtk::Image I_Button_Quit;
		Gtk::Image I_ToggleButton_ShowFileChooser;

		// Tooltips for buttons.
		Gtk::Tooltips Button_Tooltips;
	
		// This will serve as the central variable to determine the scale factor of the image being displayed
		double scalefactor;
		void set_scalefactor(double scale);

		// this activates or deactivates all image operation buttons (zoom, rotate..)
		virtual void set_buttons_active(bool active);

		void load_config(void);
		void save_config(void);

		// set the correct directory in the filechooser after loading an image
		void set_filechooser_dir(void);

		// indicate we're busy
		void busy(bool);
		Gdk::Cursor Hand, Watch;
		
		// printing system	
		void print(Gtk::PrintOperationAction print_action);
		Glib::RefPtr<Gtk::PageSetup> refPageSetup;
		Glib::RefPtr<Gtk::PrintSettings> refPrintSettings;
	
	// Event Handlers
	   		
		virtual void on_button_next(void);
		virtual void on_button_previous(void);
		
		virtual void on_button_zoom_in(void);
		virtual void on_button_zoom_out(void);
		virtual void on_button_zoom_1to1(void);
		virtual void adjust_adjustment_on_zoom(double oldscale);
		
		virtual void on_button_rotate_anticlockwise(void);
		virtual void on_button_rotate_clockwise(void);
		virtual void adjust_adjustment_on_rotate(int angle,double h_old, double v_old);

		virtual void on_button_save(void);
		virtual void on_save_dialog_response(int response);
		virtual void on_save_error_dialog_response(int response);
		
		virtual void on_button_print(void);
		
		virtual void on_button_show_filechooser(void);

		virtual void on_button_quit(void);

		virtual bool on_button1_pressed_motion(GdkEventMotion*);
		virtual bool on_button_press_event(GdkEventButton*);
		virtual bool on_button_release_event(GdkEventButton*);
		
		virtual void on_spinbutton_digits_changed(void);
	
		virtual void on_mouse_wheel_up(void);
		virtual void on_mouse_wheel_down(void);
		virtual void on_right_click(void);
		virtual void on_left_click(void);

		virtual void on_resize(void);
		
		virtual bool on_delete_event(GdkEventAny*);

		// the following two ints are important for the drag scrolling
		gdouble dragoldx, dragoldy;
		virtual void on_drag_data_received(const Glib::RefPtr<Gdk::DragContext>&, int, int, const Gtk::SelectionData&, guint, guint);

		// load the file when we activate it in the filechooser
		virtual void on_file_activated(void);
		virtual void open_new_file( Glib::ustring );
		virtual void on_update_preview(void);

		// let's catch some key presses in order to make the app key-operable
		virtual bool on_key_press_event(GdkEventKey * key);
	};
