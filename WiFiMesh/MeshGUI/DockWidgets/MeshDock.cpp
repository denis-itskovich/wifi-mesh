/*
 * AMeshDock.cpp
 *
 *  Created on: 21/08/2008
 *      Author: denis
 */

#include "MeshDock.h"

MeshDock::MeshDock(const QString& title, QWidget* parent) : QDockWidget(title, parent)
{
}

MeshDock::MeshDock(QWidget* parent) : QDockWidget(parent)
{
}

MeshDock::~MeshDock()
{
}

MeshDocument* MeshDock::document()
{
	return m_document;
}
