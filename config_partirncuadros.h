#ifndef CONFIG_PARTIRNCUADROS_H
#define CONFIG_PARTIRNCUADROS_H

#include "INCLUDE_opencv.h"
#include "INCLUDE_QTstuff.h"

namespace CONFIG
{

class partirNcuadros
{
    int n;
    int tamano_CuadroPartido_N;
    int *tamano_XxX_MatrizCroped;

    bool esNecesarioOtroCuadro;
    bool primeraVez;
    int cuantosCuadrosSonNecesarios;
public:
    partirNcuadros(int n, int *tamano_XxX_MatrizCroped);
    void calibrar(Mat &mat_original);

    //setter
    void set_n(int n);

    //getter
    int get_n(){ return n; }
    int* get_n_SEGUIMIENTO(){ return &n; }
    int get_cuantosCuadrosSonNecesarios(){ return cuantosCuadrosSonNecesarios; }
    bool get_esNecesarioOtroCuadro(){ return esNecesarioOtroCuadro; }
};

}
#endif // CONFIG_PARTIRNCUADROS_H
