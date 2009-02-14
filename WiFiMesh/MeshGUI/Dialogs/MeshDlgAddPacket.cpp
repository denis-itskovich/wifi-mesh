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
 * @file MeshDlgAddPacket.cpp
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * @date 07/02/2009
 * @author Denis Itskovich
 */

#include "MeshDlgAddPacket.h"
#include "../Document/MeshDocument.h"

MeshDlgAddPacket::MeshDlgAddPacket(const QList<StationId>& destList, StationId src, QWidget* parent) :
	QDialog(parent)
{
	init();
	for (QList<StationId>::const_iterator i = destList.begin(); i != destList.end(); ++i)
	{
		if (*i != src) m_comboDestination->addItem(QString("Station %1").arg(*i), (unsigned int)*i);
	}
}

void MeshDlgAddPacket::init()
{
	setModal(true);
	setMinimumWidth(minimumWidth() * 2);
	setWindowTitle(tr("New packet"));
	setWindowIcon(QIcon(":/packet.png"));

	QGroupBox* mainGroup = new QGroupBox(tr("Packet parameters"));

	m_buttonAdd = new QPushButton(QIcon(":/accept.png"), tr("Add"));
	connect(m_buttonAdd, SIGNAL(clicked()), this, SLOT(accept()));

	m_buttonCancel = new QPushButton(QIcon(":/cancel.png"), tr("Cancel"));
	connect(m_buttonCancel, SIGNAL(clicked()), this, SLOT(reject()));

	m_comboDestination = new QComboBox;
	m_spinTime = new QDoubleSpinBox;
	m_spinTime->setRange(0.01, 36000);
	m_spinTime->setDecimals(2);
	m_spinTime->setSuffix(" [sec]");

	m_spinSize = new QSpinBox;
	m_spinSize->setRange(1, 65536);
	m_spinSize->setSuffix(" [Bytes]");

	QFormLayout* mainLayout = new QFormLayout;
	mainLayout->addRow(tr("Destination:"), m_comboDestination);
	mainLayout->addRow(tr("Time:"), m_spinTime);
	mainLayout->addRow(tr("Size:"), m_spinSize);
	mainGroup->setLayout(mainLayout);

	QHBoxLayout* buttonsLayout = new QHBoxLayout;
	buttonsLayout->addStretch(99);
	buttonsLayout->addWidget(m_buttonAdd);
	buttonsLayout->addWidget(m_buttonCancel);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(mainGroup);
	layout->addItem(buttonsLayout);

	setLayout(layout);
}

double MeshDlgAddPacket::time() const
{
	return m_spinTime->value();
}

StationId MeshDlgAddPacket::destination() const
{
	return m_comboDestination->itemData(m_comboDestination->currentIndex(), Qt::UserRole).toUInt();
}

unsigned long MeshDlgAddPacket::dataSize() const
{
	return (unsigned long)m_spinSize->value();
}
