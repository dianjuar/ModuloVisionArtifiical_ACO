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

void matIntSender::write(FileStorage &fs) const
{
    fs<<"{"<< "serverDir" << serverDir.toUtf8().data()<<"}";
}

void matIntSender::read(const FileNode &node)
{
    std::string server;

    node["serverDir"] >> server;

    serverDir = QString::fromUtf8( server.c_str() );
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
