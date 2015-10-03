#ifndef CONFIG_CALIBRAR_H
#define CONFIG_CALIBRAR_H

#include "config_configbase.h"
#include "INCLUDE_QTstuff.h"
#include "time.h"
#include "stand_capturadorimagen.h"

#include "iostream"

namespace CONFIG
{

class calibrador: public configBase
{

public:
    calibrador();
    ~calibrador();

    void set_distanciaEntreCuadros(int distanciaEntreCuadros_PX, int n);
    void set_distanciaEntreCuadros(float distancia){ this->distanciaEntreCuadros_REAL = distancia; }
    bool set_rutaDelArchivo( QString path );

    bool get_todoEnOrden(){ return todoEnOrden; }

    Point pixelPoint2realPoint(Point Ppx);
    double distanciaEntreDosPuntosReales(Point P_RealA, Point P_RealB );

    float get_distanciaEntreCuadros_REAL();

    void write(FileStorage &fs) const;
    void read(const FileNode& node);

private :
    bool todoEnOrden;
    float distanciaEntreCuadros_REAL;
    QString rutaDelArchivo;

    double C22, C13, C32, C12, C33, C23;
    double C11, C31, C21;
};

}
#endif // CONFIG_CALIBRAR_H
