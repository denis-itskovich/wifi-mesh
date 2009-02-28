INCLUDEPATH += ../MeshCore/inc
TARGET = MeshGUI
QT += core \
    gui
HEADERS += Views/MeshViewTextStatistics.h \
    App/MeshMainWindow.h \
    Views/Theme/MeshTheme.h \
    Views/MeshViewStatistics.h \
    Views/Widgets/MeshWidgetLog.h \
    Views/Widgets/MeshWidgetGraphics.h \
    Views/Widgets/MeshWidgetChart.h \
    Views/Items/MeshChartItem.h \
    Views/Items/MeshGraphItemVelocityHandle.h \
    Views/Items/MeshGraphItemArrow.h \
    Views/Items/MeshGraphItemLink.h \
    Views/Items/MeshGraphItemStation.h \
    Views/Items/MeshTreeItemStation.h \
    Views/Items/MeshItemStation.h \
    Views/MeshView.h \
    Views/MeshViewRandomizer.h \
    Views/MeshViewSettings.h \
    Views/MeshViewSniffer.h \
    Views/MeshViewStations.h \
    Views/MeshViewStationsGraph.h \
    Views/MeshViewStationsList.h \
    Dialogs/MeshDlgAddPacket.h \
    Dialogs/MeshDlgAbout.h \
    Document/MeshDocument.h \
    App/MeshApp.h
SOURCES += Views/Items/MeshChartItem.cpp \
    Views/MeshViewTextStatistics.cpp \
    App/MeshMainWindow.cpp \
    Views/Theme/MeshTheme.cpp \
    Views/MeshViewStatistics.cpp \
    Views/Widgets/MeshWidgetLog.cpp \
    Views/Widgets/MeshWidgetGraphics.cpp \
    Views/Widgets/MeshWidgetChart.cpp \
    Views/Items/MeshGraphItemVelocityHandle.cpp \
    Views/Items/MeshGraphItemArrow.cpp \
    Views/Items/MeshGraphItemLink.cpp \
    Views/Items/MeshGraphItemStation.cpp \
    Views/Items/MeshTreeItemStation.cpp \
    Views/Items/MeshItemStation.cpp \
    Views/MeshView.cpp \
    Views/MeshViewRandomizer.cpp \
    Views/MeshViewSettings.cpp \
    Views/MeshViewSniffer.cpp \
    Views/MeshViewStations.cpp \
    Views/MeshViewStationsGraph.cpp \
    Views/MeshViewStationsList.cpp \
    Dialogs/MeshDlgAddPacket.cpp \
    Dialogs/MeshDlgAbout.cpp \
    Document/MeshDocument.cpp \
    App/MeshApp.cpp
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
