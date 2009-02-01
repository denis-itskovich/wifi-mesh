/**
 * \file MeshTreeItemStation.h
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * \date 13/01/2009
 * \author Denis Itskovich
 */


#ifndef MESHTREEITEMSTATION_H_
#define MESHTREEITEMSTATION_H_

#include "MeshItemStation.h"
#include <QtGui>

class MeshTreeItemStation : public MeshItemStation, public QTreeWidgetItem
{
public:
	MeshTreeItemStation(MeshViewStations* pContainer, Station* pStation);

	virtual void updateStation();

private:
	void initLocationNode(QTreeWidgetItem* item);
	void initVelocityNode(QTreeWidgetItem* item);
	void initSchedulerNode(QTreeWidgetItem* item);
	void initRoutingNode(QTreeWidgetItem* item);

	void init();
};

#endif /* MESHTREEITEMSTATION_H_ */
