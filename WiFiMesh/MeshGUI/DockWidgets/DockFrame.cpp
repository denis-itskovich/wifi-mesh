/*
 * AMeshDock.cpp
 *
 *  Created on: 21/08/2008
 *      Author: denis
 */

#include "DockFrame.h"

DockFrame::DockFrame(MeshDocument* doc, QWidget* parent) :
	QWidget(parent)
{
	m_document = doc;
	init();
}

DockFrame::~DockFrame()
{
}

MeshDocument* DockFrame::document()
{
	return m_document;
}

void DockFrame::setDocument(MeshDocument* doc)
{
	m_document = doc;
}

void DockFrame::init()
{
}
