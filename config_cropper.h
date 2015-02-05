#ifndef CONFIG_CROPPER_H
#define CONFIG_CROPPER_H

#include "INCLUDE_opencv.h"
#include "stand_Tools.h"

namespace CONFIG
{

class cropper
{
    Mat imagen_Rayada;
    Rect contenedor;
    bool hay_Contenedor;

    int canny_umbral_1, canny_umbral_2;
    Rect contenedorMasGrande( vector< vector<Point> > contours );

    int tamano_XxX_MatrizCroped;

public:
    cropper(int canny_umbral_1, int canny_umbral_2);

    void set_cannyU_1(int);
    void set_cannyU_2(int);
    Mat get_ImagenRayada(){ return imagen_Rayada; }
    int get_tamano_MatrizCroped(){ return tamano_XxX_MatrizCroped; }
    bool hayContenedor(){  return hay_Contenedor; }

    void calibracion(Mat mat);
    void cortarImagen(Mat &);

};

}

#endif // CONFIG_CROPPER_H
