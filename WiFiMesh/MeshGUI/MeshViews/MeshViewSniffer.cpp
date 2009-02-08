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

void MeshViewSniffer::addPacket(const Packet* pPacket, StationId deliveredId)
{
	if (!m_visActions[pPacket->type]->isChecked()) return;
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
			<< PKT_TYPES_TEXT[pPacket->type]
			<< stationId(pPacket->originalSrcId)
			<< stationId(pPacket->originalDstId)
			<< stationId(pPacket->transitSrcId)
	        << stationId(pPacket->transitDstId)
	        << stationId(deliveredId)
			<< QString::number(pPacket->size + sizeof(*pPacket))
			<< QString::number(pPacket->nodesCount);

	QTreeWidgetItem* item = new QTreeWidgetItem(columns);
	item->setIcon(0, QIcon(":/packet.png"));
	QBrush brush(PKT_TYPES_COLOR[pPacket->type]);
	for (int i = 0; i < item->columnCount(); ++i) item->setForeground(i, brush);
	return item;
}

void MeshViewSniffer::setDocument(MeshDocument* doc)
{
	disconnect(doc);

	connect(doc, SIGNAL(packetDispatched(const Packet*, StationId)), this, SLOT(addPacket(const Packet*, StationId)));
	connect(doc, SIGNAL(simulationStarted()), this, SLOT(clear()));

	MeshView::setDocument(doc);
}
