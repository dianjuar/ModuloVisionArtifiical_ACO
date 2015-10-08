#ifndef CV_TOOLS_H
#define CV_TOOLS_H

#include "INCLUDE_opencv.h"
#include "INCLUDE_QTstuff.h"

namespace Tools
{

//class prototyping *************************
class OpenCV;
class general;

namespace math
{
    class circulo;
    class lineaRecta;
}
//class prototyping *************************


class OpenCV
{
public:
    static Mat tratamientoDeImagenStantdar(Mat source, bool withGaussianBlur=true); //pasar a escala de grises y aplicar guassianBlur

    static QPixmap Mat2QPixmap(Mat m, bool resize = false, int x=400);

    static QPixmap Mat2QPixmap(Mat m, int scale);

    static Rect contenedorMasGrande( vector< vector<Point> > contours);

    static vector<Vec3f> DetectarCirculos(Mat mat, vector<Vec3f> ListacirculosDetectados,int n=2, bool dibujar=true);

    static void dibujarRecta(Mat &mat, math::lineaRecta linea, bool colorRojo=true, bool dibujarCentro = true);
    static void dibujarCirculo(Mat &mat, Point center, int radio, int BaseAngle, int startAngle, int endAngle);
    //R1 siempre será la recta del robot
    static void anguloEntreRectas(Mat &mat, math::lineaRecta R1, math::lineaRecta R2, float &teta, float &anguloInicial, bool dibujar = true);

private:
    static void dibujarCirculos(Mat mat, vector<Vec3f> circles);
};

////////////////////////////////////////////

class general
{
public:
    static bool DEBUG;
    static QString IntMat2QString(int **mat, int n);

    static std::vector<std::string> split(std::string s, const std::string delim);
    static std::vector<QString> split(QString s, const QString delim);
};

///////////////////////////////////////////
    namespace math
    {
        class circulo
        {
        public:
            Point centro;
            int radio;

            circulo(Point centro, int radio);

            circulo();
        };
        /////////////////////
        class lineaRecta
        {
            void calcularDistancia();

            //para el calculo del angulo entre las 2 rectas es necesario ordenar primero las rectas.
        public:
            lineaRecta(float m, float b, Point A, Point B);
            lineaRecta(float m, float b);
            //lineaRecta(float m, float b, );
            lineaRecta(Point A, Point B);
            lineaRecta(){}

            float m;
            float b;
            float distanciaDelaRecta;
            Point puntoMedio;
            Point A,B;

            float puntoEnY(float puntoX);
            float puntoEnX(float puntoY);

            static void OrganizarRectas(lineaRecta &R1, lineaRecta &R2);
            static float anguloEntre2Rectas(lineaRecta lA, lineaRecta lB);

            bool operator==(const lineaRecta& other);
        };
    }
//////////////////////////////////////////
}


#endif // CV_TOOLS_H
