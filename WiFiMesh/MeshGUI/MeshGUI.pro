INCLUDEPATH += ../MeshCore/inc
TARGET = MeshGUI
QT += core \
    gui
HEADERS += CoreWrappers/MeshSimulator.h \
    CoreWrappers/MeshException.h \
    CoreWrappers/MeshCore.h \
    DockWidgets/DockTimeLine.h \
    DockWidgets/DockStationProperties.h \
    DockWidgets/DockFrame.h \
    DockWidgets/DockStations.h \
    DockWidgets/DockRandomizer.h \
    Document/MeshDocument.h \
    CoreWrappers/MeshStation.h \
    Dialogs/MeshAboutDlg.h \
    App/MeshApp.h
SOURCES += CoreWrappers/MeshSimulator.cpp \
    DockWidgets/DockTimeLine.cpp \
    DockWidgets/DockFrame.cpp \
    DockWidgets/DockStations.cpp \
    DockWidgets/DockStationProperties.cpp \
    DockWidgets/DockRandomizer.cpp \
    Document/MeshDocument.cpp \
    CoreWrappers/MeshStation.cpp \
    Dialogs/MeshAboutDlg.cpp \
    App/MeshApp.cpp \
    main.cpp
FORMS += 
RESOURCES += MeshApp.qrc
LIBS += -lMeshCore

debug:LIBS += -L../MeshCore/Debug
release:LIBS += -L../MeshCore/Release
