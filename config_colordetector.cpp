#include "config_colordetector.h"

using namespace CONFIG::coTra;

using Tools::math::Cuadrante_I;
using Tools::math::Cuadrante_II;
using Tools::math::Cuadrante_III;
using Tools::math::Cuadrante_IV;

int colorDetector_MANAGER::NumeroDeColores = 3;
colorDetector_WORKER **colorDetector_MANAGER::colorDetectorWORKERS;

void colorDetector_MANAGER::inicializar_sesgadores()
{
    colorDetectorWORKERS = new colorDetector_WORKER*[NumeroDeColores];
    for (int i = 0; i < NumeroDeColores; i++)
    {
        colorDetectorWORKERS[i] = new colorDetector_WORKER(i+1,&low_diff,&high_diff,&conn,&val,&flags,&kernel_rectangular,&kernel_ovalado);

        connect(colorDetectorWORKERS[i], SIGNAL(DESPACHAR_SolicitudDeTratectoria(int,float,double,float)),
                this, SLOT(RECIBIR_Despacho_CorreccionTrayectoria_FromWORKER(int,float,double,float)));
    }
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
        system("rm /tmp/PruebasAngulo/*/*.png");
        QString ruta = "mkdir /tmp/PruebasAngulo/";
        system(ruta.toUtf8().data());
        for (int i = 1; i <= colorDetector_MANAGER::NumeroDeColores; i++)
            system( QString( ruta+QString::number(i) ).toUtf8().data() );
    }

    inicializar_sesgadores();
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
    NumeroDeColores = (int)node["numeroDeColores"] ;
    inicializar_sesgadores();

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

vector<Tools::math::lineaRecta> colorDetector_MANAGER::getRectasToDraw()
{
    vector<Tools::math::lineaRecta> r;

    for (int i = 0; i < NumeroDeColores; i++)
        if( !(colorDetectorWORKERS[i]->rectaToDraw == Tools::math::lineaRecta()) )
            r.push_back( colorDetectorWORKERS[i]->rectaToDraw );

    return r;
}

void colorDetector_MANAGER::eliminarRecta(int RobotID)
{
    colorDetectorWORKERS[RobotID-1]->rectaToDraw = Tools::math::lineaRecta();
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
    Tools::OpenCV::DetectarCirculos( frame_sesgado, ListacirculosDetectados);

    if( ListacirculosDetectados.size() != 2 )// esto debe cambiarse para que detecte 1 solo circulo.
    {
        if( deteccionErrada++ > 10)
        {
            imshow("debug_sesgo",frame_sesgado);
            imshow("debug_blancoYNegro",frame_thresholded);
        }

        return false;
    }

    deteccionErrada = 0;

    int radio1 = ListacirculosDetectados[0][2];
    Point centro1(ListacirculosDetectados[0][0] + rectanguloSesgador.x,
                  ListacirculosDetectados[0][1] + rectanguloSesgador.y);

    int radio2 = ListacirculosDetectados[1][2];
    Point centro2(ListacirculosDetectados[1][0] + rectanguloSesgador.x,
                  ListacirculosDetectados[1][1] + rectanguloSesgador.y);

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

    ListacirculosDetectados = vector<Vec3f>();
    return true;
}

