TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt


SOURCES += main.cpp \
    sort.cpp \
    filter.cpp \
    Qsort.cpp \
    2.cpp

QMAKE_CXXFLAGS+= -fopenmp
QMAKE_LFLAGS +=  -fopenmp
#QMAKE_CFLAGS_RELEASE += -fopenmp
#QMAKE_CFLAGS_DEBUG += -fopenmp
#LIBS += -lgomp
#include(deployment.pri)
#qtcAddDeployment()

HEADERS += \
    Image_reader.h

