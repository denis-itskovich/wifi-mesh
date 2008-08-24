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
	Q_OBJECT
public:
	DockStationProperties(QWidget* parent = 0);
	virtual ~DockStationProperties();

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
