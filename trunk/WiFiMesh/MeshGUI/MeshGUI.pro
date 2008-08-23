INCLUDEPATH += 
TARGET = MeshGUI
QT += core \
    gui
HEADERS += DockWidgets/MeshFrameStations.h \
    DockWidgets/MeshFrameStationProperties.h \
    DockWidgets/MeshFrameRandomizer.h \
    DockWidgets/MeshFrame.h \
    Document/MeshDocument.h \
    CoreWrappers/MeshStation.h \
    Dialogs/MeshAboutDlg.h \
    App/MeshApp.h
SOURCES += DockWidgets/MeshFrameStations.cpp \
    DockWidgets/MeshFrameStationProperties.cpp \
    DockWidgets/MeshFrameRandomizer.cpp \
    DockWidgets/MeshFrame.cpp \
    Document/MeshDocument.cpp \
    CoreWrappers/MeshStation.cpp \
    Dialogs/MeshAboutDlg.cpp \
    App/MeshApp.cpp \
    main.cpp
FORMS += 
RESOURCES += MeshApp.qrc
