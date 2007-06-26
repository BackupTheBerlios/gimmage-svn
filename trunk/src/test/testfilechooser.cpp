// testfilechooser.cpp

#include <iostream>

#include <gtkmm.h>

extern "C"
{
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
}

class TestChooser : public Gtk::VBox
{
public:
	TestChooser(int argc, char **argv) :
		TestButton( Gtk::Stock::APPLY ),
		HideButton( Gtk::Stock::REMOVE )
		{
		_argc = argc;
		_argv = argv;
		
		TestButton.signal_clicked().connect(sigc::mem_fun(*this,&TestChooser::on_test_button));
		HideButton.signal_clicked().connect(sigc::mem_fun(*this,&TestChooser::on_hide_button));
		
		pack_start(FileChooser);
		Buttons.pack_start(TestButton);
		Buttons.pack_start(HideButton);
		pack_start(Buttons);
		}
	
	
		
	void on_hide_button(void)
		{
		FileChooser.is_visible() ? FileChooser.hide() : FileChooser.show();
		}
	
	void on_test_button(void)
		{
		test( _argc, _argv );
		}
	
	
	void test( int argc, char **argv )
		{
		static int i = 1;
		if( argc > 1 && i < argc && FileChooser.is_visible() )
			{
			if( stat(argv[i], &filemode) != 0 )
				{
				std::cout << "Stat Error!  " << argv[i] << std::endl;
				return;
				}		
						
			Glib::ustring filename;
			// Resolve any ".." in the filename and make the filename absolute
			if( Glib::path_is_absolute( argv[i] ) )
				filename = argv[i] ;
			else
				filename = (Glib::ustring)get_current_dir_name() + "/" + (Glib::ustring)argv[i];
			
			while( filename.find("..") != Glib::ustring::npos )
				{
				filename.erase( 
					filename.rfind( '/', filename.find("..")-2), 
					filename.find("..")+2 - filename.rfind( '/', filename.find("..")-2) );
				}
				
			if(filemode.st_mode & S_IFREG)
				{
				std::cout << "\nselecting: " << filename << std::endl;
				FileChooser.select_filename(filename);
				while(Gtk::Main::events_pending()) Gtk::Main::iteration();
				std::cout << "FileChooser reports: " << FileChooser.get_filename() << std::endl;
				}
			i++;	
			if( i == argc-1 )
			  i = 1;	
			}
		}	

private:
	int _argc;
	char **_argv;

	struct stat filemode;

	Gtk::FileChooserWidget FileChooser;
	Gtk::HButtonBox Buttons;
	Gtk::Button	TestButton;	
	Gtk::Button HideButton;	
};

int main(int argc,char **argv)
	{	
	Glib::init();
	Glib::set_prgname("testfilechooser");
	Glib::set_application_name("testfilechooser");
	Gtk::Main kit(argc,argv);

	Gtk::Window testwindow;
	TestChooser widget(argc,argv);

	testwindow.add(widget);
	testwindow.show_all_children();
	
	testwindow.set_size_request(512,384);
	
	Gtk::Main::run(testwindow);
	
	return 0;
	}
