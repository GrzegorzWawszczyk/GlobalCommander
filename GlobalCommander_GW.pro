#-------------------------------------------------
#
# Project created by QtCreator 2016-09-13T14:29:29
#
#-------------------------------------------------

QT      += core gui

QT      += concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GlobalCommander_GW
TEMPLATE = app


SOURCES += main.cpp\
        gcmdmainwindow.cpp \
    filelistmodel.cpp \
    drivelistmodel.cpp \
    drivescombobox.cpp \
    filesview.cpp \
    filestableview.cpp \
    fileoperationshandler.cpp \
    filescopier.cpp \
    filesmover.cpp \
    settingswindow.cpp

HEADERS  += gcmdmainwindow.h \
    filelistmodel.h \
    drivelistmodel.h \
    drivescombobox.h \
    filesview.h \
    filestableview.h \
    fileoperationshandler.h \
    filescopier.h \
    filesmover.h \
    settingswindow.h

FORMS    += gcmdmainwindow.ui \
    filesview.ui \
    settingswindow.ui
