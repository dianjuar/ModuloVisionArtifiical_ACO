#include "config_senderBase.h"

using namespace CONFIG;

const QString senderBase::MSJ_sinComprobar = QString("No se ha comprobado el estado de la conexión.");
const QString senderBase::MSJ_comprobando = QString("Comprobando el estado de la conexión.");
const QString senderBase::MSJ_correcto = QString("Estado de conexión CORRECTO!.");
const QString senderBase::MSJ_incorrecto = QString("Estado de conexión INCORRECTO!.");
const QString senderBase::RUTAIMG_correcto = QString("./media/TestConnection/Right.png");
const QString senderBase::RUTAIMG_incorrecto = QString("./media/TestConnection/Bad.png");
const QString senderBase::RUTAIMG_comprobando = QString("./media/TestConnection/connecting.gif");

const QString senderBase::MSJEnvio_divisor = QString("->");
const QString senderBase::MSJEnvio_Prefijo_Mat = QString("Mat");
const QString senderBase::MSJEnvio_Prefijo_Dist = QString("Dist");
const QString senderBase::MSJEnvio_conectado = QString("connect");
const QString senderBase::MSJEnvio_cerrar = QString("close");

senderBase::senderBase(QString serverDir, int serverPort)
{
    this->serverDir = serverDir;
    this->serverPort = serverPort;
    buenaConexion=false;
}

void senderBase::testConnection( bool Testserio)
{
    client.connectToHost(serverDir,serverPort);

    if(client.waitForConnected(3000))//connected
        buenaConexion = true;
    else
        buenaConexion = false;

    if(!Testserio)
    {
        client.write(MSJEnvio_cerrar.toUtf8().data());
        client.disconnectFromHost();
    }
}

void senderBase::enviar(QByteArray byte)
{
    testConnection(true);
    client.write( byte );
}
