/*
 * AMeshDock.cpp
 *
 *  Created on: 21/08/2008
 *      Author: denis
 */

#include "MeshFrame.h"

MeshFrame::MeshFrame(QWidget* parent) : QFrame(parent)
{
	init();
}

MeshFrame::~MeshFrame()
{
}

MeshDocument* MeshFrame::document()
{
	return m_document;
}

void MeshFrame::init()
{
}
