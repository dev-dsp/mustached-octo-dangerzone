#-------------------------------------------------
#
# Project created by QtCreator 2014-02-21T13:24:34
#
#-------------------------------------------------

QT       += core gui sql
QTPLUGIN += qsqlite
QMAKE_CXXFLAGS += -O2 -g -w
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RUBD
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    DB.cpp \
    tblmodel.cpp \
    editdialog.cpp \
    adddialog.cpp \
    dateeditdelegate.cpp \
    exportdialog.cpp

HEADERS  += mainwindow.h \
    DB.h \
    tblmodel.h \
    editdialog.h \
    DialogTypes.h \
    adddialog.h \
    dateeditdelegate.h \
    exportdialog.h

FORMS    += mainwindow.ui \
    editdialog.ui \
    adddialog.ui \
    exportdialog.ui
