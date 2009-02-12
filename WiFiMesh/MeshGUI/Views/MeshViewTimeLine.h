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
 * DockTimeLine.h
 *
 *  Created on: 21/10/2008
 *      Author: denis
 */

#ifndef DOCKTIMELINE_H_
#define DOCKTIMELINE_H_

#include "MeshView.h"

class MeshTimeLineGraph : public QGraphicsView
{
	Q_OBJECT
public:
	MeshTimeLineGraph(QWidget* parent = 0);
	void addItem(double event);
	void removeItem(double event);
};

class MeshViewTimeLine : public MeshView
{
public:
	MeshViewTimeLine();
	virtual ~MeshViewTimeLine();

};

#endif /* DOCKTIMELINE_H_ */
