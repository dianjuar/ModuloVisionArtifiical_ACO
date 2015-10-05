#include "config_colordetector.h"

using namespace CONFIG;
using namespace coTra;

int colorDetector_MANAGER::NumeroDeColores;

extern const int NORTE;
extern const int NORESTE;
extern const int ESTE;
extern const int SURESTE;
extern const int SUR;
extern const int SUROESTE;
extern const int OESTE;
extern const int NOROESTE;

void colorDetector_MANAGER::inicializar_sesgadores(int NumeroDeColores)
{
    this->NumeroDeColores = NumeroDeColores;
    colorDetectorWORKERS = new colorDetector_WORKER*[NumeroDeColores];

    for (int i = 0; i < NumeroDeColores; i++)
    {
        colorDetectorWORKERS[i] = new colorDetector_WORKER(i+1,&low_diff,&high_diff,&conn,&val,&flags,&kernel_rectangular,&kernel_ovalado);

        connect(colorDetectorWORKERS[i], SIGNAL(DESPACHAR_SolicitudDeTratectoria(int,float,double,float)),
                this, SLOT(RECIBIR_Despacho_CorreccionTrayectoria_FromWORKER(int,float,double,float)));
    }

}

float colorDetector_MANAGER::calcular_anguloDesface(Tools::math::lineaRecta rectaRobot, int direccion_Robot_Nominal)
{

}

colorDetector_MANAGER::colorDetector_MANAGER()
{
    //estos 5 valores fueron sacados despues de varias pruebas en el laboratorio de prototipos y fueron los que mejores resultado arrojaron
    low_diff = 20;
    high_diff = 10;
    conn = 4;
    val = 255;
    flags = conn + (val << 8) + CV_FLOODFILL_MASK_ONLY;
    kernel_rectangular = getStructuringElement(MORPH_RECT, Size(5, 5));
    kernel_ovalado = getStructuringElement(MORPH_ELLIPSE, Size(30, 30));

    if( Tools::general::DEBUG )
    {
        system("rm /tmp/PruebasAngulo/*/*.jpg");
        QString ruta = "mkdir /tmp/PruebasAngulo/";
        system(ruta.toUtf8().data());
        for (int i = 1; i <= colorDetector_MANAGER::NumeroDeColores; i++)
            system( QString( ruta+QString::number(i) ).toUtf8().data() );
    }

    inicializar_sesgadores(3);
}

