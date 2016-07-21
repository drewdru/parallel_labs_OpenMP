TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt


SOURCES += main.cpp \
    filter.cpp \
    TextureFeatures.cpp

QMAKE_LIBS+=-lgomp
QMAKE_CXXFLAGS+= -fopenmp
QMAKE_LFLAGS +=  -fopenmp
#QMAKE_CFLAGS_RELEASE += -fopenmp
#QMAKE_CFLAGS_DEBUG += -fopenmp
#LIBS += -lgomp
#include(deployment.pri)
#qtcAddDeployment()

HEADERS += \
    Image_reader.h
