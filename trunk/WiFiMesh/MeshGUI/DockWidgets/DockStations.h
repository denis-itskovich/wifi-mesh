/*
 * Stations.h
 *
 *  Created on: 20/08/2008
 *      Author: denis
 */

#ifndef MESHDOCKSTATIONS_H_
#define MESHDOCKSTATIONS_H_

#include "DockFrame.h"

class DockStations : public DockFrame
{
public:
	DockStations(QWidget* parent = 0);
	virtual ~DockStations();

	void addStation(MeshStation* station);
	void clear();

private:
	void init();

	QListWidget*	m_listWidget;
	QPushButton*	m_buttonAdd;
	QPushButton*	m_buttonRemove;
	QPushButton*	m_buttonProperties;
};

#endif /* MESHDOCKSTATIONS_H_ */
