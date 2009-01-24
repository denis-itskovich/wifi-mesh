/**
 * \file MeshViewCanvas.h
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * \date 13/01/2009
 * \author Denis Itskovich
 */


#ifndef MESHVIEWCANVAS_H_
#define MESHVIEWCANVAS_H_

#include "MeshViewStations.h"
#include "MeshGraphItemStation.h"

class MeshGraphics : public QGraphicsView
{
	Q_OBJECT
public:
	MeshGraphics(QWidget* parent = 0);
	void addItem(MeshGraphItemStation* item);
	void removeItem(MeshGraphItemStation* item);

signals:
	void doubleClicked(QPointF pos);

protected:
	virtual void keyPressEvent(QKeyEvent* event);
    virtual void wheelEvent(QWheelEvent* event);
    virtual void mouseDoubleClickEvent(QMouseEvent* event);
    virtual void drawBackground(QPainter *painter, const QRectF &rect);
};

class MeshViewStationsGraph : public MeshViewStations
{
	Q_OBJECT
public:
	MeshViewStationsGraph(QWidget* parent = 0);
	virtual ~MeshViewStationsGraph();

	virtual void setDocument(MeshDocument* document);

signals:
	void addStation(Location loc);

protected:
	virtual void addStation(Station* pStation);
	virtual void removeStation(Station* pStation);
	virtual void setCurrent(Station* pStation);

private slots:
	void addStation(QPointF pos);
	void updateWorldSize();

private:
	void init();
	MeshGraphItemStation* findItem(Station* pStation) const;
	MeshGraphItemStation* currentItem() const;

	MeshGraphics*	m_graphStations;
	QGraphicsItem*	m_resizeGaps[4];
};

#endif /* MESHVIEWCANVAS_H_ */
