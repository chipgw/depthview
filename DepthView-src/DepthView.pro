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
    checkerboardrender.cpp

HEADERS  += depthviewwindow.h \
    imagewidget.h \
    stereorender.h \
    sidebysiderender.h \
    interlacedrender.h \
    settingswindow.h \
    checkerboardrender.h

FORMS    += \
    depthviewwindow.ui \
    settingswindow.ui

RC_FILE = DepthView.rc


