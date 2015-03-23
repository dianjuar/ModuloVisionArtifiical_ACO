#include "config_calibrador.h"

using namespace CONFIG;
using namespace std;

calibrador::calibrador(float squareSize, Size boardSize, int NFotos, int delay)
{
    this->squareSize = squareSize;
    this->boardSize = boardSize;
    this->NFotos = NFotos;
    this->delay = delay*1000;

    thread_stop = false;
    todoEnOrden = false;
    mode = mode_STANDBAY;
    fotoActual=0;
    prevTimestamp = 0;

    if(STAND::capturadorImagen::modo_elegido == STAND::capturadorImagen::Modo_ImagenStatica)
        m = imread("./media/static.png");
    else    
        todoEnOrden=true;


}

calibrador::~calibrador()
{
    stop();
}

void calibrador::set_boardSize(Size boardSize)
{
    this->boardSize = boardSize;
}

void calibrador::set_delay(int delay)
{
    this->delay = delay*1000;
}

void calibrador::set_calibrar()
{
    mode = mode_CAPTURING;
    prevTimestamp = 0;

    todoEnOrden = false;
    imagePoints.clear();
    fotoActual = 0;
}

void calibrador::set_standbay()
{
    mode = mode_STANDBAY;
    fotoActual= 0;
    fotoTomada(0);
}

float calibrador::get_distanciaEntreCuadros()
{
    return distanciaEntreCuadros;
}

void calibrador::set_distanciaEntreCuadros(int distanciaEntreCuadros_PX, int n)
{
    int n2 = n/2;

    Point A(n2*distanciaEntreCuadros_PX,n2*distanciaEntreCuadros_PX);
    Point B(n2*distanciaEntreCuadros_PX, (n2+1)*distanciaEntreCuadros_PX);

    Point RealPointA = pixel2Point(A);
    Point RealPointB = pixel2Point(B);

    distanciaEntreCuadros = get_distanciaEntrePuntos(RealPointA, RealPointB);

}

Point calibrador::point2Pixel(Point RealPoint)
{
    Mat WordlVec = Mat::zeros(4,1,cameraMatrix.type());

    WordlVec.at<double>(0,0)= RealPoint.x;
    WordlVec.at<double>(1,0)= RealPoint.y;
    WordlVec.at<double>(2,0)= 0.0;
    WordlVec.at<double>(3,0)= 1;


    Mat pixel = cameraMatrix*distortionMat*WordlVec;

    return  Point(pixel.at<double>(0,0),
                  pixel.at<double>(1,0));
}

Point calibrador::pixel2Point(Point Ppx, bool undistorsioned)
{
    if(!undistorsioned)
    Ppx = undistortionedPoints( Ppx );

    Mat matConst = cameraMatrix*distortionMat;

    double C22, C13, C32, C12, C33, C23;
    double C11, C31, C21;

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

    double denominador = C11*C22 - C11*C32*Ppx.y - C31*C22*Ppx.x -C12*C21 + C12*C31*Ppx.y + C32*C21*Ppx.x;

    double numeradorX = C33*C22*Ppx.x - C13*C22 + C13*C32*Ppx.y - C12*C33*Ppx.y + C12*C23 - C32*C23*Ppx.x;
    double numeradorY = C11*C33*Ppx.y - C11*C23 + C23*C31*Ppx.x - C21*C33*Ppx.x + C21*C13 - C13*C31*Ppx.y;

    Point ret( numeradorX/denominador ,
               numeradorY/denominador);


    return ret;
}

double calibrador::get_distanciaEntrePuntos(Point RealA, Point RealB)
{
    return qSqrt( qPow(RealA.x - RealB.x, 2) + qPow(RealA.y - RealB.y, 2) );
}

void calibrador::write(FileStorage &fs) const //Write serialization for this class
{
    fs << "{" << "cameraMatrix" << cameraMatrix <<
                 "distCoeffs" << distCoeffs <<
                  "rMat" << rMat << "tMat" << tMat <<
                  "distortionMat" << distortionMat <<
                  "reprojErr" << reprojErr << "totalAvgErr" << totalAvgErr << "aspectRatio" << aspectRatio << "reprojErrs" << reprojErrs <<
                  "K1" << K1 << "K2" << K2 << "K3" << K3 <<
          "}";
}

