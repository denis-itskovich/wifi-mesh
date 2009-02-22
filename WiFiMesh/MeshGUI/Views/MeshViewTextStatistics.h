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
 * @file MeshViewTextStatistics.h
 * @date 22/02/2009
 * @author Denis Itskovich
 */

#ifndef MESHVIEWTEXTSTATISTICS_H_
#define MESHVIEWTEXTSTATISTICS_H_

#include "MeshView.h"


class MeshViewTextStatistics : public MeshView
{
    Q_OBJECT
public:
    MeshViewTextStatistics(QWidget* parent = NULL);

    virtual void setDocument(MeshDocument* doc);

protected slots:
    void updateStatistics(const Statistics* pStatistics);

private:
    void init();

    QTextEdit*          m_text;
};

#endif /* MESHVIEWTEXTSTATISTICS_H_ */
