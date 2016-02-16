#include "stand_networking.h"

using namespace Network;
using namespace Tools::Network;


DataSend::DataSend(QTcpSocket *socket)
{
    this->socket = socket;
}

void DataSend::enviar(QByteArray byte)
{
    socket->write( byte );
    socket->flush();
}

void DataSend::enviar(QString s)
{
    socket->write( s.toUtf8() );
}
/////////////////////////////////////////////////////////
DataRecibe::DataRecibe(QTcpSocket *socket)
{
    this->socket = socket;
}

void DataRecibe::readyRead()
{
    qDebug()<<("*********Leyendo**********");
    QString s( socket->readAll() );
    qDebug()<<s;
    qDebug()<<("********Analizando***********");

    AnalizadorDeMensajes(s);
}
/////////////////////////////////////////////////////////
///Mensajes Standar
const QString GestionDeMensajes::Msj_divisor = "->";
const QString GestionDeMensajes::Msj_divisor_2 = "_";
const QString GestionDeMensajes::Msj_cerrar = "close";

    ///FUNCTIONS Standar
QString GestionDeMensajes::Enviar_MSJ_conectado()
{
    return Msj_conectado;
}

///////////////***********************/////////////////////
const QString GestionDeMensajes::Msj_conectado = "connect";
///Mensajes TO SMA
const QString GestionDeMensajes::Msj_SMAtoMDV_solicitudTrayectoria = "CorrectMe";
const QString GestionDeMensajes::Msj_SMAtoMDV_correctedTrayectoriaAPPLIED = "CorrectFIN";
//---
const QString GestionDeMensajes::Msj_MDVtoSMA_DespachoSolicitudTrayectoria = "Corrected";

    ///FUNCTIONS SMA
QString GestionDeMensajes::Enviar_TOSMA_MSJ_TrayectoriaCorrected(int RobotID, float teta, double distanciaDesface, float anguloDesface)
{
    QString r = Msj_MDVtoSMA_DespachoSolicitudTrayectoria + Msj_divisor +
                                           QString::number(RobotID) + Msj_divisor_2 +
                                           QString::number(teta) + Msj_divisor_2 +
                                           QString::number(distanciaDesface) + Msj_divisor_2 +
                                           QString::number(anguloDesface);

    return r;
}
/////////////////////////////////////////////////////////
Client::Client(QString host, int port):
    DataSend(&socket),
    DataRecibe(&socket)
{
    connected_B = false;
    this->host = host;
    this->port = port;
}

void Client::connectToHost()
{
    socket.connectToHost(host, port);

    connected_B = socket.waitForConnected(3000);

    if(connected_B)//connected
    {
        connect(&socket, SIGNAL(connected()), this, SLOT(connected()));
        connect(&socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
        connect(&socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
        connect(&socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

        //enviar( Tools::Network::GestionDeMensajes::Enviar_MSJ_conectado() );//el error está aquí
    }
    else
    {
        qDebug()<<"ERROR al intentar conectar con -> "<<host<<":"<<port;
    }
}

void Client::connected()
{
    qDebug()<<"CONECTADO a -> "<<host<<":"<<port;
}

void Client::disconnected()
{
     qDebug()<<"DESCONECTADO de -> "<<host<<":"<<port;
}

void Client::bytesWritten(qint64 bytes)
{
    qDebug()<<"Bytes escritos"<<bytes;
}
