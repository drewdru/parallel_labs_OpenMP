#-------------------------------------------------
#
# Project created by QtCreator 2014-12-09T10:29:40
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = parallel_lab5
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_LIBS+=-lgomp
QMAKE_CXXFLAGS+= -fopenmp
QMAKE_LFLAGS +=  -fopenmp

SOURCES += main.cpp \
    threads.cpp
