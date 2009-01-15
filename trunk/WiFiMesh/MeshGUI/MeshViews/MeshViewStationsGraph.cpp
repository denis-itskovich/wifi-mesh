/**
 * \file MeshViewCanvas.cpp
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * \date 13/01/2009
 * \author Denis Itskovich
 */

#include "MeshViewStationsGraph.h"

MeshViewStationsGraph::MeshViewStationsGraph(QWidget *parent) :
	MeshViewStations(parent)
{
	init();
}

MeshViewStationsGraph::~MeshViewStationsGraph()
{

}

void MeshViewStationsGraph::init()
{
	QGraphicsScene* scene = new QGraphicsScene();
	m_graphStations = new MeshGraphics();
	setLayout(new QGridLayout(this));
	layout()->addWidget(m_graphStations);

	scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-200, -200, 400, 400);

	m_graphStations->setScene(scene);
    m_graphStations->setCacheMode(QGraphicsView::CacheBackground);
    m_graphStations->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    m_graphStations->setRenderHint(QPainter::Antialiasing);
    m_graphStations->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    m_graphStations->setResizeAnchor(QGraphicsView::AnchorViewCenter);

    connect(m_graphStations, SIGNAL(doubleClicked(QPointF)), this, SLOT(addStation(QPointF)));
}

MeshGraphItemStation* MeshViewStationsGraph::findItem(Station* pStation)
{
	return dynamic_cast<MeshGraphItemStation*>(MeshViewStations::findItem(pStation));
}

void MeshViewStationsGraph::updateStation(Station *pStation)
{
	MeshGraphItemStation* item = findItem(pStation);
	item->updateStation();
}

void MeshViewStationsGraph::addStation(QPointF pos)
{
	Location loc;
	loc.x = pos.x();
	loc.y = pos.y();
	emit addStation(loc);
}

void MeshViewStationsGraph::addStation(Station *pStation)
{
	MeshGraphItemStation* item = new MeshGraphItemStation(this, pStation);
	registerStation(pStation, item);
	m_graphStations->addItem(item);
}

void MeshViewStationsGraph::setCurrent(Station *pStation)
{
	MeshGraphItemStation* item = findItem(pStation);
	if (!item) return;
	item->setSelected(true);
}

void MeshViewStationsGraph::removeStation(Station *pStation)
{
	m_graphStations->removeItem(findItem(pStation));
	unregisterStation(pStation);
}

void MeshViewStationsGraph::setDocument(MeshDocument *doc)
{
	MeshViewStations::setDocument(doc);
	connect(this, SIGNAL(addStation(Location)), doc, SLOT(addStation(Location)));
}

MeshGraphics::MeshGraphics(QWidget* parent) :
	QGraphicsView(parent)
{
}

void MeshGraphics::wheelEvent(QWheelEvent* event)
{
	qreal scaleFactor = pow(2.0, -event->delta() / 240);
	scale(scaleFactor, scaleFactor);
}

void MeshGraphics::mouseDoubleClickEvent(QMouseEvent* event)
{
	emit doubleClicked(event->posF());
}

void MeshGraphics::keyPressEvent(QKeyEvent* event)
{
	qreal scaleFactor = 1.0;

	switch (event->key())
	{
	case Qt::Key_Plus: scaleFactor = 1.2;
	case Qt::Key_Minus: scaleFactor = 1/1.2;
	default: break;
	}

	if (scaleFactor != 1.0) scale(scaleFactor, scaleFactor);
}

void MeshGraphics::addItem(MeshGraphItemStation* item)
{
	if (item && scene()) scene()->addItem(item);
}

void MeshGraphics::removeItem(MeshGraphItemStation* item)
{
	if (item && scene()) scene()->removeItem(item);
}
