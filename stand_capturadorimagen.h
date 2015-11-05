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

    static Mat Imagen;
    static Mat Imagen_Procesada;

    capturadorImagen(int modo=0, int devise=-1);
    ~capturadorImagen();

    int get_device(){ return devise; }
    bool isCamaraAbierta();    
    void deviceChanged(int newDevice);

    void write(FileStorage &fs) const; //Write serialization for this class
    void read(const FileNode& node);

public slots:
        void stop();
        void InicicarHilo();

signals:
        void tell();
        void devise_Changed(Mat &frame);
private:

    //sincronización del hilo
    QMutex m_mutex;
    bool thread_stop;

    VideoCapture vc;    

    //funcionamiento interno
    int devise;

    void readImage();
    QImage Mat2QImage(Mat m);
    void run();
    void constructor(int modo, int devise);

};

}

#endif // STAND_CAPTURADORIMAGEN_H
