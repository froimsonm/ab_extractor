#-------------------------------------------------
#
# Project created by QtCreator 2015-05-14T12:47:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AbExtractor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcompressor.cpp

HEADERS  += mainwindow.h \
    qcompressor.h

INCLUDEPATH += C:/Qt/zlib-1.2.8
LIBS += -LC:/Qt/zlib-1.2.8 -lz

FORMS    += mainwindow.ui

