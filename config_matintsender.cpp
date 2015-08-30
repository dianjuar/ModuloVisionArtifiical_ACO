#include "config_matintsender.h"

using namespace CONFIG;

matIntSender::matIntSender(QString serverDir):senderBase(serverDir,port)
{

}

void matIntSender::enviarInformacion(int **mat, int n, float dist)
{
    QString sms = senderBase::MSJEnvio_Prefijo_Mat;
    sms.append( STAND::Tools::IntMat2QString( mat, n ));

    sms.append( senderBase::MSJEnvio_divisor );

    sms.append(senderBase::MSJEnvio_Prefijo_Dist);
    sms.append( QString::number(dist) );

    enviar( sms.toUtf8().data() );
}

void matIntSender::enviarInformacion(QString mat, float dist)
{
    QString sms = senderBase::MSJEnvio_Prefijo_Mat;
    sms.append( mat );
    sms.append( senderBase::MSJEnvio_divisor );

    sms.append(senderBase::MSJEnvio_Prefijo_Dist);
    sms.append( QString::number(dist) );

    enviar( sms.toUtf8().data() );
}

void matIntSender::write(FileStorage &fs) const
{
    fs<<"{"<< "serverDir" << serverDir.toUtf8().data()<< "}";
}

void matIntSender::read(const FileNode &node)
{
    std::string server;

    node["serverDir"] >> server;

    serverDir = QString::fromUtf8( server.c_str() );
}


