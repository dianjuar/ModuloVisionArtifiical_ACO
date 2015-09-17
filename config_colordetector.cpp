#include "config_colordetector.h"

using namespace CONFIG;
using namespace coTra;

colorDetector::colorDetector()
{
    //estos 5 valores fueron sacados despues de varias pruebas en el laboratorio de prototipos y fueron los que mejores resultado arrojaron
    low_diff = 20;
    high_diff = 10;
    conn = 4;
    val = 255;
    flags = conn + (val << 8) + CV_FLOODFILL_MASK_ONLY;
    kernel_rectangular = getStructuringElement(MORPH_RECT, Size(5, 5));
    kernel_ovalado = getStructuringElement(MORPH_ELLIPSE, Size(30, 30));

    NumeroDeColores = 3;
    sesgador3colores = new sesgador[NumeroDeColores];

    for (int i = 0; i < NumeroDeColores; i++)
        sesgador3colores[i].set_parameters4sesgo(i+1,&low_diff,&high_diff,&conn,&val,&flags,&kernel_rectangular,&kernel_ovalado);

}

void colorDetector::write(FileStorage &fs) const
{
    fs << "{";

        fs<<"numeroDeColores" << NumeroDeColores;

        fs << "Colores"<<"[";
        for (int i = 0; i < NumeroDeColores; i++)
        {
            double h_h = sesgador3colores[i].get_h_h();
            double l_h = sesgador3colores[i].get_l_h();
            double h_s = sesgador3colores[i].get_h_s();
            double l_s = sesgador3colores[i].get_l_s();

            fs << "{:";
                fs << "h_h" << h_h;
                fs << "l_h" << l_h;
                fs << "h_s" << h_s;
                fs << "l_s" << l_s;
            fs <<"}";
        }
        fs<<"]";
    fs<<"}";
}

void colorDetector::read(const FileNode &node)
{
    NumeroDeColores = (int)node["numeroDeColores"];
    sesgador3colores = new sesgador [NumeroDeColores];
   /* for (int i = 0; i < NumeroDeColores; i++)
        sesgador3colores[i]= sesgador(i+1);*/


    FileNode features = node["Colores"];

    FileNodeIterator it = features.begin();
    FileNodeIterator it_end = features.end();

    // iterate through a sequence using FileNodeIterator
    for( int i = 0; it != it_end; it++, i++ )
    {
        double h_h = (double)(*it)["h_h"];
        double l_h = (double)(*it)["l_h"];
        double h_s = (double)(*it)["h_s"];
        double l_s = (double)(*it)["l_s"];

        sesgador3colores[i].set_values4sesgo(h_h,l_h,h_s,l_s);
    }

}

void colorDetector::calibrar(int Nsesgo)
{
    sesgador3colores[Nsesgo].calibrar();
}
///////////////////////////////////////////////////
void sesgador::set_values4sesgo(double h_h, double l_h, double h_s, double l_s)
{
    this-> h_h = h_h;
    this-> l_h = l_h;
    this-> h_s = h_s;
    this-> l_s = l_s;
}

void sesgador::recortar()
{
    vector<vector<Point> > contornos;
    vector<Vec4i> hierarchy;

    findContours(frame_thresholded.clone(),contornos, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    Rect roi = Tools::OpenCV::contenedorMasGrande( contornos );
    frame_sesgado = frame->clone()(roi);
}

sesgador::sesgador()
{
    h_h = l_h = h_s = l_s = 0;
    selected = false;
    frame_sesgado = Mat::zeros( 20, 20, CV_8UC3 );
    frame_thresholded = Mat::zeros( 20, 20, CV_8UC3 );

    frame = STAND::capturadorImagen::getImagen_pointer_procesada();
}

void sesgador::set_parameters4sesgo(int ID,
                                    const int *low_diff, const int *high_diff,
                                    const int *conn,
                                    const int *val,
                                    const int *flags,
                                    const Mat *kernel_rectangular, const Mat *kernel_ovalado)
{
    this->ID = ID;

    this->low_diff = low_diff;
    this->high_diff = high_diff;
    this->conn = conn;
    this->val = val;
    this->flags = flags;
    this->kernel_rectangular = kernel_rectangular;
    this->kernel_ovalado = kernel_ovalado;
}

void sesgador::calibrar(/*Mat m*/)
{
    //frame = m;

    if(!this->isRunning())
        start();
}

void sesgador::set_seedPoint(Point p)
{
    selected = true;

    // make mask using floodFill
    mask = Scalar::all(0);

    floodFill(*frame, mask, p,
              Scalar(255, 255, 255), 0,
              Scalar(*low_diff, *low_diff, *low_diff),
              Scalar(*high_diff, *high_diff, *high_diff), *flags);

    // find the H and S range of piexels selected by floodFill
    Mat channels[3];
    split(frame_hsv, channels);

    Mat InputMask = mask.rowRange(1, mask.rows-1).colRange(1, mask.cols-1);

    minMaxLoc(channels[0], &this->l_h, &this->h_h, NULL, NULL, InputMask );
    minMaxLoc(channels[1], &this->l_s, &this->h_s, NULL, NULL, InputMask );

}

void sesgador::corregirTrayectoria(const int direccionRobot_Nominal, const Point RobotPoint_Nominal)
{


    emit correccionDeTrayectoriaProcesada(ID,0.0,0.0);
}

void sesgador::run()
{
    //codigo sacado de la página 101 del libre Practical Opencv
   if(!selected)
   {
       sync.lock();
       mask.create(frame->rows+2, frame->cols+2, CV_8UC1);
       sync.unlock();
   }

   cvtColor(frame->clone(), frame_hsv, CV_BGR2HSV);

   // extract the hue and saturation channels
   int from_to[] = {0,0, 1,1};
   Mat hs(frame->size(), CV_8UC2);
   mixChannels(&frame_hsv, 1, &hs, 1, from_to, 2);

   // check for the range of H and S obtained from floodFill
   Mat aux; //esto se hace para que al usuario se muestre la umbralización con todas las operaciones morfoligicas aplicadas

   inRange(hs,
           Scalar( l_h, l_s ),
           Scalar( h_h, h_s ),
           aux);

   // open and close to remove noise
   morphologyEx(aux, aux, MORPH_OPEN, *kernel_rectangular);
   morphologyEx(aux, aux, MORPH_CLOSE, *kernel_rectangular);

   morphologyEx(aux, aux, MORPH_CLOSE, *kernel_ovalado);

   morphologyEx(aux, aux, MORPH_CLOSE, *kernel_rectangular);
   morphologyEx(aux, aux, MORPH_OPEN, *kernel_rectangular);


   frame_thresholded = aux;

   recortar();
}
