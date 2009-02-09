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
    connect(m_graphStations, SIGNAL(focusCleared()), this, SLOT(clearCurrent()));
//    connect(scene, SIGNAL(selectionChanged()), this, SLOT(clearCurrent()));
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
    MeshGraphItemStation* item = findItem(pStation);
	m_graphStations->removeItem(item);
	MeshViewStations::removeStation(pStation);
	delete item;
}

void MeshViewStationsGraph::setCurrent(Station* pStation)
{
	if (currentItem()) currentItem()->clearFocus();
    MeshViewStations::setCurrent(pStation);
    if (currentItem()) currentItem()->setFocus();
}

void MeshViewStationsGraph::setDocument(MeshDocument *doc)
{
	MeshViewStations::setDocument(doc);

	connect(this, SIGNAL(addStation(Location)), doc, SLOT(addStation(Location)));
	connect(doc, SIGNAL(worldSizeChanged()), this, SLOT(updateWorldSize()));
	connect(doc, SIGNAL(beginTransmit(const Station*, const Station*, const Packet*)), this, SLOT(beginTransmit(const Station*, const Station*, const Packet*)));
    connect(doc, SIGNAL(endTransmit(const Station*)), this, SLOT(endTransmit(const Station*)));

	updateWorldSize();
}

void MeshViewStationsGraph::beginTransmit(const Station* pSrc, const Station* pDst, const Packet* pPacket)
{
    MeshGraphItemStation* srcItem = findItem((Station*)pSrc);
    MeshGraphItemStation* dstItem = findItem((Station*)pDst);
    MeshGraphItemLink* link = new MeshGraphItemLink(srcItem, dstItem, pPacket);
    srcItem->beginTransmit();
    m_srcToLink.insert(pSrc, link);
    m_dstToLink.insert(pDst, link);
    m_graphStations->addItem(link);
}

void MeshViewStationsGraph::endTransmit(const Station* pDst)
{
    assert(m_dstToLink.count(pDst) == 1);
    if (!m_dstToLink.count(pDst)) return;
    MeshGraphItemLink* link = m_dstToLink.value(pDst);
    const Station* pSrc = m_srcToLink.key(link);
    assert(m_srcToLink.count(pSrc) != 0);

    findItem((Station*)pSrc)->endTransmit();
    m_graphStations->removeItem(link);
    m_srcToLink.remove(pSrc, link);
    m_dstToLink.remove(pDst, link);
    delete link;
}

void MeshViewStationsGraph::updateLinks(const LinkList& links)
{
    for (LinkList::const_iterator i = links.begin(); i != links.end(); ++i)
    {
        (*i)->updateLink();
    }
}

void MeshViewStationsGraph::updateStation(Station* pStation)
{
    MeshViewStations::updateStation(pStation);
    if (m_srcToLink.count(pStation)) updateLinks(m_srcToLink.values(pStation));
    if (m_dstToLink.count(pStation)) updateLinks(m_dstToLink.values(pStation));
}

void MeshViewStationsGraph::clearCurrent()
{
    currentChanged((Station*)NULL);
}

void MeshViewStationsGraph::updateWorldSize()
{
	QGraphicsScene* scene = m_graphStations->scene();
	if (scene)
	{
		Size size = document()->worldSize();
	    scene->setSceneRect(-size.x/2.0, -size.y/2.0, size.x, size.y);
		scene->update();
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

void MeshGraphics::mousePressEvent(QMouseEvent* event)
{
    if (itemAt(event->pos()) == NULL) emit focusCleared();
    QGraphicsView::mousePressEvent(event);
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

void MeshGraphics::addItem(QGraphicsItem* item)
{
	if (item && scene()) scene()->addItem(item);
}

void MeshGraphics::removeItem(QGraphicsItem* item)
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
