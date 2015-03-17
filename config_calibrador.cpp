#include "config_calibrador.h"

using namespace CONFIG;

//int calibrador::mode_STANDBAY;

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

                    if(mode) mode = mode_CALIBRATED; else mode = mode_STANDBAY;

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

bool calibrador::runAndSave()
{
    vector<Mat> rvecs, tvecs;

    bool ok = runCalibration( rvecs, tvecs);

    todoEnOrden = ok;

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
