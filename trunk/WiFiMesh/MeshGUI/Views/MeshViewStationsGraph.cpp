/*********************************************************************************
WiFi Mesh Simulator PRO
Copyright (C) 2009 Denis Itskovich

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*********************************************************************************/

/**
 * @file MeshViewCanvas.cpp
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * @date 13/01/2009
 * @author Denis Itskovich
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

	QMenu* menu = new QMenu;
	menu->addAction(m_actAddPacket);
	menu->addAction(m_actAddStation);
	menu->addAction(m_actRemoveStation);

	m_graphStations->setMenu(menu);
	m_graphStations->setScene(scene);
    m_graphStations->setCacheMode(QGraphicsView::CacheBackground);
    m_graphStations->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    m_graphStations->setRenderHint(QPainter::Antialiasing);
    m_graphStations->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    m_graphStations->setResizeAnchor(QGraphicsView::AnchorViewCenter);

    connect(m_graphStations, SIGNAL(doubleClicked(QPointF)), this, SLOT(addStation(QPointF)));
    connect(m_graphStations, SIGNAL(focusCleared()), this, SLOT(clearCurrent()));
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
	emit MeshViewStations::addStation(loc);
}

void MeshViewStationsGraph::addStation(Station *pStation)
{
	MeshGraphItemStation* item = new MeshGraphItemStation(this, pStation);
	registerStation(pStation, item);
	m_graphStations->addItem(item);
	MeshViewStations::addStation(pStation);
}

void MeshViewStationsGraph::addStationTriggered()
{
    QPointF pos = m_graphStations->pos();
    Location loc = { pos.x(), pos.y() };
    emit MeshViewStations::addStation(loc);
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

	connect(doc, SIGNAL(worldSizeChanged()), this, SLOT(updateWorldSize()));
	updateWorldSize();
}

void MeshViewStationsGraph::beginTransmit(const Station* pSrc, const Station* pDst, const Packet* pPacket)
{
    MeshGraphItemStation* srcItem = findItem((Station*)pSrc);
    MeshGraphItemStation* dstItem = findItem((Station*)pDst);
    srcItem->updateStation();
    dstItem->updateStation();
    MeshGraphItemLink* link = new MeshGraphItemLink(srcItem, dstItem, pPacket);
    m_srcToLink.insert(pSrc, link);
    m_dstToLink.insert(pDst, link);
    m_graphStations->addItem(link);
    MeshViewStations::beginTransmit(pSrc, pDst, pPacket);
}

void MeshViewStationsGraph::endTransmit(const Station* pDst)
{
    MeshViewStations::endTransmit(pDst);
    assert(m_dstToLink.count(pDst) == 1);
    if (!m_dstToLink.count(pDst)) return;
    MeshGraphItemLink* link = m_dstToLink.value(pDst);
    const Station* pSrc = m_srcToLink.key(link);
    assert(m_srcToLink.count(pSrc) != 0);
    m_graphStations->removeItem(link);
    m_srcToLink.remove(pSrc, link);
    m_dstToLink.remove(pDst, link);
    delete link;
    findItem((Station*)pSrc)->updateStation();
    findItem((Station*)pDst)->updateStation();
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

MeshGraphics::~MeshGraphics()
{
    delete m_menu;
}

QPointF MeshGraphics::pos() const
{
    return m_pos;
}

void MeshGraphics::setMenu(QMenu* menu)
{
    m_menu = menu;
}

void MeshGraphics::contextMenuEvent(QContextMenuEvent* event)
{
    m_pos = mapToScene(event->pos());
    if (m_menu) m_menu->exec(event->globalPos());
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

    // Shadow
//    QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
//    QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
//    if (rightShadow.intersects(rect) || rightShadow.contains(rect))
//        painter->fillRect(rightShadow, Qt::darkGray);
//    if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
//        painter->fillRect(bottomShadow, Qt::darkGray);
//
//    QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
//    gradient.setColorAt(0, QRgb(0x00dfdfdf));
//    gradient.setColorAt(1, Qt::lightGray);
//    painter->fillRect(rect.intersect(sceneRect), gradient);
//    painter->setBrush(Qt::NoBrush);
//    painter->drawRect(sceneRect);

	painter->setPen(QColor(Qt::black));
	painter->fillRect(sceneRect, QBrush(QRgb(0x00dfdfdf)));
	painter->drawRect(sceneRect);
}
