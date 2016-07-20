#include "config_calibrador.h"

using namespace CONFIG;
using namespace std;

double calibrador::C33;
double calibrador::C22;
double calibrador::C13;
double calibrador::C32;
double calibrador::C12;
double calibrador::C23;
double calibrador::C11;
double calibrador::C31;
double calibrador::C21;

double calibrador::K1;
double calibrador::K2;

Mat calibrador::intrinsic;
Mat calibrador::distortion_coefficients;

calibrador::calibrador()
{
    todoEnOrden = false;
}

void calibrador::set_distanciaEntreCuadros(int distanciaEntreCuadros_PX, int n)
{
    int n2 = n/2;

    Point A(n2*distanciaEntreCuadros_PX,n2*distanciaEntreCuadros_PX);
    Point B(n2*distanciaEntreCuadros_PX, (n2+1)*distanciaEntreCuadros_PX);

    distanciaEntreCuadros_REAL = distanciaReal_2PuntosPixeles(A, B, false);
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
                  "C23" << C23 <<
                  "C11" << C11 <<
                  "C31" << C31 <<
                  "C21" << C21 <<
                  "intrinsic"<< intrinsic<<
                  "K1"  << K1 <<
                  "K2"  << K2 <<
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
    C23 = (float)node["C23"];
    C11 = (float)node["C11"];
    C31 = (float)node["C31"];
    C21 = (float)node["C21"];

    distanciaEntreCuadros_REAL = (float)node["distanciaEntreCuadros_REAL"];

    node["intrinsic"] >> intrinsic;
    K1 = (double)node["K1"];
    K2 = (double)node["K2"];
}

bool calibrador::set_rutaDelArchivo(QString path)
{
    this->rutaDelArchivo = path;

    Mat bigMatExtrinc, distortionMat;

    FileStorage fs;
    todoEnOrden = fs.open(rutaDelArchivo.toUtf8().data() ,FileStorage::READ);

        if ( !todoEnOrden ) return false;

    fs["camera_matrix"] >> intrinsic;
    fs["distortion_coefficients"] >> distortion_coefficients;
    K1 = distortion_coefficients.at<double>(0,0);
    K2 = distortion_coefficients.at<double>(1,0);

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

    return todoEnOrden;
}

Point2f calibrador::pixelPoint2realPoint(Point2f Ppx, bool corregir)
{
    if( corregir)
        Ppx = corregirDistorcion( Ppx );

    double denominador = C11*C22 - C11*C32*Ppx.y - C31*C22*Ppx.x -C12*C21 + C12*C31*Ppx.y + C32*C21*Ppx.x;

    double numeradorX = C33*C22*Ppx.x - C13*C22 + C13*C32*Ppx.y - C12*C33*Ppx.y + C12*C23 - C32*C23*Ppx.x;
    double numeradorY = C11*C33*Ppx.y - C11*C23 + C23*C31*Ppx.x - C21*C33*Ppx.x + C21*C13 - C13*C31*Ppx.y;

    Point2f realP( numeradorX/denominador ,
                 numeradorY/denominador);

    return realP;
}

double calibrador::distanciaReal_2PuntosReales(Point2f P_RealA, Point2f P_RealB)
{
    return Tools::math::distanciaEntre2Puntos(P_RealA, P_RealB);
}

double calibrador::distanciaReal_2PuntosPixeles(Point Px_A, Point Px_B, bool corregir)
{
    Point P_RealA = pixelPoint2realPoint(Px_A, corregir);
    Point P_RealB = pixelPoint2realPoint(Px_B, corregir);

    return distanciaReal_2PuntosReales(P_RealA, P_RealB);
}

Point2f calibrador::corregirDistorcion( Point2f p)
{
    //normalizr puntos
    Point2f pNorm = normalizarPunto(p);

    Point2f pCorrectedNormalized = corregirDistorcion_Radial(pNorm);

    desnormalizarPunto(pCorrectedNormalized);

    return p;
}

Point2f calibrador::normalizarPunto(Point2f p)
{
    Mat pNormalized = Mat(3,1, intrinsic.type());
    pNormalized.at<double>(0,0) = p.x;
    pNormalized.at<double>(1,0) = p.y;
    pNormalized.at<double>(2,0) = 1;

    //normalized points
    pNormalized = intrinsic.inv() * pNormalized;

    p.x = pNormalized.at<double>(0,0);
    p.y = pNormalized.at<double>(1,0);

    //cout<<pNormalized.at<double>(0, 0)<<"  "<<pNormalized.at<double>(0,1)<<endl;

    return p;
}

Point2f calibrador::corregirDistorcion_Radial(Point2f pNorm)
{
    //this number is common in the two functions
    double common = K1*( qPow(pNorm.x,2) + qPow(pNorm.y,2) ) + K2*qPow( qPow(pNorm.x,2) + qPow(pNorm.y,2) , 2);

    double pxCorrectedNormalized = pNorm.x + pNorm.x*common;
    double pyCorrectedNormalized = pNorm.y + pNorm.y*common;
    Point2f pCorrectedNormalized( pxCorrectedNormalized, pyCorrectedNormalized );

    return pCorrectedNormalized;
}

Point2f calibrador::desnormalizarPunto(Point2f p)
{
    Mat correctedNormalized = Mat(3,1, intrinsic.type());

    correctedNormalized.at<double>(0,0) = p.x;
    correctedNormalized.at<double>(1,0) = p.y;
    correctedNormalized.at<double>(2,0) = 1;

    Mat pCorrected = intrinsic * correctedNormalized;

    p.x = pCorrected.at<double>(0,0);
    p.y = pCorrected.at<double>(1,0);

    return p;

}

