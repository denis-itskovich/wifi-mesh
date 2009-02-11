/**
 * \file MeshViewSniffer.h
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * \date 27/01/2009
 * \author Denis Itskovich
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

	QTreeWidget*	m_packets;
	QAction*		m_visActions[ePKT_TYPE_LAST];
};

#endif /* MESHVIEWPACKETSLIST_H_ */
