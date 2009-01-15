INCLUDEPATH += ../MeshCore/inc
TARGET = MeshGUI
QT += core \
    gui
HEADERS += MeshViews/MeshGraphItemStation.h \
    MeshViews/MeshTreeItemStation.h \
    MeshViews/MeshItemStation.h \
    MeshViews/MeshViewStationsGraph.h \
    MeshViews/MeshViewStationsList.h \
    MeshViews/MeshLog.h \
    MeshViews/MeshViewTimeLine.h \
    MeshViews/MeshViewStations.h \
    MeshViews/MeshViewStationProperties.h \
    MeshViews/MeshViewSettings.h \
    MeshViews/MeshViewRandomizer.h \
    MeshViews/MeshView.h \
    CoreWrappers/MeshException.h \
    CoreWrappers/MeshCore.h \
    Document/MeshDocument.h \
    Dialogs/MeshAboutDlg.h \
    App/MeshApp.h
SOURCES += MeshViews/MeshGraphItemStation.cpp \
    MeshViews/MeshTreeItemStation.cpp \
    MeshViews/MeshItemStation.cpp \
    MeshViews/MeshViewStationsList.cpp \
    MeshViews/MeshViewStationsGraph.cpp \
    MeshViews/MeshLog.cpp \
    MeshViews/MeshViewTimeLine.cpp \
    MeshViews/MeshViewStations.cpp \
    MeshViews/MeshViewStationProperties.cpp \
    MeshViews/MeshViewSettings.cpp \
    MeshViews/MeshViewRandomizer.cpp \
    MeshViews/MeshView.cpp \
    Document/MeshDocument.cpp \
    Dialogs/MeshAboutDlg.cpp \
    App/MeshApp.cpp \
    main.cpp
FORMS += 
RESOURCES += MeshApp.qrc
LIBS += -lMeshCore
debug:LIBS += -L../MeshCore/Debug
release:LIBS += -L../MeshCore/Release
