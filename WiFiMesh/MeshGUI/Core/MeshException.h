/*********************************************************************************
WiFi Mesh Simulator PRO
Copyright (C) 2009 Denis Itskovich

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*********************************************************************************/

/**
 * @file MeshException.h
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * @date 07/01/2009
 * @author Denis Itskovich
 */


#ifndef MESHEXCEPTION_H_
#define MESHEXCEPTION_H_

#include <exception>
#include <string>

class MeshException : public std::exception
{
public:
	MeshException(EStatus status, const char* filename, int linenum) :
	    m_msg(StatusGetMessage(status)),
	    m_filename(filename),
	    m_linenum(linenum)
	    {

	    }

	virtual const char* what() const throw() { return m_msg.c_str(); }
	virtual ~MeshException() throw() {}

private:
	std::string    m_msg;
	std::string    m_filename;
	int            m_linenum;
};

#endif /* MESHEXCEPTION_H_ */
