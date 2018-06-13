#-------------------------------------------------
#
# Project created by QtCreator 2017-01-16T16:07:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Visu
TEMPLATE = app


SOURCES += main.cpp\
            mainwindow.cpp \
            qcustomplot.cpp \
            myplots.cpp

HEADERS  += mainwindow.h \
            qcustomplot.h \
            myplots.h \
    frame.h

FORMS    += mainwindow.ui
