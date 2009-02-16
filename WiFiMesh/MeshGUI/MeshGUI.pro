INCLUDEPATH += ../MeshCore/inc
TARGET = MeshGUI
QT += core \
    gui
HEADERS += Views/Items/MeshChartItem.h \
    Views/MeshViewChart.h \
    Views/Items/MeshGraphItemVelocityHandle.h \
    Views/Items/MeshGraphItemArrow.h \
    Views/Items/MeshGraphItemLink.h \
    Views/Items/MeshGraphItemStation.h \
    Views/Items/MeshTreeItemStation.h \
    Views/Items/MeshItemStation.h \
    Views/MeshLog.h \
    Views/MeshView.h \
    Views/MeshViewRandomizer.h \
    Views/MeshViewSettings.h \
    Views/MeshViewSniffer.h \
    Views/MeshViewStationProperties.h \
    Views/MeshViewStations.h \
    Views/MeshViewStationsGraph.h \
    Views/MeshViewStationsList.h \
    Views/MeshViewTimeLine.h \
    Dialogs/MeshDlgAddPacket.h \
    Dialogs/MeshDlgAbout.h \
    Document/MeshDocument.h \
    App/MeshApp.h
SOURCES += Views/MeshViewChart.cpp \
    Views/Items/MeshGraphItemVelocityHandle.cpp \
    Views/Items/MeshGraphItemArrow.cpp \
    Views/Items/MeshGraphItemLink.cpp \
    Views/Items/MeshGraphItemStation.cpp \
    Views/Items/MeshTreeItemStation.cpp \
    Views/Items/MeshItemStation.cpp \
    Views/MeshLog.cpp \
    Views/MeshView.cpp \
    Views/MeshViewRandomizer.cpp \
    Views/MeshViewSettings.cpp \
    Views/MeshViewSniffer.cpp \
    Views/MeshViewStationProperties.cpp \
    Views/MeshViewStations.cpp \
    Views/MeshViewStationsGraph.cpp \
    Views/MeshViewStationsList.cpp \
    Views/MeshViewTimeLine.cpp \
    Dialogs/MeshDlgAddPacket.cpp \
    Dialogs/MeshDlgAbout.cpp \
    Document/MeshDocument.cpp \
    App/MeshApp.cpp \
    main.cpp
FORMS += 
RESOURCES += MeshApp.qrc
LIBS += -lMeshCore
DebugBuild { 
    LIBS += -L../MeshCore/Debug
    PRE_TARGETDEPS += ../MeshCore/Debug/libMeshCore.a
}
ReleaseBuild { 
    LIBS += -L../MeshCore/Release
    PRE_TARGETDEPS += ../MeshCore/Release/libMeshCore.a
}
