#include "stand_capturadorimagen.h"

using namespace STAND;

void capturadorImagen::run()
{
   // if(modo_elegido==Modo_Video && vc.isOpened())
   // {
        while(1)
        {
            {
                QMutexLocker locker(&m_mutex);
                if (thread_stop) break;
                ///////////////////////////
                switch(modo_elegido)
                {
                    case Modo_Video:
                        vc >> Imagen;
                        this->tell();
                    break;

                    case Modo_ImagenStatica:
                        this->tell();
                    break;
                }
                ///////////////////////////
            }
            msleep( modo_elegido == Modo_Video? 1:150 );
        }
    //}
}

capturadorImagen::capturadorImagen(int modo, int devise)
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
        break;
    }
}

capturadorImagen::~capturadorImagen()
{
    stop();
    vc.release();
}

Mat capturadorImagen::getImagen()
{
    return Imagen;
}

void capturadorImagen::stop()
{
    QMutexLocker locker(&m_mutex);
    thread_stop = true;
}

bool capturadorImagen::isCamaraAbierta()
{
    return modo_elegido==Modo_Video? vc.isOpened() : true;
}

