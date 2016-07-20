#ifndef CONFIG_CALIBRAR_H
#define CONFIG_CALIBRAR_H

#include "config_configbase.h"
#include "INCLUDE_QTstuff.h"
#include "time.h"
#include "stand_capturadorimagen.h"
#include "stand_Tools.h"

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

    static Point2f pixelPoint2realPoint(Point2f Ppx, bool corregir=true);
    static double distanciaReal_2PuntosReales(Point2f P_RealA, Point2f P_RealB );
    static double distanciaReal_2PuntosPixeles(Point Px_A, Point Px_B, bool corregir=true);

    float get_distanciaEntreCuadros_REAL();

    void write(FileStorage &fs) const;
    void read(const FileNode& node);

private :
    bool todoEnOrden;
    float distanciaEntreCuadros_REAL;
    QString rutaDelArchivo;

    static double C22, C13, C32, C12, C33, C23;
    static double C11, C31, C21;

    static double K1, K2;
    static Mat distortion_coefficients, intrinsic;

    static Point2f corregirDistorcion(Point2f p);
    static Point2f normalizarPunto( Point2f p );
    static Point2f corregirDistorcion_Radial(Point2f pNorm  );
    static Point2f desnormalizarPunto(Point2f p);
};

}
#endif // CONFIG_CALIBRAR_H
