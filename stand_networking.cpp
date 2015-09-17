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
/////////////////////////////////////////////////////////s
void DataRecibe::stop()
{
    pararHilo = true;
}

DataRecibe::DataRecibe(QTcpSocket *socket)
{
    this->socket = socket;
    pararHilo = false;
}

void DataRecibe::run()
{
    QString msj;

    while( !pararHilo)
    {
        socket->waitForBytesWritten();
        socket->waitForReadyRead();

        QByteArray buff;
        buff = socket->readAll();
        msj = QString( buff );

        qDebug()<<("*******************");
        qDebug()<<("String recibido...");
        qDebug()<<(msj);
        qDebug()<<("*******************");

        AnalizadorDeMensajes(msj);
    }
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
///Mensajes TO ACO
const QString GestionDeMensajes::MSJEnvio_Prefijo_Mat = "Mat";
const QString GestionDeMensajes::MSJEnvio_Prefijo_Dist = "Dist";
///Mensajes TO SMA
const QString GestionDeMensajes::Msj_solicitudTrayectoria = "CorrectMe";
/////////////////////////////////////////////////////////
DataClient::DataClient(QString host, int port):
    DataSend(&socket),
    DataRecibe(&socket)
{
    connected = false;
    this->host = host;
    this->port = port;
}

void DataClient::connectToHost(bool iniciarEscucha)
{
    socket.connectToHost(host, port);

    if(socket.waitForConnected(3000))//connected
    {
        connected = true;
        enviar( Tools::Network::GestionDeMensajes::Enviar_MSJ_conectado() );

        if(iniciarEscucha)
            start();
    }
    else
        connected = false;
}



