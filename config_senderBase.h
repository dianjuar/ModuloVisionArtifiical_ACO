#ifndef CONFIG_SENDERBASE_H
#define CONFIG_SENDERBASE_H

#include "INCLUDE_QTstuff.h"

namespace CONFIG
{

class senderBase
{
protected:
    QTcpSocket client;
    QString serverDir;

    int serverPort;
    bool buenaConexion;

public:
    static const QString MSJ_sinComprobar;
    static const QString MSJ_comprobando;
    static const QString MSJ_correcto;
    static const QString MSJ_incorrecto;
    static const QString RUTAIMG_correcto;
    static const QString RUTAIMG_incorrecto;
    static const QString RUTAIMG_comprobando;

    static const QString MSJEnvio_divisor;
    static const QString MSJEnvio_DefaultTest;
    static const QString MSJEnvio_Prefijo_Mat;
    static const QString MSJEnvio_Prefijo_Dist;


    senderBase(QString serverDir, int serverPort);
    senderBase(){}

    void testConnection(bool Testserio=false);
    void set_buenaConexion(bool buenaConexion){ this->buenaConexion = buenaConexion; }
    void set_serverDir( QString serverDir ){ this->serverDir = serverDir; }
    bool get_buenaConexion(){ return buenaConexion; }

    void enviar(QByteArray byte);
    QByteArray virtual toByteArrat(){ return QByteArray(); }
};

}
#endif // CONFIG_SENDERBASE_H
