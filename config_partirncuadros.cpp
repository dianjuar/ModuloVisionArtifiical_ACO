#include "config_partirncuadros.h"

using namespace CONFIG;

partirNcuadros::partirNcuadros(int n)
{
    this->n = n;
    primeraVez = true;
}

void partirNcuadros::calibrar(Mat &mat_original)
{
    if(primeraVez)
        set_n(n);

    Scalar color = Scalar(0,255,0);

    for(int i=1; i<n;i++)
    {
        //los horizontales
        Point pH1 = Point(0,tamano_CuadroPartido_N*i);
        Point pH2 = Point(cropper::tamano_MatrizCortada, tamano_CuadroPartido_N*i);
        line(mat_original,pH1,pH2, color,2);

        for(int j=1; j<n;j++)
        {
            Point pV1 = Point(tamano_CuadroPartido_N*i,0);
            Point pV2 = Point(tamano_CuadroPartido_N*i, cropper::tamano_MatrizCortada);

        line(mat_original,pV1,pV2, color,2);
        }
    }
}

void partirNcuadros::set_n(int n)
{
    this->n = n;

    tamano_CuadroPartido_N = cropper::tamano_MatrizCortada/ n;
    esNecesarioOtroCuadro = (cropper::tamano_MatrizCortada-tamano_CuadroPartido_N*n)>tamano_CuadroPartido_N;

    if(esNecesarioOtroCuadro)
    {
        cuantosCuadrosSonNecesarios = (cropper::tamano_MatrizCortada-tamano_CuadroPartido_N*n) / tamano_CuadroPartido_N;
        this->n += cuantosCuadrosSonNecesarios;
        tamano_CuadroPartido_N = cropper::tamano_MatrizCortada/ this->n;
        //qDebug()<<cuantosCuadrosSonNecesarios;
    }
    else
        cuantosCuadrosSonNecesarios = 0;

    primeraVez = false;
}
