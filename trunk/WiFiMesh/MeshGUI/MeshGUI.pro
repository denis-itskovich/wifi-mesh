INCLUDEPATH += 
TARGET = MeshGUI
QT += core \
    gui
HEADERS += Document/MeshDocument.h \
    DockWidgets/MeshDock.h \
    DockWidgets/MeshDockRandomizer.h \
    CoreWrappers/MeshStation.h \
    DockWidgets/MeshDockStations.h \
    Dialogs/MeshAboutDlg.h \
    App/MeshApp.h
SOURCES += Document/MeshDocument.cpp \
    DockWidgets/MeshDock.cpp \
    DockWidgets/MeshDockRandomizer.cpp \
    CoreWrappers/MeshStation.cpp \
    DockWidgets/MeshDockStations.cpp \
    Dialogs/MeshAboutDlg.cpp \
    App/MeshApp.cpp \
    main.cpp
FORMS += 
RESOURCES += MeshApp.qrc
