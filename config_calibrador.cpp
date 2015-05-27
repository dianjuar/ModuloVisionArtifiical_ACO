#include "config_calibrador.h"

using namespace CONFIG;
using namespace std;

#include <iostream>
using std::count;
using std::endl;

calibrador::calibrador()
{
    todoEnOrden = false;
}

void calibrador::set_distanciaEntreCuadros(int distanciaEntreCuadros_PX, int n)
{
    int n2 = n/2;

    Point A(n2*distanciaEntreCuadros_PX,n2*distanciaEntreCuadros_PX);
    Point B(n2*distanciaEntreCuadros_PX, (n2+1)*distanciaEntreCuadros_PX);

    Point RealPointA = pixelPoint2realPoint(A);
    Point RealPointB = pixelPoint2realPoint(B);

    distanciaEntreCuadros_REAL = distanciaEntreDosPuntosReales(RealPointA, RealPointB);
}

float calibrador::get_distanciaEntreCuadros_REAL()
{
    return distanciaEntreCuadros_REAL;
}

void calibrador::write(FileStorage &fs) const //Write serialization for this class
{
    fs << "{" <<  "C33" << C33 <<
                  "C22" << C22 <<
                  "C13" << C13 <<
                  "C32" << C32 <<
                  "C12" << C12 <<
                  "C33" << C33 <<
                  "C23" << C23 <<
                  "C11" << C11 <<
                  "C31" << C31 <<
                  "C21" << C21 <<
                  "distanciaEntreCuadros_REAL" << distanciaEntreCuadros_REAL <<
          "}";
}

void calibrador::read(const FileNode& node)                          //Read serialization for this class
{
    C33 = (float)node["C33"];
    C22 = (float)node["C22"];
    C13 = (float)node["C13"];
    C32 = (float)node["C32"];
    C12 = (float)node["C12"];
    C33 = (float)node["C33"];
    C23 = (float)node["C23"];
    C11 = (float)node["C11"];
    C31 = (float)node["C31"];
    C21 = (float)node["C21"];

    distanciaEntreCuadros_REAL = (float)node["distanciaEntreCuadros_REAL"];
}

void calibrador::set_rutaDelArchivo(QString path)
{
    this->rutaDelArchivo = path;

    Mat intrinsic, distortion_coefficients, bigMatExtrinc, distortionMat;

    FileStorage fs;
    todoEnOrden = fs.open(rutaDelArchivo.toUtf8().data() ,FileStorage::READ);

    fs["camera_matrix"] >> intrinsic;
    fs["distortion_coefficients"] >> distortion_coefficients;
    fs["extrinsic_parameters"] >> bigMatExtrinc;

    Mat lastExtrinc = bigMatExtrinc.row( bigMatExtrinc.rows -1 ); //obtiene la ultima linea de la matriz
    Mat rvec = lastExtrinc.colRange(0,3);
    Mat tvecAux = lastExtrinc.colRange(3,6);
    Mat tvec = Mat(3,1,tvecAux.type());

    for (int var = 0; var < 3; var++)
        tvec.at<double>(var,0) = tvecAux.at<double>(0,var);

    Mat R;
    Rodrigues(rvec, R);

    cv::Mat T = Mat::zeros(3, 3, intrinsic.type()); // T is 4x4
    T( cv::Range(0,3), cv::Range(0,2) ) = R(cv::Range(0,3), cv::Range(0,2)) * 1; // copies R into T
    T( cv::Range(0,3), cv::Range(2,3) ) = tvec * 1; // copies tvec into T

    distortionMat = T;

    Mat matConst = intrinsic*distortionMat;

    C33 = matConst.at<double>( 3-1 , 3-1 );
    C22 = matConst.at<double>( 2-1 , 2-1 );
    C13 = matConst.at<double>( 1-1 , 3-1 );
    C32 = matConst.at<double>( 3-1 , 2-1 );
    C12 = matConst.at<double>( 1-1 , 2-1 );
    C33 = matConst.at<double>( 3-1 , 3-1 );
    C23 = matConst.at<double>( 2-1 , 3-1 );
    C11 = matConst.at<double>( 1-1 , 1-1 );
    C31 = matConst.at<double>( 3-1 , 1-1 );
    C21 = matConst.at<double>( 2-1 , 1-1 );

    fs.release();
}

Point calibrador::pixelPoint2realPoint(Point Ppx)
{

    double denominador = C11*C22 - C11*C32*Ppx.y - C31*C22*Ppx.x -C12*C21 + C12*C31*Ppx.y + C32*C21*Ppx.x;

    double numeradorX = C33*C22*Ppx.x - C13*C22 + C13*C32*Ppx.y - C12*C33*Ppx.y + C12*C23 - C32*C23*Ppx.x;
    double numeradorY = C11*C33*Ppx.y - C11*C23 + C23*C31*Ppx.x - C21*C33*Ppx.x + C21*C13 - C13*C31*Ppx.y;

    Point realP( numeradorX/denominador ,
                 numeradorY/denominador);

    return realP;
}

double calibrador::distanciaEntreDosPuntosReales(Point P_RealA, Point P_RealB)
{
    return sqrt( qPow(P_RealA.x - P_RealB.x, 2) + qPow(P_RealA.y - P_RealB.y, 2) );
}
