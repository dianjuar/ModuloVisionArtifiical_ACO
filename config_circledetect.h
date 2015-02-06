#ifndef CONFIG_CIRCLEDETECT_H
#define CONFIG_CIRCLEDETECT_H

#include "INCLUDE_opencv.h"
#include "INCLUDE_QTstuff.h"

namespace CONFIG
{

class circleDetect
{
    int HOUGH_min_dist;
    int HOUGH_param_1;
    int HOUGH_param_2;
    int HOUGH_min_radius;
    int HOUGH_max_radius;

public:
    circleDetect();

    //set y get
    int get_HOUGH_min_dist(){ return HOUGH_min_dist; }
    int get_HOUGH_param_1(){ return HOUGH_param_1; }
    int get_HOUGH_param_2(){ return HOUGH_param_2; }
    int get_HOUGH_min_radius(){ return HOUGH_min_radius; }
    int get_HOUGH_max_radius(){ return HOUGH_max_radius; }
    void set_HOUGH_min_dist( int i){ HOUGH_min_dist = i; }
    void set_HOUGH_param_1( int i){ HOUGH_param_1 = i; }
    void set_HOUGH_param_2( int i){ HOUGH_param_2 = i; }
    void set_HOUGH_min_radius( int i){ HOUGH_min_radius = i; }
    void set_HOUGH_max_radius( int i){ HOUGH_max_radius = i; }
};

}
#endif // CONFIG_CIRCLEDETECT_H
