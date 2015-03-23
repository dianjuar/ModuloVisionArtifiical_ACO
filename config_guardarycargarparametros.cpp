#include "config_guardarycargarparametros.h"

using namespace CONFIG;

const QString guardarYCargarParametros::QScalib = QString("calib");
const QString guardarYCargarParametros::QScrop = QString("crop");
const QString guardarYCargarParametros::QSINTMatB = QString("INTMatB");
const QString guardarYCargarParametros::QSmatSender = QString("matSender");
const QString guardarYCargarParametros::QSPN = QString("PN");
const QString guardarYCargarParametros::QSumb = QString("umb");
const QString guardarYCargarParametros::QSnombreArchivo = QString("parametrosCalibración.yml");

guardarYCargarParametros::guardarYCargarParametros(calibrador *calib, cropper crop, INTMatBuilder *INTMatB,
                                                   matIntSender *matSender, partirNcuadros *PN, umbralizador *umb)
{
    this-> calib = calib;
    this-> crop = crop;
    this-> INTMatB = INTMatB;
    this-> matSender = matSender;
    this-> PN = PN;
    this-> umb = umb;
}

void guardarYCargarParametros::guardar()
{
    fs = FileStorage(QSnombreArchivo.toUtf8().data(), FileStorage::WRITE);

    //fs << QScalib.toUtf8().data() << calib;

    fs.release();

}
