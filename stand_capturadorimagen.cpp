#include "stand_capturadorimagen.h"

using namespace STAND;

int capturadorImagen::modo_elegido;
Mat capturadorImagen::Imagen_P = Mat::zeros(20,20, CV_8UC3 );

void capturadorImagen::run()
{
    for(;;)
    {
        {
            QMutexLocker locker(&m_mutex);
            if (thread_stop)
                break;
            ///////////////////////////
            if( modo_elegido == Modo_Video && vc.isOpened() )
                vc >> Imagen;

            emit tell();
            ///////////////////////////
        }
        msleep( modo_elegido == Modo_Video? 1:150 );
    }
}

void capturadorImagen::constructor(int modo, int devise)
{
    thread_stop = false;

    this->modo_elegido = modo;
    this->devise = devise;

    switch(modo_elegido)
    {
        case Modo_ImagenStatica:
            Imagen = imread("./media/static.png");
        break;

        case Modo_Video:
            vc.open(devise);
            vc.set(CV_CAP_PROP_FRAME_WIDTH , 640);
            vc.set(CV_CAP_PROP_FRAME_HEIGHT , 480);
        break;
    }

    start();
}

capturadorImagen::capturadorImagen(int modo, int devise)
{
    constructor(modo, devise);
}

capturadorImagen::~capturadorImagen()
{
    stop();
    vc.release();
}

void capturadorImagen::stop()
{
    //QMutexLocker locker(&m_mutex);
    thread_stop = true;

    while( this->isRunning() ){ }
    //if( vc.isOpened() )
        vc.release();
}

void capturadorImagen::InicicarHilo()
{
    vc.open( devise );
    thread_stop = false;
    start();
}

bool capturadorImagen::isCamaraAbierta()
{
    return modo_elegido==Modo_Video? vc.isOpened() : true;
}

void capturadorImagen::deviceChanged(int newDevice)
{
    if(modo_elegido == Modo_Video)
    {        
        if(vc.isOpened())
        {
            QMutexLocker locker(&m_mutex);
            vc.release();
        }

        vc.open(newDevice);
        devise = newDevice;
    }
}

void capturadorImagen::write(FileStorage &fs) const
{
    fs << "{" << "modo_elegido" << modo_elegido <<
                 "devise" << devise <<
          "}";
}

void capturadorImagen::read(const FileNode& node)                          //Read serialization for this class
{
    stop();

    int devise = (int)node["devise"];
    int modo_elegido = (int)node["modo_elegido"];

    constructor(modo_elegido, devise  );
}
