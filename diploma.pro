#-------------------------------------------------
#
# Project created by QtCreator 2015-04-21T12:42:36
#
#-------------------------------------------------

QT       += core gui printsupport
CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = diploma
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    qcustomplot.cpp

HEADERS  += widget.h \
            bass.h \
    qcustomplot.h

FORMS    += widget.ui

unix|win32: LIBS += -lbass
