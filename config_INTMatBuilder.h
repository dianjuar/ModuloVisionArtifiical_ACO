#ifndef CONFIG_MATCARTOONERED_H
#define CONFIG_MATCARTOONERED_H

#include "INCLUDE_opencv.h"
#include "INCLUDE_QTstuff.h"

namespace CONFIG
{

class INTMatBuilder
{
    Mat *mat_original_BlackAndWhite;
    Mat mat_cartooned;

    Mat MAT_libre,
        MAT_obstaculo,
        MAT_inicio,
        MAT_fin;

    int **INT_mat;
    int n;

    int tamano_cuadroAnalizar_MatrizCroped;
    int tamano_MatCartooned;
    int tamano_imagenCartoonOriginal;
    int tamano_cuadroMatCartooned;
    int *tamano_MatrizCropped;

    void construir_INTMat_and_cartoon();

    bool Validos_PuntosInicioFin();
    bool contieneNegro(Mat m);

    Point *P_Inicio, *P_Fin;

    bool contieneError;
    void set_TamanosYEscalas();

    void copiar_CuadroMatCartoon_a_MatCartoon(int i, int j, int valor);
    void crear_MartCartooned();

public:
    INTMatBuilder(Mat *mat_original_BlackAndWhite,int n, int *tamano_MatrizCropped);
    void calibrar();

    static int const MAPA_libre=0;
    static int const MAPA_obstaculo=1;
    static int const MAPA_inicio=2;
    static int const MAPA_fin=3;

    //setter
    void set_n(int n);
    void set_P_InicioYFin(Point *Inicio, Point *Fin);

    //getter
    bool get_contieneError(){ return contieneError; }
    Mat get_MatCartooned(){  return mat_cartooned; }
    int** get_INT_mat(){ return INT_mat; }
    int get_tamano_MatCartooned(){ return tamano_MatCartooned; }
    int get_n(){ return n; }
};

}
#endif // CONFIG_MATCARTOONERED_H
