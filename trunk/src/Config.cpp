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

// gimmage: Config.cpp
/* 	Class for configuration managment */

#include "Config.h"

CConfig::CConfig()
	{
	}
	
CConfig::~CConfig()
	{
	}

void CConfig::load()
	{
	std::list<Glib::ustring> lines;
	
	Glib::ustring homedir = Glib::get_home_dir();
	Glib::ustring filename = ".gimmagerc";
	
	Glib::ustring configfile = Glib::build_filename( homedir, filename );
	
	std::ifstream gimmagerc;
	}
	
void CConfig::save()
	{
	}
