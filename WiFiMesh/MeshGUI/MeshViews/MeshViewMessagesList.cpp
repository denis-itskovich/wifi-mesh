/**
 * \file MeshViewMessagesList.cpp
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * \date 27/01/2009
 * \author Denis Itskovich
 */

#include "MeshViewMessagesList.h"
#include "../CoreWrappers/MeshCore.h"

static const char* MSG_TYPES[] =
{
		"Search request",
		"Search response",
		"Data",
		"Ack"
};

MeshViewMessagesList::MeshViewMessagesList(QWidget* parent) :
	MeshView(parent)
{
	init();
}

void MeshViewMessagesList::init()
{
	m_messages = new QTreeWidget;
	m_messages->setColumnCount(7);
	m_messages->setHeaderLabels(QStringList() << tr("Time") << tr("Type") << tr("From") << tr("To") << tr("Source") << tr("Destination") << tr("Size"));
	setLayout(new QVBoxLayout(this));
	layout()->addWidget(m_messages);
}

void MeshViewMessagesList::addMessage(const Message* pMessage)
{
	m_messages->addTopLevelItem(createItem(pMessage));
}

void MeshViewMessagesList::clear()
{
	m_messages->clear();
}

QTreeWidgetItem* MeshViewMessagesList::createItem(const Message* pMessage)
{
	QStringList columns;
	columns << QString::number(document()->time())
			<< MSG_TYPES[pMessage->type]
			<< QString::number(pMessage->originalSrcId)
			<< QString::number(pMessage->originalDstId)
			<< QString::number(pMessage->transitSrcId)
	        << QString::number(pMessage->transitDstId)
			<< QString::number(pMessage->size + sizeof(*pMessage));

	QTreeWidgetItem* item = new QTreeWidgetItem(columns);
	return item;
}

void MeshViewMessagesList::setDocument(MeshDocument* doc)
{
	disconnect(doc);

	connect(doc, SIGNAL(messageDispatched(const Message*)), this, SLOT(addMessage(const Message*)));
	connect(doc, SIGNAL(simulationStarted()), this, SLOT(clear()));

	MeshView::setDocument(doc);
}