float colorDetector_WORKER::calcular_anguloDesface(Mat &mat,
                                                   Tools::math::lineaRecta rectaRobot,
                                                   Tools::math::lineaRecta rectaOrientacion,
                                                   float tetaRobot,
                                                   int DireccionNominal)
{
    float angulo;
    bool diagonal = false;
    switch (DireccionNominal)
    {
        case Tools::general::NORTE:
        case Tools::general::SUR:
            angulo = M_PI/2;
        break;

        case Tools::general::NOR_ESTE:
        case Tools::general::SUR_OESTE:
            angulo = (3*M_PI)/4;
            diagonal = true;
        break;

        case Tools::general::OESTE:
        case Tools::general::ESTE:
            angulo = 0.0;
        break;

        case Tools::general::NOR_OESTE:
        case Tools::general::SUR_ESTE:
            angulo = M_PI/4;
            diagonal = true;
        break;
    }

    //el punto A siempre será a donde debe llegar de la rectaDistancia
    //el punto A siempre será la base y el puntoB será el direccional

    //tita es el ángulo que forma la rectaOrientacion con el eje X
    float tita = Tools::math::lineaRecta::anguloEntre2Rectas(Tools::math::lineaRecta::ejeX(rectaRobot.puntoMedio),
                                                             rectaOrientacion,
                                                             true,true,
                                                             &mat,Scalar(12,205,235));
    //mover el punto de direccional al origen
    Point2f direccionalToOrigin = Point2f(rectaRobot.B.x - rectaRobot.puntoMedio.x,
                                          -1.0*rectaRobot.B.y +rectaRobot.puntoMedio.y );
    int cuadranteC = Tools::math::cuadranteDeUnPunto( direccionalToOrigin );

    int signoR = calcular_signoR(cuadranteC, tetaRobot, tita);

    Point2f Pdirec_TdestR( signoR*(rectaRobot.distanciaDelaRecta/2)*cos(tita),
                           signoR*(rectaRobot.distanciaDelaRecta/2)*sin(tita) );

    //dibujar Puntos
    if(Tools::general::DEBUG)
    {
        Point2f Pdirec_TdestR_1( Pdirec_TdestR.x + rectaOrientacion.A.x,
                                 -1.0*Pdirec_TdestR.y + rectaOrientacion.A.y );
        Point2f Pdirec_TdestR_2( Pdirec_TdestR.x + rectaRobot.puntoMedio.x,
                                -1.0*(Pdirec_TdestR.y - rectaRobot.puntoMedio.y ));

        Tools::OpenCV::dibujarPunto(mat, Pdirec_TdestR_1 );
        Tools::OpenCV::dibujarPunto(mat, Pdirec_TdestR_2 );

            if( diagonal )
            {
                Point2f PdirecROTADO = Tools::math::translateAPoint(Pdirec_TdestR);

                Point2f PdirecROTADO_Transladado1 = Point2f( PdirecROTADO.x + rectaOrientacion.A.x,
                                                             -1.0*PdirecROTADO.y + rectaOrientacion.A.y  );
                Point2f PdirecROTADO_Transladado2 = Point2f( PdirecROTADO.x + rectaRobot.puntoMedio.x,
                                                             -1.0*(PdirecROTADO.y - rectaRobot.puntoMedio.y ));

                Tools::OpenCV::dibujarPunto(mat,
                                            PdirecROTADO_Transladado1,
                                            Scalar(0,255,255) );
                Tools::OpenCV::dibujarPunto(mat,
                                            PdirecROTADO_Transladado2,
                                            Scalar(0,255,255) );
            }
    }
    //dibujar Puntos

    int cuadranteC_FINAL = Tools::math::cuadranteDeUnPunto( diagonal ?
                                                              Tools::math::translateAPoint(Pdirec_TdestR)
                                                              :
                                                              Pdirec_TdestR);
    if( diagonal )
        system(QString("echo \""+QString::number(cuadranteC)+"\" >> /tmp/PruebasAngulo/"+QString::number(ID)+"/t.txt ").toUtf8().data());

    int distancia = 50;
    //el punto A siempre será a donde debe llegar
    Tools::math::lineaRecta nuevaRectaRobot(Point( rectaOrientacion.A.x - (distancia)*cos(angulo),
                                                   rectaOrientacion.A.y - (distancia)*sin(angulo)),
                                            Point( rectaOrientacion.A.x + (distancia)*cos(angulo),
                                                   rectaOrientacion.A.y + (distancia)*sin(angulo) ) );

    Tools::OpenCV::dibujarRecta(*frame, nuevaRectaRobot);

    float tetaDesface = Tools::math::lineaRecta::anguloEntre2Rectas(rectaOrientacion, nuevaRectaRobot,
                                                                    false, true,
                                                                    frame,Scalar(255,0,0),true);

    bool correccionDeAngulo = false;
    switch (DireccionNominal)
    {
        case Tools::general::NORTE:
            if(cuadranteC_FINAL == Tools::math::Cuadrante_III || cuadranteC_FINAL == Tools::math::Cuadrante_IV )
                correccionDeAngulo = true;
        break;

        case Tools::general::SUR:
            if(cuadranteC_FINAL == Tools::math::Cuadrante_I || cuadranteC_FINAL == Tools::math::Cuadrante_II )
                correccionDeAngulo = true;
        break;
        //-------------------------------------------
        case Tools::general::NOR_ESTE:
            if(cuadranteC_FINAL == Tools::math::Cuadrante_II || cuadranteC_FINAL == Tools::math::Cuadrante_III )
                correccionDeAngulo = true;
        break;

        case Tools::general::SUR_OESTE:
            if(cuadranteC_FINAL == Tools::math::Cuadrante_I || cuadranteC_FINAL == Tools::math::Cuadrante_IV )
            correccionDeAngulo = true;
        break;
        //-------------------------------------------
        case Tools::general::OESTE:
            if(cuadranteC_FINAL == Tools::math::Cuadrante_I || cuadranteC_FINAL == Tools::math::Cuadrante_IV )
                correccionDeAngulo = true;
        break;

        case Tools::general::ESTE:
            if(cuadranteC_FINAL == Tools::math::Cuadrante_II || cuadranteC_FINAL == Tools::math::Cuadrante_III )
                correccionDeAngulo = true;
        break;
        //-------------------------------------------
        case Tools::general::NOR_OESTE:
            if(cuadranteC_FINAL == Tools::math::Cuadrante_III || cuadranteC_FINAL == Tools::math::Cuadrante_IV )
                correccionDeAngulo = true;
        break;

        case Tools::general::SUR_ESTE:
            if(cuadranteC_FINAL == Tools::math::Cuadrante_II || cuadranteC_FINAL == Tools::math::Cuadrante_I )
                correccionDeAngulo = true;
        break;
    }


    if(correccionDeAngulo)
    {
        int vuelta = tetaDesface < 0 ? 180:-180;
        tetaDesface = (tetaDesface + vuelta );
    }

    Tools::math::lineaRecta A=rectaOrientacion, B=nuevaRectaRobot;
    Tools::math::lineaRecta::OrganizarRectas(A,B);
    tetaDesface = tetaDesface * (A==rectaOrientacion  ? 1.0:-1.0);

    if(Tools::general::DEBUG)
        qDebug()<<"AnguloDesface:"<<tetaDesface;

    return tetaDesface;
}

