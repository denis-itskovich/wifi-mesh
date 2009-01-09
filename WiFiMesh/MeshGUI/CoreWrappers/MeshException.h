/**
 * \file MeshException.h
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * \date 07/01/2009
 * \author Denis Itskovich
 */


#ifndef MESHEXCEPTION_H_
#define MESHEXCEPTION_H_

#include <exception>
#include <string>
#include <cstdarg>

class MeshException : public std::exception
{
public:
	MeshException(const char* fmt, ...)
	{
		char buff[1024];
		va_list args;
		va_start(args, fmt);
		vsprintf(buff, fmt, args);
		va_end(args);
		m_msg = buff;
	}

	virtual const char* what() const throw() { return m_msg.c_str(); }
	virtual ~MeshException() throw() {}

private:
	std::string m_msg;
};

#endif /* MESHEXCEPTION_H_ */
