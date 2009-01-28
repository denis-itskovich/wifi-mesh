/**
 * \file MeshTreeItemStation.cpp
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * \date 13/01/2009
 * \author Denis Itskovich
 */

#include "MeshTreeItemStation.h"

MeshTreeItemStation::MeshTreeItemStation(MeshViewStations* pContainer, Station* pStation) :
	MeshItemStation(pContainer, pStation),
	QTreeWidgetItem(QStringList() << name())
{
	init();
}

void MeshTreeItemStation::init()
{
	QPointF loc = location();
	QPointF vel = velocity();

	QTreeWidgetItem* locItem = new QTreeWidgetItem(QStringList() << "Location");
	locItem->addChild(new QTreeWidgetItem(QStringList() << "x" << QString::number(loc.x(), 'g', 2)));
	locItem->addChild(new QTreeWidgetItem(QStringList() << "y" << QString::number(loc.y(), 'g', 2)));

	QTreeWidgetItem* velItem = new QTreeWidgetItem(QStringList() << "Velocity");
	velItem->addChild(new QTreeWidgetItem(QStringList() << "x" << QString::number(vel.x(), 'g', 2)));
	velItem->addChild(new QTreeWidgetItem(QStringList() << "y" << QString::number(vel.y(), 'g', 2)));

	addChild(locItem);
	addChild(velItem);
}

void MeshTreeItemStation::updateStation()
{
	QTreeWidgetItem* locItem = child(0);
	QTreeWidgetItem* velItem = child(1);

	QPointF loc = location();
	QPointF vel = velocity();

	locItem->setText(1, QString("(%1, %2)").arg(loc.x()).arg(loc.y()));
	velItem->setText(1, QString("(%1, %2)").arg(vel.x()).arg(vel.y()));

	locItem->child(0)->setText(1, QString("%1").arg(loc.x()));
	locItem->child(1)->setText(1, QString("%1").arg(loc.y()));

	velItem->child(0)->setText(1, QString("%2").arg(vel.x()));
	velItem->child(1)->setText(1, QString("%2").arg(vel.y()));
}
