#ifndef STAND_DATASEND_H
#define STAND_DATASEND_H

#include "INCLUDE_QTstuff.h"
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
    void cerrarConexion();
    void enviar(QByteArray byte);
    void enviar(QString s);
};

/////////////////////////////////////////////////////
class DataRecibe : public QThread
{
private:
    static const int MaxBufferSize = 1024000;
    QTcpSocket *socket;
    bool pararHilo;

    virtual void AnalizadorDeMensajes(QString msj){}
public slots:
    void stop();
signals:

public:
    DataRecibe(QTcpSocket *socket);
    DataRecibe(){}
    void cerrarConexion();
    void run();
};

/////////////////////////////////////////////////////
class DataClient: public Network::DataSend, public Network::DataRecibe
{
private:
   QTcpSocket socket;

   DataSend send;
   DataRecibe recibe;

protected:
   QString host;
   int port;
   bool connected;

public:
   DataClient(QString host, int port);
   void connectToHost(bool iniciarEscucha=false);


   void set_host(QString host){ this->host = host; }
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
    };

    }// namespaceNetwork
}// namespaceTools
#endif // STAND_DATASEND_H
