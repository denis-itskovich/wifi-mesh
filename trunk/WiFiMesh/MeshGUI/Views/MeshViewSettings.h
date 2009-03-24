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
 * @file MeshViewSettings.h
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * @date 11/01/2009
 * @author Denis Itskovich
 */

#ifndef DOCKSETTINGS_H_
#define DOCKSETTINGS_H_

#include "MeshView.h"

class MeshViewSettings : public MeshView
{
	Q_OBJECT
public:
	MeshViewSettings(QWidget* parent = 0);
	virtual ~MeshViewSettings();

	virtual void setDocument(MeshDocument* doc);

signals:
	void updateDataRate(int dataRate);
	void updateSize(Size size);

private slots:
    void updateView();
	void setDataRate(double dataRate);
	void setUnits(int units);
	void setHeight(int height);
	void setWidth(int width);

	void setCoverage(double coverage);
	void setMaxAttenuation(double atten);
	void setAttenuationCoefficient(double coef);

private:
	void init();
	int dataRate();

    QDoubleSpinBox*	    m_spinCoverage;
    QDoubleSpinBox*     m_spinAttenuationCoefficient;
    QDoubleSpinBox*     m_spinMaximumAttenuation;
    QDoubleSpinBox*     m_spinRouteExpTimeout;
    QDoubleSpinBox*     m_spinRouteRetryTimeout;
    QSpinBox*           m_spinRouteRetryThreshold;
    QSpinBox*           m_spinRoutingTableSize;
    QDoubleSpinBox*     m_spinPacketRetryTimeout;
    QSpinBox*           m_spinPacketRetryThreshold;
    QSpinBox*           m_spinPacketHopsThreshold;
    QSpinBox*           m_spinRelayBufferSize;
    QDoubleSpinBox*     m_spinDataRate;
    QComboBox*          m_comboDataUnits;
    QCheckBox*          m_checkSmoothenEvents;
    QSpinBox*           m_spinHeight;
    QSpinBox*           m_spinWidth;
    int                 m_dataRate;
};

#endif /* DOCKSETTINGS_H_ */
