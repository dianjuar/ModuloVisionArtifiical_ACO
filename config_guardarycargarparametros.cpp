#include "config_guardarycargarparametros.h"

using namespace CONFIG;

const QString guardarYCargarParametros::QScap = QString("cap");
const QString guardarYCargarParametros::QScalib = QString("calib");
const QString guardarYCargarParametros::QScrop = QString("crop");
const QString guardarYCargarParametros::QSmatSender = QString("matSender");
const QString guardarYCargarParametros::QSnombreArchivo = QString("parametrosCalibracion.yml");

guardarYCargarParametros::guardarYCargarParametros(STAND::capturadorImagen *cap, calibrador *calib, cropper *crop,
                                                   matIntSender *matSender)
{
    this->cap = cap;
    this-> calib = calib;
    this-> crop = crop;
    this-> matSender = matSender;
}

void guardarYCargarParametros::guardar()
{
    fs = FileStorage(QSnombreArchivo.toUtf8().data(), FileStorage::WRITE);

    fs << QScap.toUtf8().data();
    cap->write( fs );

    fs << QScalib.toUtf8().data();
    calib->write( fs );

    fs<<QScrop.toUtf8().data();
    crop->write(fs);

    fs<<QSmatSender.toUtf8().data();
    matSender->write(fs);

    fs.release();

}

void guardarYCargarParametros::cargar()
{
    fs = FileStorage(QSnombreArchivo.toUtf8().data(), FileStorage::READ);


    FileNode n = fs[QScap.toUtf8().data()];
    cap->read( n );

    n = fs[QScalib.toUtf8().data()];
    calib->read( n );

    n = fs[QScrop.toUtf8().data()];
    crop->read( n );

    n = fs[QSmatSender.toUtf8().data()];
    matSender->read( n );


    fs.release();

}
