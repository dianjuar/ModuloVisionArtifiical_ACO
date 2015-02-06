#ifndef CONFIG_CIRCLEDETECT_H
#define CONFIG_CIRCLEDETECT_H

#include "INCLUDE_opencv.h"
#include "INCLUDE_QTstuff.h"
#include "stand_Tools.h"

namespace CONFIG
{
class circleDetect
{
    int HOUGH_min_dist;
    int HOUGH_param_1;
    int HOUGH_param_2;
    int HOUGH_min_radius;
    int HOUGH_max_radius;

    bool calibracionCorrecta;
    STAND::circulo *c1,*c2;

public:
    circleDetect(int HOUGH_min_dist,
                 int HOUGH_param_1,int HOUGH_param_2,
                 int HOUGH_min_radius,int HOUGH_max_radius);

    // get
    int get_HOUGH_min_dist(){ return HOUGH_min_dist; }
    int get_HOUGH_param_1(){ return HOUGH_param_1; }
    int get_HOUGH_param_2(){ return HOUGH_param_2; }
    int get_HOUGH_min_radius(){ return HOUGH_min_radius; }
    int get_HOUGH_max_radius(){ return HOUGH_max_radius; }
    bool get_calibracionCorrecta(){ return calibracionCorrecta; }
    Point get_Inicio();
    Point get_Fin();
    Point* get_PuntoInicio_SEGUIMIENTO();
    Point* get_PuntoFin_SEGUIMIENTO();

    //set
    void set_HOUGH_min_dist( int i){ HOUGH_min_dist = i; }
    void set_HOUGH_param_1( int i){ HOUGH_param_1 = i; }
    void set_HOUGH_param_2( int i){ HOUGH_param_2 = i; }
    void set_HOUGH_min_radius( int i){ HOUGH_min_radius = i; }
    void set_HOUGH_max_radius( int i){ HOUGH_max_radius = i; }

    void calibrar(Mat &mat_original);
};

}
#endif // CONFIG_CIRCLEDETECT_H
