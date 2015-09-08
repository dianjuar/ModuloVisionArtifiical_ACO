#ifndef CONFIG_PARTIRNCUADROS_H
#define CONFIG_PARTIRNCUADROS_H

#include "config_cropper.h"
#include "INCLUDE_opencv.h"
#include "INCLUDE_QTstuff.h"

namespace CONFIG
{

class partirNcuadros
{
    int n;
    int tamano_CuadroPartido_N;

    bool esNecesarioOtroCuadro;
    bool primeraVez;
    int cuantosCuadrosSonNecesarios;
public:
    partirNcuadros(int n);
    void calibrar(Mat &mat_original);

    //setter
    void set_n(int n);

    //getter
    int get_n(){ return n; }
    int get_cuantosCuadrosSonNecesarios(){ return cuantosCuadrosSonNecesarios; }
    int get_distanciaEnPXentreCuadros(){ return tamano_CuadroPartido_N; }
    bool get_esNecesarioOtroCuadro(){ return esNecesarioOtroCuadro; }
};

}
#endif // CONFIG_PARTIRNCUADROS_H
