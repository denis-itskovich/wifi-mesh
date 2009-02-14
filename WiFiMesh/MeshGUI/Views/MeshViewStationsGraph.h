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
 * @file MeshViewCanvas.h
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * @date 13/01/2009
 * @author Denis Itskovich
 */


#ifndef MESHVIEWCANVAS_H_
#define MESHVIEWCANVAS_H_

#include "MeshViewStations.h"
#include "Items/MeshGraphItemStation.h"
#include "Items/MeshGraphItemLink.h"

class MeshGraphics : public QGraphicsView
{
	Q_OBJECT

public:
	MeshGraphics(QWidget* parent = 0);
	void addItem(QGraphicsItem* item);
	void removeItem(QGraphicsItem* item);
	void setMenu(QMenu* menu);
	QPointF pos() const;

signals:
	void doubleClicked(QPointF pos);
	void focusCleared();

protected:
    virtual void contextMenuEvent(QContextMenuEvent* event);
	virtual void keyPressEvent(QKeyEvent* event);
    virtual void wheelEvent(QWheelEvent* event);
    virtual void mouseDoubleClickEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void drawBackground(QPainter *painter, const QRectF &rect);

private:
    QMenu*  m_menu;
    QPointF m_pos;
};

class MeshViewStationsGraph : public MeshViewStations
{
	Q_OBJECT
public:
	MeshViewStationsGraph(QWidget* parent = 0);

	virtual void setDocument(MeshDocument* document);

protected:
	virtual void addStation(Station* pStation);
	virtual void removeStation(Station* pStation);
	virtual void setCurrent(Station* pStation);
	virtual void updateStation(Station* pStation);
    virtual void beginTransmit(const Station* pSrc, const Station* pDst, const Packet* pPacket);
    virtual void endTransmit(const Station* pDst);

private slots:
    void clearCurrent();
	void addStation(QPointF pos);
	void updateWorldSize();

private:
    typedef QMultiMap<const Station*, MeshGraphItemLink*>   LinkMap;
    typedef QList<MeshGraphItemLink*>                       LinkList;

    void init();
	MeshGraphItemStation* findItem(Station* pStation) const;
	MeshGraphItemStation* currentItem() const;
	void updateLinks(const LinkList& links);
    void addStationTriggered();

	MeshGraphics*	       m_graphStations;
	QGraphicsItem*	       m_resizeGaps[4];
	LinkMap                m_srcToLink;
	LinkMap                m_dstToLink;
};

#endif /* MESHVIEWCANVAS_H_ */
