#ifndef CONFIG_MATCARTOONERED_H
#define CONFIG_MATCARTOONERED_H

#include "INCLUDE_opencv.h"
#include "INCLUDE_QTstuff.h"

namespace CONFIG
{

class INTMatBuilder:public QObject
{
    Q_OBJECT
public:
    INTMatBuilder(Mat *mat_original_BlackAndWhite,int n, int *tamano_MatrizCropped);
    INTMatBuilder(){}
    ~INTMatBuilder(){}

    static int const MAPA_libre=0;
    static int const MAPA_obstaculo=1;
    static int const MAPA_inicio=2;
    static int const MAPA_fin=3;

    //setter
    void set_n(int n);
    void set_P_Fin(Point Fin);
    void set_P_Inicio(Point Inicio);

    //getter
    Mat get_MatCartooned(){  return mat_cartooned; }
    int** get_INT_mat(){ return INT_mat; }
    int get_tamano_MatCartooned(){ return tamano_MatCartooned; }
    int get_n(){ return n; }
    bool get_settedPuntoF(){ return bool_settedPuntoF; }
    bool get_settedPuntoI(){ return bool_settedPuntoI; }
    bool get_todoEnOrden(){ return bool_settedPuntoF&&bool_settedPuntoI; }

private slots:

signals:
    void settedPuntoF(bool);
    void settedPuntoI(bool);

private:
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

    bool contieneNegro(Mat m);

    Point P_Inicio, P_Fin;

    void set_TamanosYEscalas();

    void copiar_CuadroMatCartoon_a_MatCartoon(int i, int j, int valor);
    void crear_MartCartooned();

    Point convert_PointScreen2PointMat(Point p);

    bool bool_settedPuntoF, bool_settedPuntoI;
};

}
#endif // CONFIG_MATCARTOONERED_H
