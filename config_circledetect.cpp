#include "config_circledetect.h"

using namespace CONFIG;

circleDetect::circleDetect(int HOUGH_min_dist, int HOUGH_param_1, int HOUGH_param_2, int HOUGH_min_radius, int HOUGH_max_radius)
{
    calibracionCorrecta = false;
    this->HOUGH_min_dist = HOUGH_min_dist;
    this->HOUGH_param_1 = HOUGH_param_1;
    this->HOUGH_param_2 = HOUGH_param_2;
    this->HOUGH_min_radius = HOUGH_min_radius;
    this->HOUGH_max_radius = HOUGH_max_radius;
}

Point circleDetect::get_Inicio()
{
    bool c1_esMayor = c1->radio > c2->radio; //el circulo mayor es el de llegada

   return c1_esMayor ? c2->centro: c1->centro;
}

Point circleDetect::get_Fin()
{
    bool c1_esMayor = c1->radio > c2->radio; //el circulo mayor es el de llegada

    return c1_esMayor ? c1->centro: c2->centro;
}

void circleDetect::calibrar(Mat  &mat_original)
{
    vector<Vec3f> circulosDetectados;

    HoughCircles( STAND::Tools::tratamientoDeImagenStantdar(mat_original),
                  circulosDetectados, CV_HOUGH_GRADIENT, 1,
                  HOUGH_min_dist, HOUGH_param_1, HOUGH_param_2, HOUGH_min_radius, HOUGH_max_radius );

     //circulo c1,c2;

     for( size_t i = 0; i < circulosDetectados.size(); i++ )
     {
        Point center(cvRound(circulosDetectados[i][0]), cvRound(circulosDetectados[i][1]));
        int radius = cvRound(circulosDetectados[i][2]);

        if(circulosDetectados.size()==2)
        {
            if(i==0)
                c1 = new STAND::circulo(center,radius);
            else
                c2= new STAND::circulo(center,radius);
        }

        //dibujar circulos
        // circle center
        circle( mat_original, center, 4, Scalar(255,0,0), -1);
        // circle outline
        circle( mat_original, center, radius, Scalar(0,0,255), 2);
      }

     if( circulosDetectados.size() != 2 )
     {
         qDebug()<<"No se ha detectado los circulos necesarios (2).  \n Modifique los parámetros para detectarlos";
         calibracionCorrecta = false;
     }
     else //cuando se detectan 2 ciruclos
     {
        calibracionCorrecta = true;
        /* int C1ii = c1.centro.y/tamano_cuadroAnalizar_MatrizCroped;
         int C1jj = c1.centro.x/tamano_cuadroAnalizar_MatrizCroped;
         int C2ii = c2.centro.y/tamano_cuadroAnalizar_MatrizCroped;
         int C2jj = c2.centro.x/tamano_cuadroAnalizar_MatrizCroped;

             bool c1_esMayor = c1.radio > c2.radio; //el circulo mayor es el de llegada

             INT_mat[C1ii][C1jj] = c1_esMayor ? MAPA_fin:MAPA_inicio;
             INT_mat[C2ii][C2jj] = c1_esMayor ? MAPA_inicio:MAPA_fin;*/
     }

}
