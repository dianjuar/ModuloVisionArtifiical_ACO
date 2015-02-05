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

public:
    cropper(int canny_umbral_1, int canny_umbral_2);
    cropper(){}

    void set_cannyU_1(int);
    void set_cannyU_2(int);

    void calibracion(Mat mat);
    void cortarImagen(Mat &);

    bool hayContenedor();
    Mat get_ImagenRayada();
};

}

#endif // CONFIG_CROPPER_H