void calibrador::read(const FileNode& node)                          //Read serialization for this class
{
    node["cameraMatrix"] >> cameraMatrix;
    node["distCoeffs"] >> distCoeffs;
    node["rMat"] >> rMat; node["tMat"] >> tMat;
    node["distortionMat"] >> distortionMat ;

    reprojErr = (float)node["reprojErr"];
    totalAvgErr = (double)node["totalAvgErr"];
    aspectRatio = (float)node["aspectRatio"];
    node["reprojErr"] >> reprojErrs;
    K1 = (float)node["K1"];
    K2 = (float)node["K2"];
    K3 = (float)node["K3"];
}

void calibrador::stop()
{
    QMutexLocker locker(&m_mutex);
    thread_stop = true;
    vc.release();
}

void calibrador::InicicarHilo(int device)
{
    thread_stop = false;

    vc.open(device);

    Mat aux;
    vc >> aux;

    imageSize = aux.size();

    start();
}

void calibrador::run()
{    
    for(;;)
    {
        {

            QMutexLocker locker(&m_mutex);
            if (thread_stop) break;
            ///////////////////////////
            if(STAND::capturadorImagen::modo_elegido == STAND::capturadorImagen::Modo_Video)
            {
                vc >> m;
                cvtColor(m, viewGray, COLOR_BGR2GRAY);
                bool blink = false;


                bool found;

                found = findChessboardCorners( viewGray, boardSize, pointbuf,
                                               CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FAST_CHECK | CV_CALIB_CB_NORMALIZE_IMAGE);

                // improve the found corners' coordinate accuracy
                if(found)
                    cornerSubPix( viewGray, pointbuf, Size(11,11),
                                  Size(-1,-1), TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));

                if( mode == mode_CAPTURING &&
                        found &&
                        clock() - prevTimestamp > delay*1e-3*CLOCKS_PER_SEC)
                {
                    imagePoints.push_back(pointbuf);
                    prevTimestamp = clock();
                    blink = true;
                    fotoTomada( ++fotoActual );
                }

                if(found)
                    drawChessboardCorners( m, boardSize, Mat(pointbuf), found );

                if( blink )
                    bitwise_not(m, m);

                /*
                if( mode == mode_CALIBRATED && undistortImage )
                {
                    Mat temp = view.clone();
                    undistort(temp, view, cameraMatrix, distCoeffs);
                }
                */

                this->tell();

                if( mode == mode_CAPTURING && imagePoints.size() == NFotos )
                {
                    bool ok = runAndSave();

                    if(ok) mode = mode_CALIBRATED; else mode = mode_STANDBAY;

                    CalibracionExitosa(ok);
                }
            }
            else
                this->tell();
            ///////////////////////////
        }
        msleep( STAND::capturadorImagen::modo_elegido == STAND::capturadorImagen::Modo_Video? 1:150 );
    }
}

void calibrador::set_Ks()
{
    K1 = distCoeffs.at<double>(0,0);
    K2 = distCoeffs.at<double>(0,1);
    K3 = distCoeffs.at<double>(0,2);
}

void calibrador::set_distortionMat()
{
    Mat rvecAux = Mat(1,3,tMat.type());

    for (int var = 0; var < 3; var++)
        rvecAux.at<double>(0,var) = rMat.at<double>(var,0);

    Mat R;
    Rodrigues(rvecAux, R);

    cout<<R<<endl;
    cout<<tMat<<endl;
    cout<<"****************"<<endl;


    cv::Mat T = Mat::zeros(3, 3, cameraMatrix.type()); // T is 4x4

    T( cv::Range(0,3), cv::Range(0,2) ) = R(cv::Range(0,3), cv::Range(0,2)) * 1; // copies R into T
    T( cv::Range(0,3), cv::Range(2,3) ) = tMat * 1; // copies tvec into T

    cout<<T<<endl;

    distortionMat = T;
}

bool calibrador::runAndSave()
{
    vector<Mat> rvecs, tvecs;

    bool ok = runCalibration( rvecs, tvecs);

    todoEnOrden = ok;

    if(ok)
    {
        set_rMat( rvecs );
        set_tMat( tvecs );
        set_distortionMat();
        set_Ks();
    }

    return ok;
}

