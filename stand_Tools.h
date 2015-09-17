#ifndef CV_TOOLS_H
#define CV_TOOLS_H

#include "INCLUDE_opencv.h"
#include "INCLUDE_QTstuff.h"

namespace Tools
{

class OpenCV
{
public:
    static Mat tratamientoDeImagenStantdar(Mat source, bool withGaussianBlur=true); //pasar a escala de grises y aplicar guassianBlur

    static QPixmap Mat2QPixmap(Mat m, bool resize = false, int x=400);

    static QPixmap Mat2QPixmap(Mat m, int scale);

    static Rect contenedorMasGrande( vector< vector<Point> > contours);
};

class Cfunctions
{
public:
    static QString IntMat2QString(int **mat, int n);

    static std::vector<std::string> split(std::string s, const std::string delim);
    static std::vector<QString> split(QString s, const QString delim);
};

/*class circulo
{
public:
    Point centro;
    int radio;

    circulo(Point centro, int radio)
    {
        this->centro = centro;
        this->radio = radio;
    }

    circulo(){ radio = -1; }
};*/

}


#endif // CV_TOOLS_H
