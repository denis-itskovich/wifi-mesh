/*
 * DockTimeLine.h
 *
 *  Created on: 21/10/2008
 *      Author: denis
 */

#ifndef DOCKTIMELINE_H_
#define DOCKTIMELINE_H_

#include "MeshView.h"

class MeshTimeLineGraph : public QGraphicsView
{
	Q_OBJECT
public:
	MeshTimeLineGraph(QWidget* parent = 0);
	void addItem(double event);
	void removeItem(double event);
};

class MeshViewTimeLine : public MeshView
{
public:
	MeshViewTimeLine();
	virtual ~MeshViewTimeLine();

};

#endif /* DOCKTIMELINE_H_ */
