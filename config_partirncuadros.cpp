#include "config_partirncuadros.h"

using namespace CONFIG;

partirNcuadros::partirNcuadros(int n, int *tamano_XxX_MatrizCroped)
{
    this->n = n;
    this->tamano_XxX_MatrizCroped = tamano_XxX_MatrizCroped;
}

void partirNcuadros::calibrar(Mat &mat_original)
{
    tamano_CuadroPartido_N = *tamano_XxX_MatrizCroped/ n;

    Scalar color = Scalar(0,255,0);

    for(int i=1; i<n;i++)
    {
        //los horizontales
        Point pH1 = Point(0,tamano_CuadroPartido_N*i);
        Point pH2 = Point(*tamano_XxX_MatrizCroped,tamano_CuadroPartido_N*i);
        line(mat_original,pH1,pH2, color,2);

        for(int j=1; j<n;j++)
        {
            Point pV1 = Point(tamano_CuadroPartido_N*i,0);
            Point pV2 = Point(tamano_CuadroPartido_N*i,*tamano_XxX_MatrizCroped);

        line(mat_original,pV1,pV2, color,2);
        }
    }
}

