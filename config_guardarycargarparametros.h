#ifndef CONFIG_GUARDARYCARGARPARAMETROS_H
#define CONFIG_GUARDARYCARGARPARAMETROS_H

#include "stand_capturadorimagen.h"
#include "config_calibrador.h"
#include "config_cropper.h"
#include "config_colordetector.h"
#include "config_INTMatBuilder.h"
#include "config_conexion_ACO.h"
#include "config_conexion_SMA.h"
#include "config_INTMatBuilder.h"

namespace CONFIG
{

class guardarYCargarParametros
{
    FileStorage fs;

    STAND::capturadorImagen *cap;
    calibrador *calib;
    cropper *crop;
    coTra::colorDetector *color;
    INTMatBuilder *INTmB;
    Network::conexion_ACO *matSender;
    Network::conexion_SMA *cSMA;

    static const QString QScap;
    static const QString QScalib;
    static const QString QScrop;
    static const QString QScolor;
    static const QString QSINTmB;
    static const QString QSmatSender;
    static const QString QScSMA;


public:
    static const QString QSnombreArchivo;

    guardarYCargarParametros(STAND::capturadorImagen *cap, calibrador *calib, cropper *crop,
                             coTra::colorDetector *color, INTMatBuilder *INTmB,
                             Network::conexion_ACO *matSender, Network::conexion_SMA *cSMA);

    void guardar();
    void cargar();


};

}
#endif // CONFIG_GUARDARYCARGARPARAMETROS_H
