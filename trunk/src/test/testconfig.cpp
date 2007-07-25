// testpp.cpp

#include "../Config.h"

#include <gtkmm/main.h>
#include <gtkmm/window.h>

int main(int argc,char **argv)
	{	
	Glib::init();
	Glib::set_prgname("testconfig");
	Glib::set_application_name("testconfig");
	Gtk::Main kit(argc,argv);

	CConfig Config;
	Config.load();
	
	return 0;
	}
