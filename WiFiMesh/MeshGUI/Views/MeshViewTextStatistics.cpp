/*********************************************************************************
MeshGUI
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
 * TODO Fill file purpose and description
 * @file MeshViewTextStatistics.cpp
 * @date 22/02/2009
 * @author Denis Itskovich
 */

#include "MeshViewTextStatistics.h"

MeshViewTextStatistics::MeshViewTextStatistics(QWidget* parent) :
    MeshView(parent)
{
    init();
}

void MeshViewTextStatistics::setDocument(MeshDocument* doc)
{
    disconnect(document());
    connect(doc, SIGNAL(statisticsUpdated(const Statistics*)), this, SLOT(updateStatistics(const Statistics*)));
    MeshView::setDocument(doc);
}

void MeshViewTextStatistics::init()
{
    m_text = new QTextEdit;
    m_text->setFont(QFont("Courier", 10));
    m_text->setReadOnly(true);
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(m_text);
    layout->setMargin(0);
    setLayout(layout);
}

void MeshViewTextStatistics::updateStatistics(const Statistics* pStatistics)
{
    char buff[2048];
    int len = (int)sizeof(buff);
    CHECK(StatisticsPrint(pStatistics, buff, len));
    m_text->setText(buff);
}
