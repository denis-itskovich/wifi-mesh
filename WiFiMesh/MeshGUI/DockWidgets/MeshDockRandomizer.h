/*
 * MeshDockRandomizer.h
 *
 *  Created on: 21/08/2008
 *      Author: denis
 */

#ifndef MESHDOCKRANDOMIZER_H_
#define MESHDOCKRANDOMIZER_H_

#include "MeshDock.h"

class MeshDockRandomizer : public MeshDock
{
public:
	MeshDockRandomizer(QWidget* parent = 0);
	virtual ~MeshDockRandomizer();

private:
	void init();

	QSlider*	m_sliderStationsCount;
	QTextLine*	m_textMinVelocity;
	QTextLine*	m_textMaxVelocity;

	QTextLine*	m_textMinDataAmount;
	QTextLine*	m_textMaxDataAmount;

};

#endif /* MESHDOCKRANDOMIZER_H_ */
