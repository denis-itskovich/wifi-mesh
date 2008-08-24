/*
 * StationProperties.h
 *
 *  Created on: 22/08/2008
 *      Author: denis
 */

#ifndef MESHDOCKSTATIONPROPERTIES_H_
#define MESHDOCKSTATIONPROPERTIES_H_

#include "DockFrame.h"

class DockStationProperties : public DockFrame
{
public:
	DockStationProperties(QWidget* parent = 0);
	virtual ~DockStationProperties();

private:
	void init();

	QSpinBox*		m_spinRow;
	QSpinBox*		m_spinCol;

	QDoubleSpinBox*	m_spinVelocity;
	QDoubleSpinBox*	m_spinAngle;
	QDial*			m_dialAngle;

};

#endif /* MESHDOCKSTATIONPROPERTIES_H_ */
