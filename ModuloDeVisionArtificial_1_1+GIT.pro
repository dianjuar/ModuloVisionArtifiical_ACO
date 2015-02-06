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
    config_cropper.cpp \
    config_umbralizador.cpp \
    config_circledetect.cpp \
    config_partirncuadros.cpp \
    config_INTMatBuilder.cpp

HEADERS  += ventanaprincipal.h \
    stand_capturadorimagen.h \
    INCLUDE_opencv.h \
    INCLUDE_QTstuff.h \
    config_cropper.h \
    stand_Tools.h \
    config_umbralizador.h \
    config_circledetect.h \
    config_partirncuadros.h \
    config_INTMatBuilder.h

FORMS    += ventanaprincipal.ui
