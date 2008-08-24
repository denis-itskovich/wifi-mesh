/*
 * AMeshDock.cpp
 *
 *  Created on: 21/08/2008
 *      Author: denis
 */

#include "DockFrame.h"

DockFrame::DockFrame(QWidget* parent) : QFrame(parent)
{
	init();
}

DockFrame::~DockFrame()
{
}

MeshDocument* DockFrame::document()
{
	return m_document;
}

void DockFrame::init()
{
}
