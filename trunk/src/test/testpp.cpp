// testpp.cpp

#include "../PrintPreviewWidget.h"

#include <gtkmm/main.h>
#include <gtkmm/window.h>

int main(int argc,char **argv)
	{	
	Glib::init();
	Glib::set_prgname("testpp");
	Glib::set_application_name("testpp");
	Gtk::Main kit(argc,argv);

	Gtk::Window testwindow;
	CPrintPreviewWidget PPreview;

	testwindow.add(PPreview);
	testwindow.show_all_children();
	
	Gtk::Main::run(testwindow);
	
	return 0;
	}
