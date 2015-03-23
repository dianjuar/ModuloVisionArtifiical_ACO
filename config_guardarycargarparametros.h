#ifndef CONFIG_GUARDARYCARGARPARAMETROS_H
#define CONFIG_GUARDARYCARGARPARAMETROS_H

#include "config_calibrador.h"
#include "config_cropper.h"
#include "config_INTMatBuilder.h"
#include "config_matintsender.h"
#include "config_partirncuadros.h"
#include "config_umbralizador.h"

namespace CONFIG
{

class guardarYCargarParametros
{
    FileStorage fs;

    calibrador *calib;
    cropper crop;
    INTMatBuilder *INTMatB;
    matIntSender *matSender;
    partirNcuadros *PN;
    umbralizador *umb;

    static const QString QScalib;
    static const QString QScrop;
    static const QString QSINTMatB;
    static const QString QSmatSender;
    static const QString QSPN;
    static const QString QSumb;

    static const QString QSnombreArchivo;

public:
    guardarYCargarParametros(calibrador *calib, cropper crop, INTMatBuilder *INTMatB,
                             matIntSender *matSender, partirNcuadros *PN, umbralizador *umb);
    void guardar();


};

}
#endif // CONFIG_GUARDARYCARGARPARAMETROS_H
