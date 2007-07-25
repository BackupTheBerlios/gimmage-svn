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

// gimmage: Config.h
/* 	Class for configuration managment */

// autotools config.h
#include "../config.h"
#include "defines.h"

#include "glibmm.h"

#include <fstream>
#include <iostream>

class CConfig
{
public:
	CConfig();
	~CConfig();
	
	void save();
	void load();
	
/* 	int get_( void ) { return ; }
	void set_( int param ) {  = param; }
	
	int get_( void ) { return ; }
	void set_( int param ) {  = param; }
	
	int get_( void ) { return ; }
	void set_( int param ) {  = param; }
	
	int get_( void ) { return ; }
	void set_( int param ) {  = param; }
	
	int get_( void ) { return ; }
	void set_( int param ) {  = param; }
	
	int get_( void ) { return ; }
	void set_( int param ) {  = param; }
	
	int get_( void ) { return ; }
	void set_( int param ) {  = param; }

	int get_i_width( void );
	void set_i_width( int );
	
	int get_w_width( void ) { return w_width; }
	int set_w_width( int param ) { w_width = param; } */

private:
	int i_height;
	int i_width;
	int w_height;
	int w_width;
};
