#ifndef CONFIG_PARTIRNCUADROS_H
#define CONFIG_PARTIRNCUADROS_H

#include "INCLUDE_opencv.h"

namespace CONFIG
{

class partirNcuadros
{
    int n;
    int tamano_CuadroPartido_N;
    int *tamano_XxX_MatrizCroped;
public:
    partirNcuadros(int n, int *tamano_XxX_MatrizCroped);
    void calibrar(Mat &mat_original);

    void set_n(int n){ this->n = n;}
    int get_n(){ return n; }
};

}
#endif // CONFIG_PARTIRNCUADROS_H
