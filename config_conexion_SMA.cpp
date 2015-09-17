#include "config_conexion_SMA.h"

using namespace CONFIG::Network;

void conexion_SMA::AnalizadorDeMensajes(QString msj)
{
    vector<QString> msjDividido = Tools::Cfunctions::split(msj, Tools::Network::GestionDeMensajes::Msj_divisor );

    QString encabezado = msjDividido.at(0);
    QString cuerpo = msjDividido.at(1);

    //si le solicitan corregir trayectoria.
    if( QString::compare(encabezado, Tools::Network::GestionDeMensajes::Msj_solicitudTrayectoria, Qt::CaseInsensitive ) == 0 )
    {
        /*SolCorTray -> RobotID _ DireccionNominal _ RobotPointNominal
         *si RobotPoint(-1,-1) es la primera vez que se solicita por lo tanto no se sabe cual es.*/

        vector<QString> cuerpoVec = Tools::Cfunctions::split( cuerpo, Tools::Network::GestionDeMensajes::Msj_divisor_2 );

        int RobotID = QString(cuerpoVec.at(0)).toInt();
        int direccionRobot_Nominal = QString(cuerpoVec.at(1)).toInt();
        Point RobotPoint_Nominal(QString(cuerpoVec.at(2)).toInt(),
                                 QString(cuerpoVec.at(3)).toInt());

        if(RobotPoint_Nominal.x == -1 && RobotPoint_Nominal.y == -1) //primera vez
            RobotPoint_Nominal = INTMatBuilder::P_Inicio;

        colorD->sesgador3colores[RobotID].corregirTrayectoria(direccionRobot_Nominal,RobotPoint_Nominal);
    }
}

conexion_SMA::conexion_SMA(QString serverDir, coTra::colorDetector *colorD):DataClient(serverDir,port)
{
    this->colorD = colorD;

   /* for (int i = 0; i < colorD->get_numeroDecolores(); i++)
        connect( colorD->sesgador3colores, SIGNAL(correccionDeTrayectoriaProcesada(int,float,float)),
                 this, SLOT(recibirResultado_SolicitudCorreccionDeTrayectoria(int,float,float)) )*/

}

void conexion_SMA::write(FileStorage &fs) const
{
    fs<<"{"<< "serverDir" << host.toUtf8().data()<<"}";
}

void conexion_SMA::read(const FileNode &node)
{
    std::string server;

    node["serverDir"] >> server;

    host = QString::fromUtf8( server.c_str() );
}

void conexion_SMA::recibirResultado_SolicitudCorreccionDeTrayectoria(int RobotID, float correc_grados, float correc_dist)
{

}
