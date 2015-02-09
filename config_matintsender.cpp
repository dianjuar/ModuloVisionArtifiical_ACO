#include "config_matintsender.h"

using namespace CONFIG;

matIntSender::matIntSender(QObject *parent) :
    QObject(parent)
{
    serverPort = 6666;
    buenaConexion = false;

    MSJ_sinComprobar = "No se ha comprobado el estado de la conexión.";
    MSJ_comprobando = "Comprobando el estado de la conexión.";
    MSJ_correcto = "Estado de conexión CORRECTO!.";
    MSJ_incorrecto = "Estado de conexión INCORRECTO!.";

    RUTAIMG_correcto = "./media/TestConnection/Right.png";
    RUTAIMG_incorrecto = "./media/TestConnection/Bad.png";
    RUTAIMG_comprobando = "./media/TestConnection/connecting.gif";

}

void matIntSender::testConnection(QString hostDir, bool Testserio)
{
    set_serverDir(hostDir);

    client.connectToHost(serverDir,serverPort);

    if(client.waitForConnected(3000))//connected
        buenaConexion = true;
    else
        buenaConexion = false;

    if(!Testserio)
    {
        client.write("test");
        client.disconnectFromHost();
    }
}

void matIntSender::enviarMatriz(QString hostDir,int **mat, int n)
{
    testConnection(hostDir,true);

    client.write( QByteArray( IntMat2QString(mat,n).toUtf8().data() ) );
    qDebug()<<QByteArray( IntMat2QString(mat,n).toUtf8().data() ).length();
    client.disconnectFromHost();
}

QString matIntSender::IntMat2QString(int **mat, int n)
{
    QString matQSt;

    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            matQSt.append(QString::number( mat[i][j] ));
        }

        matQSt.append( i!=n-1?"\n":"");
    }

    return matQSt;
}
