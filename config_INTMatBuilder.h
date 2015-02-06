#ifndef CONFIG_MATCARTOONERED_H
#define CONFIG_MATCARTOONERED_H

#include "INCLUDE_opencv.h"

namespace CONFIG
{

class INTMatBuilder
{
    Mat *mat_original_BlackAndWhite;
    int **INT_mat;
    int *n;
    int tamano_cuadroAnalizar_MatrizCroped;

    void construir_INTMat_and_cartoon();

    bool contieneNegroLosPuntos_InicioFin();
    bool contieneNegro(Mat m);

    Point *P_Inicio, *P_Fin;

    bool contieneError;

public:
    INTMatBuilder(Mat *mat_original_BlackAndWhite,int *n);
    void calibrar();

    static int const MAPA_libre=0;
    static int const MAPA_obstaculo=1;
    static int const MAPA_inicio=2;
    static int const MAPA_fin=3;

    void set_n(int *n);
    void set_P_InicioYFin(Point *Inicio, Point *Fin);
};

}
#endif // CONFIG_MATCARTOONERED_H