bool calibrador::runCalibration(std::vector<Mat> &rvecs, std::vector<Mat> &tvecs)
{
    cameraMatrix = Mat::eye(3, 3, CV_64F);
    distCoeffs = Mat::zeros(8, 1, CV_64F);
    std::vector<std::vector<Point3f> > objectPoints(1);

    calcChessboardCorners(objectPoints[0]);

    objectPoints.resize(imagePoints.size(),objectPoints[0]);

    double rms = calibrateCamera(objectPoints, imagePoints, imageSize, cameraMatrix,
                    distCoeffs, rvecs, tvecs, CV_CALIB_FIX_K4|CV_CALIB_FIX_K5);
                    ///*|CV_CALIB_FIX_K3*/|CV_CALIB_FIX_K4|CV_CALIB_FIX_K5);
    printf("RMS error reported by calibrateCamera: %g\n", rms);

    bool ok = checkRange(cameraMatrix) && checkRange(distCoeffs);

    totalAvgErr = computeReprojectionErrors(objectPoints, rvecs, tvecs,reprojErrs);

    return ok;
}

void calibrador::calcChessboardCorners(vector<Point3f> &corners)
{
    corners.resize(0);

    for( int i = 0; i < boardSize.height; i++ )
        for( int j = 0; j < boardSize.width; j++ )
            corners.push_back( Point3f(float(j*squareSize), float(i*squareSize), 0));

}

double calibrador::computeReprojectionErrors(const std::vector< std::vector<Point3f> > &objectPoints,
                                             const std::vector<Mat> &rvecs, const std::vector<Mat> &tvecs,
                                             std::vector<float> &perViewErrors)
{
    std::vector<Point2f> imagePoints2;
    int i, totalPoints = 0;
    double totalErr = 0, err;
    perViewErrors.resize(objectPoints.size());

    for( i = 0; i < (int)objectPoints.size(); i++ )
    {
        projectPoints(Mat(objectPoints[i]), rvecs[i], tvecs[i],
                      cameraMatrix, distCoeffs, imagePoints2);
        err = norm(Mat(imagePoints[i]), Mat(imagePoints2), CV_L2);
        int n = (int)objectPoints[i].size();
        perViewErrors[i] = (float)qSqrt(err*err/n);
        totalErr += err*err;
        totalPoints += n;
    }

    return qSqrt(totalErr/totalPoints);
}

void calibrador::set_rMat(std::vector<Mat> rvecs)
{
    rMat = rvecs.at( rvecs.size()-1 );
}

void calibrador::set_tMat(std::vector<Mat> tvecs)
{
    tMat = tvecs.at( tvecs.size()-1 );
}

// Modelos computacionales en ingeniería
// desarrollos novedosos y aplicaciones
// Jesús Marcey García C.
Point calibrador::undistortionedPoints(Point Ppx)
{
    Mat normalVecPx(3,1,rMat.type());
    normalVecPx.at<double>(0,0) = Ppx.x;
    normalVecPx.at<double>(1,0) = Ppx.y;
    normalVecPx.at<double>(2,0) = 1;

    Mat normalizedVecPx = cameraMatrix.inv()*normalVecPx;

    double Udn,Vdn;
    double Uin,Vin;

    Udn = normalizedVecPx.at<double>(0,0);
    Vdn = normalizedVecPx.at<double>(0,1);

    Uin = Udn + Udn*( K1*( qPow(Udn,2) + qPow(Vdn,2) ) + K2*qPow((qPow(Udn,2) + qPow(Vdn,2)),2) + K3*qPow((qPow(Udn,2) + qPow(Vdn,2)),3) );
    Vin = Vdn + Vdn*( K1*( qPow(Udn,2) + qPow(Vdn,2) ) + K2*qPow((qPow(Udn,2) + qPow(Vdn,2)),2) + K3*qPow((qPow(Udn,2) + qPow(Vdn,2)),3));

    normalizedVecPx.at<double>(0,0) = Uin;
    normalizedVecPx.at<double>(0,1) = Vin;

    Mat desnomalized_undistornioned_Points = cameraMatrix*normalizedVecPx;

    Point realPointsPx( desnomalized_undistornioned_Points.at<double>(0,0) ,
                        desnomalized_undistornioned_Points.at<double>(0,1));

    return realPointsPx;

}
