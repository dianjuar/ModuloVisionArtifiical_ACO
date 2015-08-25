#ifndef CONFIG_COLORDETECTOR_H
#define CONFIG_COLORDETECTOR_H

#include "INCLUDE_opencv.h"
#include "INCLUDE_QTstuff.h"
#include "stand_Tools.h"


namespace CONFIG
{

class sesgo
{

public:
    sesgo();

    double h_h, l_h, h_s, l_s;

    double get_h_h(){ return h_h; }
    double get_l_h(){ return l_h; }
    double get_h_s(){ return h_s; }
    double get_l_s(){ return l_s; }

    double *get_h_h_DIRMEM(){ return &h_h; }
    double *get_l_h_DIRMEM(){ return &l_h; }
    double *get_h_s_DIRMEM(){ return &h_s; }
    double *get_l_s_DIRMEM(){ return &l_s; }

    void setValues(double h_h, double l_h, double h_s, double l_s);

};

class colorDetector : public QThread
{
    //estos 5 valores fueron sacados despues de varias pruebas en el laboratorio de prototipos y fueron los que mejores resultado arrojaron
    int low_diff;
    int high_diff;
    int conn;
    int val;
    int flags;

    bool selected;

    sesgo *sesX;
    Point seedPoint;

    Mat frame_hsv, frame, mask;
    Mat frame_thresholded;
    Mat frame_sesgado;

    Mat kernel_rectangular;
    Mat kernel_ovalado;

    bool cortado;

    void recortar();

public:
    colorDetector();

    void write(FileStorage &fs) const;
    void read(const FileNode& node);

    void calibrar(Mat m, int Nsesgo);

    //setters
    void setSesgos(int Nsesgo,double h_h, double l_h, double h_s, double l_s);
    void set_seedPoint(int Nsesgo, Point p);

    //getters
    Mat get_m_sesgado(){ return frame_sesgado; }
    Mat get_m_thresholded(){ return frame_thresholded; }
    bool get_cortado(){ return cortado; }

    void run();

};

}
#endif // CONFIG_COLORDETECTOR_H
