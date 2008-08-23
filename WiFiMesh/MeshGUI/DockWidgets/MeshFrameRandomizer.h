/*
 * MeshFrameRandomizer.h
 *
 *  Created on: 21/08/2008
 *      Author: denis
 */

#ifndef MESHDOCKRANDOMIZER_H_
#define MESHDOCKRANDOMIZER_H_

#include "MeshFrame.h"

class MeshFrameRandomizer : public MeshFrame
{
public:
	MeshFrameRandomizer(QWidget* parent = 0);
	virtual ~MeshFrameRandomizer();

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
