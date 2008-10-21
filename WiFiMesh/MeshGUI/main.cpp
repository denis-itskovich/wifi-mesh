#include "App/MeshApp.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationVersion("0.1.0");
    a.setApplicationName("WiFi Mesh Simulator");

    // Q_INIT_RESOURCE(MeshApp);
    MeshApp w;
    w.show();
    return a.exec();
}
