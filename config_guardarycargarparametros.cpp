#include "config_guardarycargarparametros.h"

using namespace CONFIG;

const QString guardarYCargarParametros::QScap = QString("cap");
const QString guardarYCargarParametros::QScalib = QString("calib");
const QString guardarYCargarParametros::QScrop = QString("crop");
const QString guardarYCargarParametros::QScolor = QString("ColorSesgo");
const QString guardarYCargarParametros::QSmatSender = QString("matSender");
const QString guardarYCargarParametros::QSnombreArchivo = QString("parametrosCalibracion.yml");
const QString guardarYCargarParametros::QScSMA = QString("cSMA");
const QString guardarYCargarParametros::QSINTmB = QString("INTmB");

guardarYCargarParametros::guardarYCargarParametros(STAND::capturadorImagen *cap, calibrador *calib, cropper *crop,
                                                   colorDetector *color, INTMatBuilder *INTmB,
                                                   matIntSender *matSender, connectSistemaMultiAgente *cSMA)
{
    this->cap = cap;
    this-> calib = calib;
    this-> crop = crop;
    this->color = color;
    this->INTmB = INTmB;
    this-> matSender = matSender;
    this->cSMA = cSMA;
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

    fs<<QScolor.toUtf8().data();
    color->write(fs);

    fs<<QSINTmB.toUtf8().data();
    INTmB->write(fs);

    fs<<QSmatSender.toUtf8().data();
    matSender->write(fs);

    fs<<QScSMA.toUtf8().data();
    cSMA->write(fs);

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

    n = fs[QScolor.toUtf8().data()];
    color->read( n, fs );

    n = fs[QSINTmB.toUtf8().data()];
    INTmB->read( n );

    n = fs[QSmatSender.toUtf8().data()];
    matSender->read( n );

    n = fs[QScSMA.toUtf8().data()];
    cSMA->read( n );

    fs.release();

}
