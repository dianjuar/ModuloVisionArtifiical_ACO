#-------------------------------------------------
#
# Project created by QtCreator 2015-02-03T17:51:47
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = ModuloDeVisionArtificial_1_1+GIT
TEMPLATE = app

LIBS += `pkg-config opencv --libs`


SOURCES += main.cpp\
        ventanaprincipal.cpp \
    stand_capturadorimagen.cpp \
    config_cropper.cpp \
    config_umbralizador.cpp \
    config_partirncuadros.cpp \
    config_INTMatBuilder.cpp \
    config_calibrador.cpp \
    stand_clickablelabel.cpp \
    config_guardarycargarparametros.cpp \
    config_colordetector.cpp \
    stand_networking.cpp \
    stand_Tools.cpp \
    config_conexion_ACO.cpp \
    config_conexion_SMA.cpp \
    config_configbase.cpp

HEADERS  += ventanaprincipal.h \
    stand_capturadorimagen.h \
    INCLUDE_opencv.h \
    INCLUDE_QTstuff.h \
    config_cropper.h \
    stand_Tools.h \
    config_umbralizador.h \
    config_partirncuadros.h \
    config_INTMatBuilder.h \
    config_calibrador.h \
    stand_clickablelabel.h \
    config_guardarycargarparametros.h \
    config_colordetector.h \
    stand_networking.h \
    config_conexion_ACO.h \
    config_conexion_SMA.h \
    config_configbase.h

FORMS    += ventanaprincipal.ui
