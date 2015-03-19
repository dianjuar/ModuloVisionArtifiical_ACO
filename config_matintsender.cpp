#include "config_matintsender.h"

using namespace CONFIG;

matIntSender::matIntSender(QString serverDir):senderBase(serverDir,port)
{

}

void matIntSender::enviarInformacion(int **mat, int n, float dist)
{
    QString sms = senderBase::MSJEnvio_Prefijo_Mat;
    sms.append(IntMat2QString(mat,n));

    sms.append( senderBase::MSJEnvio_divisor );

    sms.append(senderBase::MSJEnvio_Prefijo_Dist);
    sms.append( QString::number(dist) );

    enviar( sms.toUtf8().data() );
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
