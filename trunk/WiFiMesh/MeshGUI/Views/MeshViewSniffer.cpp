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
#include "../Core/MeshCore.h"

static const char* PKT_TYPES_TEXT[ePKT_TYPE_LAST] =
{
		"Search request",
		"Search response",
		"Data",
		"Ack"
};

static const QRgb PKT_TYPES_COLOR[ePKT_TYPE_LAST] =
{
		0x000000bf,
		0x0000bf00,
		0x00000000,
		0x007f7f7f
};

MeshViewSniffer::MeshViewSniffer(QWidget* parent) :
	MeshView(parent)
{
	init();
}

void MeshViewSniffer::init()
{
	m_packets = new QTreeWidget;
	m_packets->setMinimumSize(600, 144);
	m_packets->setColumnCount(10);
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
			<< tr("Status"));

	QPushButton* buttonClear = new QPushButton(QIcon(":/clear.png"), tr("&Clear"));
	QPushButton* buttonVisibility = new QPushButton(QIcon(":/filter.png"), tr("&Filter"));

	connect(buttonClear, SIGNAL(clicked()), m_packets, SLOT(clear()));

	QMenu* menuVisibility = new QMenu;

	menuVisibility->addAction(initAction(ePKT_TYPE_DATA, tr("Show &data packets")));
	menuVisibility->addAction(initAction(ePKT_TYPE_ACK, tr("Show &ack packets")));
	menuVisibility->addAction(initAction(ePKT_TYPE_SEARCH_REQUEST, tr("Show search &request packets")));
	menuVisibility->addAction(initAction(ePKT_TYPE_SEARCH_RESPONSE, tr("Show search r&esponse packets")));

	buttonVisibility->setMenu(menuVisibility);

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
	QAction* action = new QAction(title, NULL);
	action->setCheckable(true);
	action->setChecked(true);
	m_visActions[type] = action;
	return action;
}

void MeshViewSniffer::addPacket(const Packet* pPacket, StationId deliveredId, EPacketStatus status)
{
	if (!m_visActions[pPacket->header.type]->isChecked()) return;
	QTreeWidgetItem* item = createItem(pPacket, deliveredId, status);
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

QTreeWidgetItem* MeshViewSniffer::createItem(const Packet* pPacket, StationId deliveredId, EPacketStatus status)
{
	QStringList columns;
	unsigned size;
	PacketGetSize(pPacket, &size);
	columns << QString::number(document()->time())
			<< PKT_TYPES_TEXT[pPacket->header.type]
			<< stationId(pPacket->header.originalSrcId)
			<< stationId(pPacket->header.originalDstId)
			<< stationId(pPacket->header.transitSrcId)
	        << stationId(pPacket->header.transitDstId)
	        << stationId(deliveredId)
			<< QString::number(size)
			<< QString::number(pPacket->header.hopsCount)
            << ((status == ePKT_STATUS_DELIVERED) ? tr("delivered") : tr("collision"));

	QTreeWidgetItem* item = new QTreeWidgetItem(columns);
	item->setIcon(0, QIcon((status == ePKT_STATUS_DELIVERED) ? ":/packet-delivered.png" : ":/packet-broken.png"));
	QBrush brush(PKT_TYPES_COLOR[pPacket->header.type]);
	for (int i = 0; i < item->columnCount(); ++i) item->setForeground(i, brush);

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

	MeshView::setDocument(doc);
}