void colorDetector_MANAGER::write(FileStorage &fs) const
{
    fs << "{";

        fs<<"numeroDeColores" << NumeroDeColores;

        fs << "Colores"<<"[";
        for (int i = 0; i < NumeroDeColores; i++)
        {
            double h_h = colorDetectorWORKERS[i]->get_h_h();
            double l_h = colorDetectorWORKERS[i]->get_l_h();
            double h_s = colorDetectorWORKERS[i]->get_h_s();
            double l_s = colorDetectorWORKERS[i]->get_l_s();

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

void colorDetector_MANAGER::read(const FileNode &node)
{
    inicializar_sesgadores( (int)node["numeroDeColores"] );

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

        colorDetectorWORKERS[i]->set_values4sesgo(h_h,l_h,h_s,l_s);
    }

}

void colorDetector_MANAGER::calibrar(int Nsesgo)
{
    colorDetectorWORKERS[Nsesgo]->calibrar();
}

void colorDetector_MANAGER::RECIBIRsolicitud_CorreccionTrayectoria(int RobotID, int direccionRobot_Nominal,
                                                                   int RobotPoint_Nominal_X, int RobotPoint_Nominal_Y)
{
    colorDetectorWORKERS[RobotID-1]->corregirTrayectoria(direccionRobot_Nominal, Point(RobotPoint_Nominal_X, RobotPoint_Nominal_Y));
}

void colorDetector_MANAGER::RECIBIR_Despacho_CorreccionTrayectoria_FromWORKER(int robotID, float teta, double distanciaDesface, float anguloDesface)
{
    emit DESPACHARsolicitud_CorreccionTrayectoria(robotID, teta, distanciaDesface, anguloDesface);
}

///////////////////////////////////////////////////
void colorDetector_WORKER::set_values4sesgo(double h_h, double l_h, double h_s, double l_s)
{
    this-> h_h = h_h;
    this-> l_h = l_h;
    this-> h_s = h_s;
    this-> l_s = l_s;
}

void colorDetector_WORKER::recortar()
{
    vector<vector<Point> > contornos;
    vector<Vec4i> hierarchy;

    findContours(frame_thresholded.clone(),contornos, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    rectanguloSesgador = Tools::OpenCV::contenedorMasGrande( contornos );

    //frame_sesgado = frame->clone()(roi);
    frame_sesgado = STAND::capturadorImagen::Imagen_Procesada(rectanguloSesgador);
}

bool colorDetector_WORKER::detectarCirculos(Tools::math::circulo &base, Tools::math::circulo &direccional)
{
    vector<Vec3f> circulos = Tools::OpenCV::DetectarCirculos( frame_sesgado );

    if( circulos.size() != 2 )// esto debe cambiarse para que detecte 1 solo circulo.
        return false;

    int radio1 = circulos[0][2];
    Point centro1(circulos[0][0] + rectanguloSesgador.x,
                  circulos[0][1] + rectanguloSesgador.y);

    int radio2 = circulos[1][2];
    Point centro2(circulos[1][0] + rectanguloSesgador.x,
                  circulos[1][1] + rectanguloSesgador.y);

    if(radio1 > radio2)
    {
        base = Tools::math::circulo(centro1,radio1);
        direccional = Tools::math::circulo(centro2, radio2);
    }
    else
    {
        direccional = Tools::math::circulo(centro1, radio2);
        base = Tools::math::circulo(centro2, radio2);
    }

    return true;
}

float colorDetector_WORKER::calcular_anguloDesface(Tools::math::lineaRecta rectaDestino, float DistanciaRectaRobot, float teta, int DireccionNominal)
{
    Tools::math::lineaRecta nuevaRectaRobot();

    /*switch (DireccionNominal)
    {
    case NORTE:
    case SUR:

        break;

    case NOROESTE:
    case SURESTE:

        break;

    case OESTE:
    case ESTE:

        break;

    case SUROESTE:
    case NORESTE:

        break;
    }*/
}

colorDetector_WORKER::colorDetector_WORKER(int ID, const int *low_diff, const int *high_diff, const int *conn, const int *val, const int *flags, const Mat *kernel_rectangular, const Mat *kernel_ovalado)
{
    this->ID = ID;
    this->low_diff = low_diff;
    this->high_diff = high_diff;
    this->conn = conn;
    this->val = val;
    this->flags = flags;
    this->kernel_rectangular = kernel_rectangular;
    this->kernel_ovalado = kernel_ovalado;

    h_h = l_h = h_s = l_s = 0;
    selected = false;
    frame_sesgado = Mat::zeros( 20, 20, CV_8UC3 );
    frame_thresholded = Mat::zeros( 20, 20, CV_8UC3 );

    frame = &STAND::capturadorImagen::Imagen_Procesada;

}

void colorDetector_WORKER::calibrar()
{
    if(!this->isRunning())
        start();
}

void colorDetector_WORKER::set_seedPoint(Point p)
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

void colorDetector_WORKER::corregirTrayectoria(const int direccionRobot_Nominal, const Point RobotPoint_Nominal)
{
    isPeticion=true;
    this->direccionRobot_Nominal = direccionRobot_Nominal;
    this->RobotPoint_Nominal = RobotPoint_Nominal;

    start();
}

void colorDetector_WORKER::run()
{
   //codigo sacado de la página 101 del libre Practical Opencv
   if(!selected)
   {
       sync.lock();
       mask.create(frame->rows+2, frame->cols+2, CV_8UC1);
       sync.unlock();
   }

   cvtColor( *frame, frame_hsv, CV_BGR2HSV);

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

    if(isPeticion)
    {
       Tools::math::circulo base, direccional;

       if( detectarCirculos(base,direccional) ) //detectados exitosamente
       {
            Tools::math::lineaRecta rectaRobot(base.centro, direccional.centro);

            int tamano_cuadros_imgReal = frame->cols/ INTMatBuilder::n;

            Tools::math::lineaRecta rectaRobot_Destino( Point(rectaRobot.puntoMedio.x,
                                                              rectaRobot.puntoMedio.y),
                                                        Point(tamano_cuadros_imgReal*(RobotPoint_Nominal.x + 0.5),
                                                              tamano_cuadros_imgReal*(RobotPoint_Nominal.y + 0.5)) );

            Mat imToDraw = frame->clone();

            float teta, anguloInicial;
            Tools::OpenCV::anguloEntreRectas(imToDraw,rectaRobot,rectaRobot_Destino,teta,anguloInicial);

            if( Tools::general::DEBUG )
            {
                QString ruta = "/tmp/PruebasAngulo/";
                ruta = ruta + QString::number(ID)+ QString("/x")+
                              QString::number(RobotPoint_Nominal.x) + QString("_y")+
                              QString::number(RobotPoint_Nominal.y) + QString(" ")+
                              QString::number( teta ) + QString(",") + QString::number(anguloInicial) +
                              QString(".jpg") ;
                imwrite( ruta.toUtf8().data(), imToDraw);
            }


            double Distancia_desface = calibrador::distanciaReal_2PuntosPixeles(rectaRobot_Destino.A, rectaRobot_Destino.B);

            float angulo_desface = calcular_anguloDesface(rectaRobot, direccionRobot_Nominal);

            emit DESPACHAR_SolicitudDeTratectoria(ID,teta,Distancia_desface,0.0);
       }

    isPeticion=false;
    }
}