void colorDetector_WORKER::guardarImagenes(Mat const imToDraw,float const teta, float const tita, double Distancia_desface)
{
    QString ruta = "/tmp/PruebasAngulo/";
    ruta = ruta + QString::number(ID)+ QString("/")+
            QString::number(++Nfoto) + QString(" -> ") +
            QString("x") + QString::number(RobotPoint_Nominal.x) +
            QString("_y") + QString::number(RobotPoint_Nominal.y) +
            QString(" teta:") + QString::number( teta ) + QString(", ") +
            QString(" tetaDesface:") + QString::number( tita ) + QString(", ") +
            QString("Dist:") + QString::number(Distancia_desface) +
            QString(".png") ;

    imwrite( ruta.toUtf8().data(), imToDraw);
}

void colorDetector_WORKER::sesgar()
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
}

double colorDetector_WORKER::procesar_DistanciaARecorrer(double distancia,
                                                        const Tools::math::lineaRecta rectaRobot,
                                                        const Tools::math::lineaRecta rectaDistancia)
{
    //el punto A siempre será a donde debe llegar de la rectaDistancia
    //el punto A siempre será la base y el puntoB será el direccional

    //el punto base está más cerca del punto destino.
    RobotEnRetroceso = Tools::math::PointAisCloserTo(rectaRobot.A, rectaRobot.B,rectaDistancia.A);

    return distancia*(RobotEnRetroceso ? -1:1);
}


