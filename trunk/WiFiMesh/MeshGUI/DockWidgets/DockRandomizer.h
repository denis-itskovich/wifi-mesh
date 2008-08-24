/*
 * Randomizer.h
 *
 *  Created on: 21/08/2008
 *      Author: denis
 */

#ifndef MESHDOCKRANDOMIZER_H_
#define MESHDOCKRANDOMIZER_H_

#include "DockFrame.h"

class DockRandomizer : public DockFrame
{
public:
	DockRandomizer(QWidget* parent = 0);
	virtual ~DockRandomizer();

private:
	void init();

	QSlider*			m_sliderStationsCount;
	QSpinBox*			m_spinStationsCount;

	QDoubleSpinBox*		m_spinMinVelocity;
	QDoubleSpinBox*		m_spinMaxVelocity;

	QSpinBox*			m_spinMinDataAmount;
	QSpinBox*			m_spinMaxDataAmount;

	QPushButton*		m_buttonGenerate;

};

#endif /* MESHDOCKRANDOMIZER_H_ */
