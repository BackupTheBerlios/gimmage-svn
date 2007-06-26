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

// gimmage: Param.h
/* 	Parameter class for the Config class */

/* GCC is broken in that it does not support template classes that 
   are defined outside of the header. For this reason, we include 
   Param.cpp directly into Param.h */	

template<class T> 
Param<T>::Param( )
	{
	}

template<class T> 
Param<T>::Param( std::string _name )
	{
	set_name(_name);
	}
	
template<class T> 
Param<T>::~Param()
	{
	}
	
template<class T> 
T Param<T>::get()
	{
	return value;
	}
	
template<class T> 
void Param<T>::set( T _value )
	{
	value = _value;
	}
	
template<class T> 
std::string Param<T>::get_name()
	{
	return name;
	}

template<class T> 
void Param<T>::set_name( std::string _name )
	{
	name = _name;
	}
