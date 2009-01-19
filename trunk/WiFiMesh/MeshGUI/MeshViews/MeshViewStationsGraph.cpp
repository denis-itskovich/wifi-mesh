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
	QLayout* layout = new QHBoxLayout;

	layout->setMargin(0);
	layout->addWidget(m_graphStations);
	setLayout(layout);

	scene->setItemIndexMethod(QGraphicsScene::NoIndex);

	m_graphStations->setScene(scene);
    m_graphStations->setCacheMode(QGraphicsView::CacheBackground);
    m_graphStations->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    m_graphStations->setRenderHint(QPainter::Antialiasing);
    m_graphStations->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    m_graphStations->setResizeAnchor(QGraphicsView::AnchorViewCenter);

    connect(m_graphStations, SIGNAL(doubleClicked(QPointF)), this, SLOT(addStation(QPointF)));
}

MeshGraphItemStation* MeshViewStationsGraph::findItem(Station* pStation) const
{
	return dynamic_cast<MeshGraphItemStation*>(MeshViewStations::findItem(pStation));
}

MeshGraphItemStation* MeshViewStationsGraph::currentItem() const
{
	return dynamic_cast<MeshGraphItemStation*>(MeshViewStations::currentItem());
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
	MeshViewStations::addStation(pStation);
}

void MeshViewStationsGraph::removeStation(Station *pStation)
{
	m_graphStations->removeItem(findItem(pStation));
	MeshViewStations::removeStation(pStation);
}

void MeshViewStationsGraph::setCurrent(Station* pStation)
{
	MeshGraphItemStation* item = findItem(pStation);
	if (item) item->setFocus();
}

void MeshViewStationsGraph::setDocument(MeshDocument *doc)
{
	MeshViewStations::setDocument(doc);
	connect(this, SIGNAL(addStation(Location)), doc, SLOT(addStation(Location)));

	QGraphicsScene* scene = m_graphStations->scene();
	if (scene)
	{
		Size size = document()->worldSize();
	    scene->setSceneRect(-size.x/2.0, -size.y/2.0, size.x, size.y);
	}
}

MeshGraphics::MeshGraphics(QWidget* parent) :
	QGraphicsView(parent)
{
}

void MeshGraphics::wheelEvent(QWheelEvent* event)
{
	qreal scaleFactor = pow(1.2, -event->delta() / 240.0);
	scale(scaleFactor, scaleFactor);
}

void MeshGraphics::mouseDoubleClickEvent(QMouseEvent* event)
{
	emit doubleClicked(mapToScene(event->pos()));
}

void MeshGraphics::keyPressEvent(QKeyEvent* event)
{
	qreal scaleFactor = 1.0;

	switch (event->key())
	{
	case Qt::Key_Plus: scaleFactor = 1.2; break;
	case Qt::Key_Minus: scaleFactor = 1/1.2; break;
	default: break;
	}

	if (scaleFactor != 1.0)
		scale(scaleFactor, scaleFactor);
    QGraphicsView::keyPressEvent(event);
}

void MeshGraphics::addItem(MeshGraphItemStation* item)
{
	if (item && scene()) scene()->addItem(item);
}

void MeshGraphics::removeItem(MeshGraphItemStation* item)
{
	if (item && scene()) scene()->removeItem(item);
}

void MeshGraphics::drawBackground(QPainter *painter, const QRectF &rect)
{
	QRectF sceneRect = this->sceneRect();
	painter->setPen(QColor(Qt::black));
	painter->fillRect(sceneRect, QBrush(QColor(Qt::lightGray)));
	painter->drawRect(sceneRect);
}
