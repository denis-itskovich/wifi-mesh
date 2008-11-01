INCLUDEPATH += 
TARGET = MeshGUI
QT += core \
    gui
HEADERS += DockWidgets/DockTimeLine.h \
    DockWidgets/DockStationProperties.h \
    DockWidgets/DockFrame.h \
    DockWidgets/DockStations.h \
    DockWidgets/DockRandomizer.h \
    Document/MeshDocument.h \
    CoreWrappers/MeshStation.h \
    Dialogs/MeshAboutDlg.h \
    App/MeshApp.h
SOURCES += DockWidgets/DockTimeLine.cpp \
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
