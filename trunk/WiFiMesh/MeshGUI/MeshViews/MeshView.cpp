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

MeshView::~MeshView()
{
}

MeshDocument* MeshView::document()
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
