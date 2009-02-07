/**
 * \file MeshViewSniffer.cpp
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * \date 27/01/2009
 * \author Denis Itskovich
 */

#include "MeshViewSniffer.h"
#include "../CoreWrappers/MeshCore.h"

static const char* MSG_TYPES[] =
{
		"Search request",
		"Search response",
		"Data",
		"Ack"
};

MeshViewSniffer::MeshViewSniffer(QWidget* parent) :
	MeshView(parent)
{
	init();
}

void MeshViewSniffer::init()
{
	m_packets = new QTreeWidget;
	m_packets->setColumnCount(9);
	m_packets->setHeaderLabels(QStringList()
			<< tr("Time")
			<< tr("Type")
			<< tr("From")
			<< tr("To")
			<< tr("Source")
			<< tr("Destination")
			<< tr("Arrived to")
			<< tr("Size")
			<< tr("Hops count"));

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(m_packets);
	setLayout(layout);
}

void MeshViewSniffer::addPacket(const Packet* pPacket, StationId deliveredId)
{
	QTreeWidgetItem* item = createItem(pPacket, deliveredId);
	m_packets->addTopLevelItem(item);
	m_packets->setCurrentItem(item);
}

void MeshViewSniffer::clear()
{
	m_packets->clear();
}

QString MeshViewSniffer::stationId(StationId id)
{
	if (id == INVALID_STATION_ID) return "Invalid";
	if (id == BROADCAST_STATION_ID) return "Broadcast";
	return QString("Station %1").arg(id);
}

QTreeWidgetItem* MeshViewSniffer::createItem(const Packet* pPacket, StationId deliveredId)
{
	QStringList columns;
	columns << QString::number(document()->time())
			<< MSG_TYPES[pPacket->type]
			<< stationId(pPacket->originalSrcId)
			<< stationId(pPacket->originalDstId)
			<< stationId(pPacket->transitSrcId)
	        << stationId(pPacket->transitDstId)
	        << stationId(deliveredId)
			<< QString::number(pPacket->size + sizeof(*pPacket))
			<< QString::number(pPacket->nodesCount);

	QTreeWidgetItem* item = new QTreeWidgetItem(columns);
	return item;
}

void MeshViewSniffer::setDocument(MeshDocument* doc)
{
	disconnect(doc);

	connect(doc, SIGNAL(packetDispatched(const Packet*, StationId)), this, SLOT(addPacket(const Packet*, StationId)));
	connect(doc, SIGNAL(simulationStarted()), this, SLOT(clear()));

	MeshView::setDocument(doc);
}
