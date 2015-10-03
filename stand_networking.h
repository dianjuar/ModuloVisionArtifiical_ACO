#ifndef STAND_DATASEND_H
#define STAND_DATASEND_H

#include "INCLUDE_QTstuff.h"
#include "stand_capturadorimagen.h"
#include "INCLUDE_opencv.h"
#include "stand_Tools.h"

namespace Network
{

class DataSend
{
private:
    QTcpSocket *socket;

public:
    DataSend(QTcpSocket *socket);
    DataSend(){}
    void enviar(QByteArray byte);
    void enviar(QString s);
};

/////////////////////////////////////////////////////
class DataRecibe: public QObject
{
    Q_OBJECT
private:
    QTcpSocket *socket;

    virtual void AnalizadorDeMensajes(QString msj){}

public slots:
    void readyRead();

public:
    DataRecibe(QTcpSocket *socket);
    DataRecibe(){}
    void run();
};

/////////////////////////////////////////////////////
class Client: public Network::DataRecibe, public Network::DataSend
{
    Q_OBJECT
private:
   QTcpSocket socket;

protected:
   QString host;
   int port;
   bool connected_B;

public:
   Client(QString host, int port);
   void connectToHost();

   //setters
   void set_host(QString host){ this->host = host; }

   //getters
   bool isConnected(){ return connected_B; }

signals:

public slots:
   void	connected();
   void	disconnected();
   void	bytesWritten(qint64 bytes);
};

}
/////////////////////////////////////////////////////
namespace Tools
{
    namespace Network
    {
        class GestionDeMensajes
        {
        public:

            ///Mensajes Standar
            static const QString Msj_divisor;
            static const QString Msj_divisor_2;
            static const QString Msj_cerrar;
            static const QString Msj_conectado;
            static QString Enviar_MSJ_conectado();
            ///Mensajes TO ACO
            static const QString MSJEnvio_Prefijo_Mat;
            static const QString MSJEnvio_Prefijo_Dist;
            ///Mensajes TO SMA
            static const QString Msj_solicitudTrayectoria;
            static const QString Msj_TrayectoriaCorrected;
            static QString Enviar_TOSMA_MSJ_TrayectoriaCorrected(int RobotID, float teta);
        };
    }// namespaceNetwork
}// namespaceTools
#endif // STAND_DATASEND_H
