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

#include "MeshApp.h"
#include "../Core/MeshException.h"

MeshApp::MeshApp(int argc, char** argv) :
    QApplication(argc, argv)
{
    setApplicationVersion("1.0.4");
    setApplicationName("WiFi Mesh Simulator PRO");
    m_mainWindow.showMaximized();
}

bool MeshApp::notify(QObject* receiver, QEvent* event)
{
    try
    {
        return QApplication::notify(receiver, event);
    }
    catch (MeshException* e)
    {
        QMessageBox::critical(&m_mainWindow, "MeshCore failure", e->what());
        return false;
    }
}

int main(int argc, char** argv)
{
    MeshApp app(argc, argv);
    return app.exec();
}
