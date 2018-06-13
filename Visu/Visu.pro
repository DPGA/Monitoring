#-------------------------------------------------
#
# Project created by QtCreator 2017-01-16T16:07:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Visu
TEMPLATE = app


SOURCES += src/main.cpp\
            src/mainwindow.cpp \
            src/myplots.cpp

HEADERS  += include/mainwindow.h \
            include/myplots.h


FORMS    += src/mainwindow.ui

unix:!macx: LIBS += -L$$PWD/../../../../../../../usr/lib/x86_64-linux-gnu/ -lrt

INCLUDEPATH += $$PWD/''
INCLUDEPATH += $$PWD/include/
INCLUDEPATH +=  ../libdaq/include/
INCLUDEPATH += ../include/
INCLUDEPATH += ../libqcustomplot/include/
INCLUDEPATH += ../libshm/include/
INCLUDEPATH += ../libframe/include/
DEPENDPATH += $$PWD/''

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../../../../usr/lib/x86_64-linux-gnu/librt.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../lib/build/release/ -lringshm
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../lib/build/debug/ -lringshm -lqcustomplot -lframe
else:unix: LIBS += -L$$PWD/../build/lib/ -lringshm

INCLUDEPATH += $$PWD/../../lib/build
DEPENDPATH += $$PWD/../../lib/build
