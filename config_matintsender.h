#ifndef CONFIG_MATINTSENDER_H
#define CONFIG_MATINTSENDER_H

#include "INCLUDE_QTstuff.h"

namespace CONFIG
{

class matIntSender : public QObject
{
    Q_OBJECT
public:
    explicit matIntSender(QObject *parent = 0);

    void testConnection(QString hostDir, bool Testserio=false);
    void enviarMatriz(QString hostDir, int **mat, int n);

    void set_serverDir(QString serverDir){this->serverDir = serverDir;}
    void set_buenaConexion(bool buenaConexion){ this->buenaConexion = buenaConexion; }

    bool get_buenaConexion(){ return buenaConexion; }

    QString MSJ_sinComprobar;
    QString MSJ_comprobando;
    QString MSJ_correcto;
    QString MSJ_incorrecto;

    QString RUTAIMG_correcto;
    QString RUTAIMG_incorrecto;
    QString RUTAIMG_comprobando;

signals:

public slots:

private:
    QTcpSocket client;
    QString serverDir;



    int serverPort;
    bool buenaConexion;
    QString IntMat2QString(int **mat, int n);


};

}
#endif // CONFIG_MATINTSENDER_H
