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
 * Randomizer.h
 *
 *  Created on: 21/08/2008
 *      Author: denis
 */

#ifndef MESHDOCKRANDOMIZER_H_
#define MESHDOCKRANDOMIZER_H_

#include "MeshView.h"

class MeshViewRandomizer : public MeshView
{
	Q_OBJECT
public:
	MeshViewRandomizer(QWidget* parent = 0);
	virtual ~MeshViewRandomizer();

	virtual void setDocument(MeshDocument* document);

private slots:
    void setPathLoss(const QString& filename);
    void browse();

private:
	void init();
	void updateView();

    QSpinBox*           m_spinStationCount;
    QSpinBox*           m_spinRandomSeed;
    QDoubleSpinBox*     m_spinAvgVelocity;
    QSpinBox*           m_spinAvgDataSize;
    QSpinBox*           m_spinAvgPacketCount;
    QDoubleSpinBox*     m_spinDuration;
    QLineEdit*          m_editPathLossFile;
    QAbstractButton*    m_buttonBrowse;
    QLabel*             m_labelFlag;

    QPushButton*        m_buttonGenerateWorld;
    QPushButton*        m_buttonGeneratePackets;
};

#endif /* MESHDOCKRANDOMIZER_H_ */
