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

/*
 * .h
 *
 *  Created on: 21/08/2008
 *      Author: denis
 */

#ifndef MESHFRAME_H_
#define MESHFRAME_H_

#include <QtGui/QtGui>
#include "../Document/MeshDocument.h"

class MeshView : public QWidget
{
	Q_OBJECT
public:
	virtual void setDocument(MeshDocument* document);

protected:
	MeshView(QWidget* parent = 0);

	MeshDocument* document() const;

protected slots:
	virtual void updateView() {}
	virtual void disable();
	virtual void enable();

private:
	void init();

	MeshDocument* 	m_document;
};

#endif /* MESHFRAME_H_ */
