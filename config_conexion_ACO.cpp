#include "config_conexion_ACO.h"

using namespace CONFIG::Network;

conexion_ACO::conexion_ACO(QString serverDir):Client(serverDir,port)
{

}

void conexion_ACO::enviarInformacion(int **mat, int n, float dist)
{
    enviar( prepararMSJ_enviarMatrix(mat, n, dist) );
}

void conexion_ACO::enviarInformacion(QString mat, float dist)
{
    enviar( prepararMSJ_enviarMatrix(mat,dist) );
}

void conexion_ACO::write(FileStorage &fs) const
{
    fs<<"{"<< "serverDir" << host.toUtf8().data()<< "}";
}

void conexion_ACO::read(const FileNode &node)
{
    std::string server;

    node["serverDir"] >> server;

    host = QString::fromUtf8( server.c_str() );
}

QString conexion_ACO::prepararMSJ_enviarMatrix(int **mat, float dist, int n)
{
    QString sms=Tools::Network::GestionDeMensajes::MSJEnvio_Prefijo_Mat;

    sms.append( Tools::Cfunctions::IntMat2QString( mat, n ));

    sms.append( Tools::Network::GestionDeMensajes::Msj_divisor );

    sms.append( Tools::Network::GestionDeMensajes::MSJEnvio_Prefijo_Dist);
    sms.append( QString::number(dist) );

    return sms;
}

QString conexion_ACO::prepararMSJ_enviarMatrix(QString mat, float dist)
{
    QString sms = Tools::Network::GestionDeMensajes::MSJEnvio_Prefijo_Mat;
    sms.append( mat );
    sms.append( Tools::Network::GestionDeMensajes::Msj_divisor );

    sms.append( Tools::Network::GestionDeMensajes::MSJEnvio_Prefijo_Dist);
    sms.append( QString::number(dist) );

    return sms;
}

void conexion_ACO::AnalizadorDeMensajes(QString msj)
{
    qDebug()<<"DIOS MEEEEOOOOO";
}


