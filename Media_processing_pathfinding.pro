#-------------------------------------------------
#
# Project created by QtCreator 2018-11-04T12:16:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Media_processing_pathfinding
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++14

SOURCES += \
        main.cpp \
    model.cpp \
    game.cpp \
    view.cpp \
    gview.cpp \
    tview.cpp \
    DisplayManner.cpp

HEADERS += \
    model.h \
    game.h \
    view.h \
    gview.h \
    tview.h \
    DisplayManner.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

RESOURCES += \
    image.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./ -lworld
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./ -lworld
else:unix: LIBS += -L$$PWD/./ -lworld

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

DISTFILES +=
