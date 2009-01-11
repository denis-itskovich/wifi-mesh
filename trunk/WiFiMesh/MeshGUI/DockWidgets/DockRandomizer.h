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
	Q_OBJECT
public:
	DockRandomizer(MeshDocument* document = 0, QWidget* parent = 0);
	virtual ~DockRandomizer();

	virtual void setDocument(MeshDocument* document);

private:
	void init();

	QSlider*			m_sliderStationsCount;
	QSpinBox*			m_spinStationsCount;

	QDoubleSpinBox*		m_spinAvgVelocity;
	QSpinBox*			m_spinAvgDataSize;
	QSpinBox*			m_spinAvgMessagesCount;

	QPushButton*		m_buttonGenerate;
};

#endif /* MESHDOCKRANDOMIZER_H_ */
