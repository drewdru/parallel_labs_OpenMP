TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt



SOURCES += main.cpp \
    pi.cpp \
    Matr.cpp

#include(deployment.pri)
#qtcAddDeployment()

QMAKE_LIBS+=-lgomp
QMAKE_CXXFLAGS+=-fopenmp
QMAKE_LFLAGS += -fopenmp
