#-------------------------------------------------
#
# Project created by QtCreator 2016-03-25T15:24:20
#
#-------------------------------------------------

QT       += core svg gui

greaterThan(QT_MAJOR_VERSION, 4): QT += svg widgets

TARGET = svgDemo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../Qt/Qt5.6.0/5.6/msvc2015_64/lib/ -lQt5Svg
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../Qt/Qt5.6.0/5.6/msvc2015_64/lib/ -lQt5Svgd
else:unix: LIBS += -L$$PWD/../../../../Qt/Qt5.6.0/5.6/msvc2015_64/lib/ -lQt5Svg

INCLUDEPATH += $$PWD/../../../../Qt/Qt5.6.0/5.6/msvc2015_64/include
DEPENDPATH += $$PWD/../../../../Qt/Qt5.6.0/5.6/msvc2015_64/include

DISTFILES +=
