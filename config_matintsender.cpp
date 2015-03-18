#include "config_matintsender.h"

using namespace CONFIG;

matIntSender::matIntSender(QString serverDir):senderBase(serverDir,port)
{

}

void matIntSender::enviarMatriz(int **mat, int n)
{
    enviar( QByteArray( IntMat2QString(mat,n).toUtf8().data() ) );
}

void matIntSender::enviarDistanciaEntreCuadros(float dist)
{
    char *data;
    sprintf(data, "%f", dist);

    enviar( QByteArray(data) );
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
