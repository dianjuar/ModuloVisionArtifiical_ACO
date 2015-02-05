#-------------------------------------------------
#
# Project created by QtCreator 2015-02-03T17:51:47
#
#-------------------------------------------------

QT       += core gui

TARGET = ModuloDeVisionArtificial_1_1+GIT
TEMPLATE = app

LIBS += `pkg-config opencv --libs`


SOURCES += main.cpp\
        ventanaprincipal.cpp \
    stand_capturadorimagen.cpp \
    config_cropper.cpp

HEADERS  += ventanaprincipal.h \
    stand_capturadorimagen.h \
    INCLUDE_opencv.h \
    INCLUDE_QTstuff.h \
    config_cropper.h \
    stand_Tools.h

FORMS    += ventanaprincipal.ui