float colorDetector_WORKER::calcular_teta(Mat &m, Tools::math::lineaRecta rectaRobot, Tools::math::lineaRecta rectaRobot_Destino)
{
    float teta = Tools::math::lineaRecta::anguloEntre2Rectas(rectaRobot, rectaRobot_Destino,
                                                            false, true,
                                                            &m);
    Tools::math::lineaRecta A = rectaRobot;
    Tools::math::lineaRecta B = rectaRobot_Destino;

    Tools::math::lineaRecta::OrganizarRectas( A, B);

    return teta * (A==rectaRobot ? 1:-1);
}


//coordenadas polares
float colorDetector_WORKER::calcular_signoR(int cuadranteC, float teta, float tita)
{
    switch (cuadranteC)
    {
        case Cuadrante_I:
            if(tita < 0 && teta > 0)
                return -1.0;
        break;

        case Cuadrante_II:
            if(  tita < 0 ||
                (tita > 0 && teta > 0))
                return -1.0;
        break;

        case Cuadrante_III:
            if(  tita > 0 ||
                 tita < 0 && teta < 0)
                return -1.0;
        break;

        case Cuadrante_IV:
            if( tita > 0 && teta < 0 )
                return -1.0;
        break;
    }

    return 1.0;
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

    rectaToDraw = Tools::math::lineaRecta();

    Nfoto = 0;
    isPeticion = false;
    deteccionErrada = 0;
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

    qDebug()<<"h_h:"<<h_h;
    qDebug()<<"l_h:"<<l_h;
    qDebug()<<"h_s:"<<h_s;
    qDebug()<<"l_s:"<<l_s;
    qDebug()<<"--------------";


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
    bool anotherRun = false;

    do
    {
        sesgar();

        if(isPeticion)
        {
            Tools::math::circulo base, direccional;

            if( detectarCirculos(base,direccional) ) //detectados exitosamente
            {

                //Siempre el punto A será la base y el punto B será la dirección.
                Tools::math::lineaRecta rectaRobot(base.centro, direccional.centro);

                int tamano_cuadros_imgReal = frame->cols/ INTMatBuilder::n;

                //el punto A siempre será a donde debe llegar
                Tools::math::lineaRecta recta_orientacion( Point(tamano_cuadros_imgReal*(RobotPoint_Nominal.x + 0.5),
                                                                  tamano_cuadros_imgReal*(RobotPoint_Nominal.y + 0.5)),
                                                            Point(rectaRobot.puntoMedio.x,
                                                                  rectaRobot.puntoMedio.y));

                rectaToDraw = recta_orientacion;

                Mat imToDraw = Tools::general::DEBUG ? *frame:frame->clone();

                float teta = calcular_teta(imToDraw,
                                           rectaRobot,recta_orientacion);

                double Distancia_desface =
                        procesar_DistanciaARecorrer( calibrador::distanciaReal_2PuntosPixeles(recta_orientacion.A, recta_orientacion.B),
                                                    rectaRobot,
                                                    recta_orientacion);

                float angulo_desface = calcular_anguloDesface(imToDraw,
                                                              rectaRobot,recta_orientacion,
                                                              teta,
                                                              direccionRobot_Nominal);

                /*if( Tools::general::DEBUG )
                {
                    imshow("",imToDraw);
                    guardarImagenes(imToDraw,
                                    teta,
                                    angulo_desface,
                                    Distancia_desface);
                }*/

                emit DESPACHAR_SolicitudDeTratectoria(ID,teta,Distancia_desface, angulo_desface);

                anotherRun = false;
            }
            else
                anotherRun = true;
        }

    }while(anotherRun);

    isPeticion=false;
}
