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
	m_messages = new QTreeWidget;
	m_messages->setColumnCount(9);
	m_messages->setHeaderLabels(QStringList()
			<< tr("Time")
			<< tr("Type")
			<< tr("From")
			<< tr("To")
			<< tr("Source")
			<< tr("Destination")
			<< tr("Arrived to")
			<< tr("Size")
			<< tr("Hops count"));
	setLayout(new QVBoxLayout(this));
	layout()->addWidget(m_messages);
}

void MeshViewSniffer::addMessage(const Message* pMessage, StationId deliveredId)
{
	QTreeWidgetItem* item = createItem(pMessage, deliveredId);
	m_messages->addTopLevelItem(item);
	m_messages->setCurrentItem(item);
}

void MeshViewSniffer::clear()
{
	m_messages->clear();
}

QString MeshViewSniffer::stationId(StationId id)
{
	if (id == INVALID_STATION_ID) return "Invalid";
	if (id == BROADCAST_STATION_ID) return "Broadcast";
	return QString("Station %1").arg(id);
}

QTreeWidgetItem* MeshViewSniffer::createItem(const Message* pMessage, StationId deliveredId)
{
	QStringList columns;
	columns << QString::number(document()->time())
			<< MSG_TYPES[pMessage->type]
			<< stationId(pMessage->originalSrcId)
			<< stationId(pMessage->originalDstId)
			<< stationId(pMessage->transitSrcId)
	        << stationId(pMessage->transitDstId)
	        << stationId(deliveredId)
			<< QString::number(pMessage->size + sizeof(*pMessage))
			<< QString::number(pMessage->nodesCount);

	QTreeWidgetItem* item = new QTreeWidgetItem(columns);
	return item;
}

void MeshViewSniffer::setDocument(MeshDocument* doc)
{
	disconnect(doc);

	connect(doc, SIGNAL(messageDispatched(const Message*, StationId)), this, SLOT(addMessage(const Message*, StationId)));
	connect(doc, SIGNAL(simulationStarted()), this, SLOT(clear()));

	MeshView::setDocument(doc);
}
