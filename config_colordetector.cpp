#include "config_colordetector.h"

using namespace CONFIG;

//estos 5 valores fueron sacados despues de varias pruebas en el laboratorio de prototipos y fueron los que mejores resultado arrojaron
int colorDetector::low_diff = 20;
int colorDetector::high_diff = 10;
int colorDetector::conn = 4;
int colorDetector::val = 255;
int colorDetector::flags = conn + (val << 8) + CV_FLOODFILL_MASK_ONLY;
Mat colorDetector::kernel_rectangular = getStructuringElement(MORPH_RECT, Size(5, 5));
Mat colorDetector::kernel_ovalado = getStructuringElement(MORPH_ELLIPSE, Size(30, 30));


colorDetector::colorDetector()
{
    NumeroDeColores = 3;
    sesgador3colores = new sesgador[NumeroDeColores];
}

void colorDetector::write(FileStorage &fs) const
{
    fs << "{";

        fs<<"numeroDeColores" << NumeroDeColores;

        for (int i = 0; i < NumeroDeColores; i++)
        {
            double h_h = sesgador3colores[i].get_h_h();
            double l_h = sesgador3colores[i].get_l_h();
            double h_s = sesgador3colores[i].get_h_s();
            double l_s = sesgador3colores[i].get_l_s();

            QString sms = "Color";
            sms.append( QString::number(i) );

            fs << sms.toUtf8().data();
                fs << "{:";
                    fs << "h_h" << h_h;
                    fs << "l_h" << l_h;
                    fs << "h_s" << h_s;
                    fs << "l_s" << l_s;
                fs <<"}";
        }

    fs<< "}";
}

void colorDetector::read(const FileNode &node, FileStorage &fs)
{
    NumeroDeColores = (int)node["numeroDeColores"];

    for (int i = 0; i < NumeroDeColores; i++)
    {
        FileNode nn = fs[QString(i).toUtf8().data()];
        double h_h = (double)nn["h_h"];
        double l_h = (double)nn["l_h"];
        double h_s = (double)nn["h_s"];
        double l_s = (double)nn["l_s"];
        sesgador3colores[i].setValues(h_h,l_h,h_s,l_s);
    }
}

void colorDetector::calibrar(Mat m, int Nsesgo)
{
    sesgador3colores[Nsesgo].calibrar(m);
}
//*********************************************************************
//*********************************************************************
colorDetector::sesgador::sesgador()
{
    h_h = l_h = h_s = l_s = 0;
    selected = false;
    frame_sesgado = Mat::zeros( 20, 20, CV_8UC3 );
    frame_thresholded = Mat::zeros( 20, 20, CV_8UC3 );
}

void colorDetector::sesgador::setValues(double h_h, double l_h, double h_s, double l_s)
{
    this-> h_h = h_h;
    this-> l_h = l_h;
    this-> h_s = h_s;
    this-> l_s = l_s;
}

void colorDetector::sesgador::recortar()
{
    vector<vector<Point> > contornos;
    vector<Vec4i> hierarchy;

    findContours(frame_thresholded.clone(),contornos, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    Rect roi = STAND::Tools::contenedorMasGrande( contornos );
    frame_sesgado = frame(roi);
}

void colorDetector::sesgador::calibrar(Mat m)
{
    frame = m;

    if(!this->isRunning())
        start();
}

void colorDetector::sesgador::set_seedPoint(Point p)
{
    selected = true;

    // make mask using floodFill
    mask = Scalar::all(0);

    floodFill(frame, mask, p,
              Scalar(255, 255, 255), 0,
              Scalar(low_diff, low_diff, low_diff),
              Scalar(high_diff, high_diff, high_diff), flags);

    // find the H and S range of piexels selected by floodFill
    Mat channels[3];
    split(frame_hsv, channels);

    Mat InputMask = mask.rowRange(1, mask.rows-1).colRange(1, mask.cols-1);

    minMaxLoc(channels[0], &this->l_h, &this->h_h, NULL, NULL, InputMask );
    minMaxLoc(channels[1], &this->l_s, &this->h_s, NULL, NULL, InputMask );

}

void colorDetector::sesgador::run()
{
    //codigo sacado de la página 101 del libre Practical Opencv
   if(!selected)
   {
       sync.lock();
       mask.create(frame.rows+2, frame.cols+2, CV_8UC1);
       sync.unlock();
   }

   cvtColor(frame.clone(), frame_hsv, CV_BGR2HSV);

   // extract the hue and saturation channels
   int from_to[] = {0,0, 1,1};
   Mat hs(frame.size(), CV_8UC2);
   mixChannels(&frame_hsv, 1, &hs, 1, from_to, 2);

   // check for the range of H and S obtained from floodFill
   inRange(hs,
           Scalar( l_h, l_s ),
           Scalar( h_h, h_s ),
           frame_thresholded);

   // open and close to remove noise
   Mat aux = frame_thresholded.clone();//esto se hace para que al usuario se muestre la umbralización con todas las operaciones morfoligicas aplicadas

   morphologyEx(aux, aux, MORPH_CLOSE, kernel_rectangular);
   morphologyEx(aux, aux, MORPH_OPEN, kernel_rectangular);

   morphologyEx(aux, aux, MORPH_OPEN, kernel_rectangular);
   morphologyEx(aux, aux, MORPH_CLOSE, kernel_rectangular);

   morphologyEx(aux, aux, MORPH_OPEN, kernel_ovalado);

   frame_thresholded = aux;


   recortar();
}
