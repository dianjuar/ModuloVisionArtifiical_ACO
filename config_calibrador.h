#ifndef CONFIG_CALIBRAR_H
#define CONFIG_CALIBRAR_H

#include "INCLUDE_opencv.h"
#include "INCLUDE_QTstuff.h"
#include "time.h"
#include "stand_capturadorimagen.h"

namespace CONFIG
{

class calibrador : public QThread
{
    Q_OBJECT

public:

    // Size(ui->slider_BoardSizeW_F1_1->value(), ui->slider_BoardSizeH_F1_1->value()), ui->slider_NFotos_F1_1->value()
    calibrador(float squareSize, Size boardSize, int NFotos, int delay);
    ~calibrador();

    void set_boardSize(Size boardSize);
    void set_delay(int delay);
    void set_NFotos(int NFotos){ this->NFotos = NFotos; }
    void set_squareSize(float squareSize){ this->squareSize = squareSize; }

    void set_calibrar();
    void set_standbay();

    bool get_todoEnOrden(){ return todoEnOrden; }

    Mat get_m(){ return m; }

public slots:
    void stop();
    void InicicarHilo(int device);

signals:
    void tell();
    void fotoTomada(int);
    void CalibracionExitosa(bool);

private :

    std::vector<Point2f> pointbuf;
    std::vector<std::vector<Point2f> > imagePoints;

    //sincronización del hilo
    QMutex m_mutex;
    bool thread_stop;
    bool todoEnOrden;


    Size boardSize;
    Size imageSize;
    int NFotos;
    int fotoActual;
    int delay;
    float squareSize;
    int mode;

    static const int mode_STANDBAY = 0;
    static const int mode_CAPTURING = 1;
    static const int mode_CALIBRATED = 2;


    Mat m;
    Mat viewGray;
    clock_t prevTimestamp;

    //variables resultantes de la calibracion
    Mat cameraMatrix;
    Mat distCoeffs;
    Mat rMat, tMat;
    float reprojErr;
    double totalAvgErr;
    float aspectRatio;
    std::vector<float> reprojErrs;

    VideoCapture vc;

    void run();

    bool runAndSave();
    bool runCalibration( std::vector<Mat> &rvecs, std::vector<Mat> &tvecs );
    void calcChessboardCorners( std::vector<Point3f>& corners);
    double computeReprojectionErrors( const std::vector<vector<Point3f> >& objectPoints,
                                      const std::vector<Mat>& rvecs, const std::vector<Mat>& tvecs,
                                      std::vector<float>& perViewErrors );


};

}
#endif // CONFIG_CALIBRAR_H
