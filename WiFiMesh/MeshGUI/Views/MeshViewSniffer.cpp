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
 * @file MeshViewSniffer.cpp
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * @date 27/01/2009
 * @author Denis Itskovich
 */

#include "MeshViewSniffer.h"
#include "../Core/MeshCore.h"
#include "Theme/MeshTheme.h"

MeshViewSniffer::MeshViewSniffer(QWidget* parent) :
	MeshView(parent),
    m_iconSuccess(":/success.png"),
    m_iconFailure(":/failure.png")
{
	init();
}

void MeshViewSniffer::init()
{
	m_packets = new QTreeWidget;
	m_packets->setMinimumSize(600, 144);
	m_packets->setColumnCount(11);
	m_packets->setHeaderLabels(QStringList()
			<< tr("Time")
			<< tr("Type")
			<< tr("From")
			<< tr("To")
			<< tr("Source")
			<< tr("Destination")
			<< tr("Arrived to")
			<< tr("Size")
			<< tr("Hops count")
            << tr("Sequence number")
			<< tr("Status"));

	QPushButton* buttonClear = new QPushButton(QIcon(":/clear.png"), tr("&Clear"));
	QPushButton* buttonVisibility = new QPushButton(QIcon(":/filter.png"), tr("&Options"));

	m_packets->setAllColumnsShowFocus(true);
	connect(buttonClear, SIGNAL(clicked()), m_packets, SLOT(clear()));

	m_menu = new QMenu;

	m_actShowPath = new QAction(tr("Show packet path"), this);
	m_actShowPath->setCheckable(true);
	m_actShowPath->setChecked(true);

	m_actShowCollisions = new QAction(tr("Show collided packets"), this);
	m_actShowCollisions->setCheckable(true);
	m_actShowCollisions->setChecked(true);

	m_actAutoScroll = new QAction(tr("&Auto scroll"), this);
	m_actAutoScroll->setCheckable(true);
	m_actAutoScroll->setChecked(true);

	m_menu->addAction(initAction(ePKT_TYPE_DATA, tr("Show &data packets")));
	m_menu->addAction(initAction(ePKT_TYPE_ACK, tr("Show &ack packets")));
	m_menu->addAction(initAction(ePKT_TYPE_SEARCH_REQUEST, tr("Show search &request packets")));
	m_menu->addAction(initAction(ePKT_TYPE_SEARCH_RESPONSE, tr("Show search r&esponse packets")));
	m_menu->addSeparator();
	m_menu->addAction(m_actShowPath);
	m_menu->addAction(m_actShowCollisions);
	m_menu->addAction(m_actAutoScroll);

	buttonVisibility->setMenu(m_menu);

	QVBoxLayout* layout = new QVBoxLayout;
	QHBoxLayout* buttonsLayout = new QHBoxLayout;

	buttonsLayout->addWidget(buttonVisibility);
	buttonsLayout->addWidget(buttonClear);
	buttonsLayout->addStretch(99);

	layout->addWidget(m_packets);
	layout->addItem(buttonsLayout);

	setLayout(layout);
}

QAction* MeshViewSniffer::initAction(EPacketType type, const QString& title)
{
	QAction* action = new QAction(title, this);
	action->setCheckable(true);
	action->setChecked(true);
	m_actVisibility[type] = action;
	return action;
}

void MeshViewSniffer::addPacket(const Packet* pPacket, StationId deliveredId, EPacketStatus status)
{
    if (!isVisible()) return;
	if (!m_actVisibility[pPacket->header.type]->isChecked()) return;
    if (status == ePKT_STATUS_COLLISION && !m_actShowCollisions->isChecked()) return;

	QTreeWidgetItem* item = createItem(pPacket, deliveredId, status);
	m_packets->addTopLevelItem(item);
	if (m_actAutoScroll->isChecked()) m_packets->scrollToItem(item);
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

QTreeWidgetItem* MeshViewSniffer::createItem(const Packet* pPacket, StationId deliveredId, EPacketStatus status)
{
	QStringList columns;
	unsigned size;
	PacketGetSize(pPacket, &size);
	MeshTheme::ItemDescriptor typeDesc = MeshTheme::packetTypeDescriptor(pPacket->header.type);
	MeshTheme::ItemDescriptor statusDesc = MeshTheme::packetStatusDescriptor(status);

	columns << QString::number(document()->time())
			<< typeDesc.title
			<< stationId(pPacket->header.originalSrcId)
			<< stationId(pPacket->header.originalDstId)
			<< stationId(pPacket->header.transitSrcId)
	        << stationId(pPacket->header.transitDstId)
	        << stationId(deliveredId)
			<< QString::number(size)
			<< QString::number(pPacket->header.hopsCount)
            << QString::number(pPacket->header.sequenceNum)
            << statusDesc.title;

	QTreeWidgetItem* item = new QTreeWidgetItem(columns);
	item->setIcon(0, ((status == ePKT_STATUS_DELIVERED) ? m_iconSuccess : m_iconFailure));
	QBrush brush(typeDesc.color);
	for (int i = 0; i < item->columnCount() - 1; ++i) item->setForeground(i, brush);
	item->setForeground(item->columnCount() - 1, QBrush(statusDesc.color));

	if (!m_actShowPath->isChecked()) return item;
	if (status == ePKT_STATUS_COLLISION) return item;
	if (pPacket->routing.length <= 1) return item;

	for (unsigned i = 0; i < pPacket->routing.length; ++i)
	{
	    QTreeWidgetItem* path = new QTreeWidgetItem(QStringList() << QString("Passed through:") << QString("Station %1").arg(pPacket->routing.path[i]));
	    item->addChild(path);
	}

	return item;
}

void MeshViewSniffer::setDocument(MeshDocument* doc)
{
	disconnect(doc);

	connect(doc, SIGNAL(packetDispatched(const Packet*, StationId, EPacketStatus)), this, SLOT(addPacket(const Packet*, StationId, EPacketStatus)));
	connect(doc, SIGNAL(simulationStarted()), this, SLOT(clear()));
    connect(doc, SIGNAL(simulationCleared()), this, SLOT(clear()));

	MeshView::setDocument(doc);
}

void MeshViewSniffer::contextMenuEvent(QContextMenuEvent* event)
{
    m_menu->exec(event->globalPos());
}
