#include "App/MeshApp.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(MeshApp);
    MeshApp w;
    w.show();
    return a.exec();
}
