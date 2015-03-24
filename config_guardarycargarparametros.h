#ifndef CONFIG_GUARDARYCARGARPARAMETROS_H
#define CONFIG_GUARDARYCARGARPARAMETROS_H

#include "stand_capturadorimagen.h"
#include "config_calibrador.h"
#include "config_cropper.h"
#include "config_INTMatBuilder.h"
#include "config_matintsender.h"

namespace CONFIG
{

class guardarYCargarParametros
{
    FileStorage fs;

    STAND::capturadorImagen *cap;
    calibrador *calib;
    cropper *crop;
    matIntSender *matSender;

    static const QString QScap;
    static const QString QScalib;
    static const QString QScrop;
    static const QString QSmatSender;


public:
    static const QString QSnombreArchivo;

    guardarYCargarParametros(STAND::capturadorImagen *cap, calibrador *calib, cropper *crop, matIntSender *matSender);
    void guardar();
    void cargar();


};

}
#endif // CONFIG_GUARDARYCARGARPARAMETROS_H
