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

/*
 * StationProperties.h
 *
 *  Created on: 22/08/2008
 *      Author: denis
 */

#ifndef MESHDOCKSTATIONPROPERTIES_H_
#define MESHDOCKSTATIONPROPERTIES_H_

#include "MeshView.h"

class MeshViewStationProperties : public MeshView
{
	Q_OBJECT
public:
	MeshViewStationProperties(QWidget* parent = 0);

private slots:
	void dialAngleChanged(int angle);
	void spinAngleChanged(int angle);

private:
	void init();

	QSpinBox*		m_spinRow;
	QSpinBox*		m_spinCol;

	QDoubleSpinBox*	m_spinVelocity;
	QSpinBox*		m_spinAngle;
	QDial*			m_dialAngle;

};

#endif /* MESHDOCKSTATIONPROPERTIES_H_ */
