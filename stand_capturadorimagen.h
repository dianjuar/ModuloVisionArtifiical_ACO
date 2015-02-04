#ifndef STAND_CAPTURADORIMAGEN_H
#define STAND_CAPTURADORIMAGEN_H

#include "INCLUDE_opencv.h"
#include "INCLUDE_QTstuff.h"

class STAND_capturadorImagen : public QThread
{
    Q_OBJECT
public:
    static int const Modo_ImagenStatica=0;
    static int const Modo_Video=1;

    STAND_capturadorImagen(int modo=0, int devise=-1);
    STAND_capturadorImagen(){}
    ~STAND_capturadorImagen();

    Mat getImagen_MatCV();

    bool isCamaraAbierta();

public slots:
        void stop();

signals:
        void tell_Mat(QImage);
private:

    //sincronización del hilo
    QMutex m_mutex;
    bool thread_stop;

    VideoCapture vc;
    Mat Imagen;

    //funcionamiento interno
    int devise;
    int modo_elegido;

    void readImage();
    QImage Mat2QImage(Mat m);
    void run();

};

#endif // STAND_CAPTURADORIMAGEN_H
