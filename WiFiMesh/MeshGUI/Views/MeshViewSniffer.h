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
 * @file MeshViewSniffer.h
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * @date 27/01/2009
 * @author Denis Itskovich
 */


#ifndef MESHVIEWPACKETSLIST_H_
#define MESHVIEWPACKETSLIST_H_

#include "MeshView.h"

class MeshViewSniffer : public MeshView
{
	Q_OBJECT
public:
	MeshViewSniffer(QWidget* parent = 0);

	void setDocument(MeshDocument* doc);

protected slots:
	void addPacket(const Packet* pPacket, StationId deliveredId, EPacketStatus status);
	void clear();

private:
	void init();
	static QString stationId(StationId id);
	QTreeWidgetItem* createItem(const Packet* pPacket, StationId deliveredId, EPacketStatus status);
	QAction* initAction(EPacketType type, const QString& title);

	QTreeWidget*   m_packets;
	QAction*       m_visActions[ePKT_TYPE_LAST];
	QAction*       m_showPath;
	QAction*       m_showCollisions;
	QIcon          m_iconSuccess;
	QIcon          m_iconFailure;
};

#endif /* MESHVIEWPACKETSLIST_H_ */
