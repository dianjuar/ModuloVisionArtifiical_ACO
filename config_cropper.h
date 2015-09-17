#ifndef CONFIG_CROPPER_H
#define CONFIG_CROPPER_H

#include "INCLUDE_opencv.h"
#include "stand_Tools.h"
#include "stand_capturadorimagen.h"

namespace CONFIG
{

class cropper
{
    Mat imagen_Rayada;
    Mat canny_edges;
    Rect contenedor;
    bool hay_Contenedor;

    int canny_umbral_1, canny_umbral_2;

    void set_MayorContenedor(Rect r);

public:
    cropper(int canny_umbral_1, int canny_umbral_2);
    cropper(){}

    static int tamano_MatrizCortada;
    //setter y getters
    void set_cannyU_1(int);
    void set_cannyU_2(int);
    Mat get_ImagenRayada(){ return imagen_Rayada; }
    Mat get_ImagenCanny(){ return canny_edges; }
    bool hayContenedor(){  return hay_Contenedor; }

    void calibracion(Mat mat);
    void cortarImagen(Mat &);
    void reset_contenedor();

    void write(FileStorage &fs) const;
    void read(const FileNode& node);

};

}

#endif // CONFIG_CROPPER_H
