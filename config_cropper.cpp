#include "config_cropper.h"

using namespace CONFIG;

void cropper::set_cannyU_1(int c)
{
    canny_umbral_1 =c;
}
void cropper::set_cannyU_2(int c)
{
    canny_umbral_2 =c;
}

void cropper::calibracion(Mat mat)
{
    imagen_Rayada = mat.clone();

    Mat matTratada,canny_edges;

    matTratada = STAND::Tools::tratamientoDeImagenStantdar(imagen_Rayada);

    Canny(matTratada, canny_edges, canny_umbral_1, canny_umbral_2);
    vector< vector<Point> > contours;
    findContours(canny_edges, contours,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    contenedor = contenedorMasGrande(contours);

    rectangle(imagen_Rayada,contenedor,Scalar(0,0,255), 2);
}

Rect cropper::contenedorMasGrande(vector<vector<Point> > contours)
{
    if(contours.size()!=0)
    {
        int larguestArea = -1;
        int largest_contour_index;

        for( int i = 0; i< contours.size(); i++ )
        {
             double a = contourArea( contours[i]);

             if(a>larguestArea)
             {
                 larguestArea = a;
                 largest_contour_index=i;
             }
        }

        hay_Contenedor = true;
        return boundingRect(contours[ largest_contour_index ] );
    }
    else
    {
        hay_Contenedor = false;
        qDebug()<<"No se encontro un ningun contorno";
        return Rect();
    }

}

cropper::cropper(int canny_umbral_1, int canny_umbral_2)
{
    this->canny_umbral_1 = canny_umbral_1;
    this->canny_umbral_2 = canny_umbral_2;
    hay_Contenedor = false;
}

Mat cropper::get_ImagenRayada()
{
    return imagen_Rayada;
}

bool cropper::hayContenedor()
{
    return hay_Contenedor;
}


