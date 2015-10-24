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

    static void dibujarPunto(Mat &m, Point p, Scalar color = Scalar(255,255,255));

    static void DetectarCirculos(Mat mat, vector<Vec3f> &ListacirculosDetectados, int n=2, bool dibujar=true);

    static void dibujarRecta(Mat &mat, math::lineaRecta linea, bool colorRojo=true, bool dibujarCentro = true);
    static void dibujarCirculo(Mat &mat,
                               Point center,
                               int radio,
                               int BaseAngle, int startAngle, int endAngle,
                               Scalar color = Scalar( 255, 0, 0 ),
                               int grosor = 2);

    //R1 siempre será la recta del robot
    static void dibujarAnguloEntreRectas(Mat &mat, math::lineaRecta R1, math::lineaRecta R2, float teta, Scalar color = Scalar(255,0,0));

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

    static const int NORTE    =0;
    static const int NOR_ESTE =1;
    static const int ESTE     =2;
    static const int SUR_ESTE =3;
    static const int SUR      =4;
    static const int SUR_OESTE=5;
    static const int OESTE    =6;
    static const int NOR_OESTE=7;
};

///////////////////////////////////////////
    namespace math
    {
        const int Cuadrante_I   = 1;
        const int Cuadrante_II  = 2;
        const int Cuadrante_III = 3;
        const int Cuadrante_IV  = 4;

        class circulo
        {
        public:
            Point centro;
            int radio;

            circulo(Point centro, int radio);

            static bool isCircleInsideOther(Tools::math::circulo c1, Tools::math::circulo c2);

            circulo();
        };
        /////////////////////
        double distanciaEntre2Puntos(Point2f A, Point2f B);
        bool PointAisCloserTo(Point A, Point B, Point Destino);
        int cuadranteDeUnPunto(Point2f p);
        /////////////////////
        class lineaRecta
        {
            void calcularDistancia();

            //para el calculo del angulo entre las 2 rectas es necesario ordenar primero las rectas.
        public:
            lineaRecta(float m, float b, Point2f A, Point2f B);
            lineaRecta(float m, float b);
            //lineaRecta(float m, float b, );
            lineaRecta(Point2f A, Point2f B);
            lineaRecta();

            float m;
            float b;
            float distanciaDelaRecta;
            Point2f puntoMedio;
            Point2f A,B;

            float puntoEnY(float puntoX);
            float puntoEnX(float puntoY);

            static void OrganizarRectas(lineaRecta &R1, lineaRecta &R2);
            static lineaRecta ejeX();
            static lineaRecta ejeX(Point puntoMedio);
            static bool isRectaR1(lineaRecta Recta ,lineaRecta const R1, lineaRecta const R2 );
            static float anguloEntre2Rectas(lineaRecta lA, lineaRecta lB,
                                            bool radianes=true,
                                            bool dibujar=false,
                                            Mat *m = NULL, Scalar color = Scalar(255,0,0));

            bool isM_positivo();

            bool operator==(const lineaRecta& other);
            void operator=(const lineaRecta& other);
        };
    }
//////////////////////////////////////////
}


#endif // CV_TOOLS_H
