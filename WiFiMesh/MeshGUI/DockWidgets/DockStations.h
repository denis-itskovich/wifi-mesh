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
	Q_OBJECT
public:
	DockStations(MeshDocument* doc = 0, QWidget* parent = 0);
	virtual ~DockStations();

	virtual void setDocument(MeshDocument* document);

signals:
	void currentChanged(Station* pStation);

private slots:
	void currentChanged(QTreeWidgetItem * current, QTreeWidgetItem * previous);
	void addStation(Station* pStation);
	void setCurrent(Station* pStation);
	void removeStation(Station* pStation);
	void updateStation(Station* pStation);

private:
	typedef QMap<QTreeWidgetItem*, Station*> ItemToStation;

	void init();
	QTreeWidgetItem* findItem(Station* pStation);
	static QString stationName(Station* pStation);
	static StationId stationId(Station* pStation);
	static Location stationLocation(Station* pStation);
	static Velocity stationVelocity(Station* pStation);

	QTreeWidget*	m_treeStations;
	QPushButton*	m_buttonAdd;
	QPushButton*	m_buttonRemove;
	ItemToStation	m_itemToStation;
};

#endif /* MESHDOCKSTATIONS_H_ */
