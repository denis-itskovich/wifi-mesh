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
 * AMeshDock.cpp
 *
 *  Created on: 21/08/2008
 *      Author: denis
 */

#include "MeshView.h"

MeshView::MeshView(QWidget* parent) :
	QWidget(parent),
	m_document(NULL)
{
	init();
}

MeshDocument* MeshView::document() const
{
	return m_document;
}

void MeshView::setDocument(MeshDocument* doc)
{
	m_document = doc;
	connect(doc, SIGNAL(updateViews()), this, SLOT(updateView()));
}

void MeshView::init()
{
}

void MeshView::enable()
{
	setEnabled(true);
}

void MeshView::disable()
{
	setEnabled(false);
}
