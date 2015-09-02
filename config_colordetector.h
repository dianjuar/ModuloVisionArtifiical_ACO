#ifndef CONFIG_COLORDETECTOR_H
#define CONFIG_COLORDETECTOR_H

#include "INCLUDE_opencv.h"
#include "INCLUDE_QTstuff.h"
#include "stand_Tools.h"


namespace CONFIG
{

class colorDetector
{
    //estos 5 valores fueron sacados despues de varias pruebas en el laboratorio de prototipos y fueron los que mejores resultado arrojaron
    static int low_diff;
    static int high_diff;
    static int conn;
    static int val;
    static int flags;

    static Mat kernel_rectangular;
    static Mat kernel_ovalado;

    int NumeroDeColores;

    void recortar();

    class sesgador : public QThread
    {
    private:

        Mat frame;
        Mat frame_hsv;
        Mat mask;
        Mat frame_thresholded;
        Mat frame_sesgado;
        bool selected;

        QMutex sync;

        double h_h, l_h, h_s, l_s;

    public:
        sesgador();

        double get_h_h(){ return h_h; }
        double get_l_h(){ return l_h; }
        double get_h_s(){ return h_s; }
        double get_l_s(){ return l_s; }

        void setValues(double h_h, double l_h, double h_s, double l_s);

        void calibrar(Mat m);
        void recortar();

        //setters
        void set_seedPoint(Point p);

        //getters
        Mat get_frame_thresholded(){ return frame_thresholded; }
        Mat get_frame_sesgado(){ return frame_sesgado; }

        void run();
    };

public:
    colorDetector();

    void write(FileStorage &fs) const;
    void read(const FileNode& node);

    void calibrar(Mat m, int Nsesgo);

    sesgador *sesgador3colores;
};

}
#endif // CONFIG_COLORDETECTOR_H
