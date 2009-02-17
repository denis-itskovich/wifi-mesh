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
	m_graphStations = new MeshWidgetGraphics();
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
