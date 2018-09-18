#-------------------------------------------------
#
# Project created by QtCreator 2018-04-18T15:29:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = visuqwt
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
include ( /usr/local/qwt-6.1.3/features/qwt.prf )

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/myplotsqwt.cpp \
    src/histogramme.cpp \
    src/mywindow.cpp

HEADERS  += include/mainwindow.h \
    include/myplotsqwt.h \
    include/histogramme.h \
    include/zoomer.h \
    include/mywindow.h

FORMS    += src/mainwindow.ui


INCLUDEPATH += ../../../ServeurUdp/libshm/include/
INCLUDEPATH += ../../../ServeurUdp/libframe/include/
INCLUDEPATH += ../../../ServeurUdp/include/


INCLUDEPATH += $$PWD/../../../ServeurUdp/build/lib
DEPENDPATH += $$PWD/../../../ServeurUdp//build/lib

unix:!macx: LIBS += -L$$PWD/../../../ServeurUdp/build/lib/ -lringshm

INCLUDEPATH += $$PWD/../../../ServeurUdp/build
DEPENDPATH += $$PWD/../../../ServeurUdp/build

unix:!macx: LIBS += -L$$PWD/../../../ServeurUdp/build/lib/ -lframe

INCLUDEPATH += $$PWD/../../../ServeurUdp/build
DEPENDPATH += $$PWD/../../../ServeurUdp/build

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../ServeurUdp/build/lib/libframe.a
