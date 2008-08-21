/*
 * MeshDockStations.h
 *
 *  Created on: 20/08/2008
 *      Author: denis
 */

#ifndef MESHDOCKSTATIONS_H_
#define MESHDOCKSTATIONS_H_

#include "MeshDock.h"

class MeshDockStations : public MeshDock
{
public:
	MeshDockStations(QWidget* parent = 0);
	virtual ~MeshDockStations();

	void addStation(MeshStation* station);
	void clear();

private:
	void init();

	QListWidget*	m_listWidget;
};

#endif /* MESHDOCKSTATIONS_H_ */
