#ifndef STAND_CAPTURADORIMAGEN_H
#define STAND_CAPTURADORIMAGEN_H

#include "INCLUDE_opencv.h"
#include "INCLUDE_QTstuff.h"

namespace STAND
{

class capturadorImagen : public QThread
{
    Q_OBJECT
public:
    static int const Modo_ImagenStatica=0;
    static int const Modo_Video=1;
    static int modo_elegido;

    capturadorImagen(int modo=0, int devise=-1);
    ~capturadorImagen();

    Mat getImagen();

    int get_device(){ return devise; }
    bool isCamaraAbierta();    
    void deviceChanged(int newDevice);


public slots:
        void stop();
        void InicicarHilo();

signals:
        void tell();
private:

    //sincronización del hilo
    QMutex m_mutex;
    bool thread_stop;

    VideoCapture vc;
    Mat Imagen;

    //funcionamiento interno
    int devise;

    void readImage();
    QImage Mat2QImage(Mat m);
    void run();

};

}

#endif // STAND_CAPTURADORIMAGEN_H
