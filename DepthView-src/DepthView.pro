#-------------------------------------------------
#
# Project created by QtCreator 2011-08-30T21:55:47
#
#-------------------------------------------------

QT       += core gui

TARGET = DepthView
TEMPLATE = app

SOURCES += main.cpp\
        depthviewwindow.cpp \
    imagewidget.cpp \
    stereorender.cpp \
    sidebysiderender.cpp \
    interlacedrender.cpp \
    settingswindow.cpp \
    checkerboardrender.cpp \
    singlerender.cpp \
    version.cpp

HEADERS  += depthviewwindow.h \
    imagewidget.h \
    stereorender.h \
    sidebysiderender.h \
    interlacedrender.h \
    settingswindow.h \
    checkerboardrender.h \
    singlerender.h \
    common.h

FORMS    += \
    depthviewwindow.ui \
    settingswindow.ui

RC_FILE = DepthView.rc

RESOURCES += \
    icons.qrc

PRE_TARGETDEPS += verphony
version.target = verphony
version.commands = "$(DEL_FILE) -f $(strip $(DESTDIR))version$$QMAKE_EXT_OBJ"
version.depends =
QMAKE_EXTRA_TARGETS += version

DEFINES += SVN_REV_MACRO=\\\"$$system(svnversion)\\\"

