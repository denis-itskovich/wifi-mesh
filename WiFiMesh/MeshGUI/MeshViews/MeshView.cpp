/*
 * AMeshDock.cpp
 *
 *  Created on: 21/08/2008
 *      Author: denis
 */

#include "MeshView.h"

MeshView::MeshView(QWidget* parent) :
	QWidget(parent),
	m_document(NULL)
{
	init();
}

MeshDocument* MeshView::document() const
{
	return m_document;
}

void MeshView::setDocument(MeshDocument* doc)
{
	m_document = doc;
}

void MeshView::init()
{
}

void MeshView::enable()
{
	setEnabled(true);
}

void MeshView::disable()
{
	setEnabled(false);
}
