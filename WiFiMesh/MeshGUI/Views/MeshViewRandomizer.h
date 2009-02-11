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

private:
	void init();

	QSlider*           m_sliderStationsCount;
	QSpinBox*          m_spinStationsCount;

	QDoubleSpinBox*    m_spinAvgVelocity;
	QSpinBox*          m_spinAvgDataSize;
	QSpinBox*          m_spinAvgPacketsCount;
	QDoubleSpinBox*    m_spinDuration;

	QPushButton*       m_buttonGenerateWorld;
	QPushButton*       m_buttonGeneratePackets;
};

#endif /* MESHDOCKRANDOMIZER_H_ */
