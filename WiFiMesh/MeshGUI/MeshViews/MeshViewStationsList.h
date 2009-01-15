/**
 * \file MeshViewStationsList.h
 *
 *  Created on: 20/08/2008
 *      Author: denis
 */

#ifndef MESHVIEWSTATIONSLIST_H_
#define MESHVIEWSTATIONSLIST_H_

#include "MeshViewStations.h"
#include "MeshTreeItemStation.h"

class MeshViewStationsList : public MeshViewStations
{
	Q_OBJECT
public:
	MeshViewStationsList(QWidget* parent = 0);
	virtual ~MeshViewStationsList();

	virtual void setDocument(MeshDocument* document);

protected:
	virtual void addStation(Station* pStation);
	virtual void setCurrent(Station* pStation);
	virtual void removeStation(Station* pStation);
	virtual void updateStation(Station* pStation);

private slots:
	virtual void currentChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);

private:
	void init();
	MeshTreeItemStation* findItem(Station* pStation);

	QTreeWidget*	m_treeStations;
	QPushButton*	m_buttonAdd;
	QPushButton*	m_buttonRemove;
};

#endif /* MESHVIEWSTATIONSLIST_H_ */
