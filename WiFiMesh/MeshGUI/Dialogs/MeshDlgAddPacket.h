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
 * @file MeshDlgAddPacket.h
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * @date 07/02/2009
 * @author Denis Itskovich
 */


#ifndef MESHDLGADDPACKET_H_
#define MESHDLGADDPACKET_H_

#include <QtGui>
#include "../Core/MeshCore.h"

class MeshDlgAddPacket : public QDialog
{
public:
	MeshDlgAddPacket(const QList<StationId>& destList, StationId src, QWidget* parent = 0);

	double time() const;
	StationId destination() const;
	unsigned long dataSize() const;

private:
	void init();

	QPushButton*	m_buttonAdd;
	QPushButton*	m_buttonCancel;

	QComboBox*		m_comboDestination;
	QDoubleSpinBox*	m_spinTime;
	QSpinBox*		m_spinSize;
};

#endif /* MESHDLGADDPACKET_H_ */
