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
 * @file MeshViewStationsList.h
 *
 *  Created on: 20/08/2008
 *      Author: denis
 */

#ifndef MESHVIEWSTATIONSLIST_H_
#define MESHVIEWSTATIONSLIST_H_

#include "MeshViewStations.h"
#include "Items/MeshTreeItemStation.h"

class MeshViewStationsList : public MeshViewStations
{
	Q_OBJECT
public:
	MeshViewStationsList(QWidget* parent = 0);

	virtual void setDocument(MeshDocument* document);

protected:
    virtual QPushButton* createButton(QAction* action);
	virtual void addStation(Station* pStation);
	virtual void setCurrent(Station* pStation);
	virtual void removeStation(Station* pStation);

private slots:
	virtual void currentChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);

private:
	void init();
	MeshTreeItemStation* findItem(Station* pStation);
	MeshTreeItemStation* currentItem() const;

	QTreeWidget*	m_treeStations;
	QPushButton*	m_buttonAdd;
	QPushButton*	m_buttonRemove;
	QPushButton*	m_buttonPacket;
};

#endif /* MESHVIEWSTATIONSLIST_H_ */
