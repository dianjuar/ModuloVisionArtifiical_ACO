#include <limits>
#include "stand_Tools.h"

using namespace Tools;

bool general::DEBUG = true;
int OpenCV::numeroDeVecesDetectadoUnCirculo = 0;
Mat OpenCV::tratamientoDeImagenStantdar(Mat source, bool withGaussianBlur)
{
    Mat aux;
    cvtColor(source,aux,CV_RGBA2GRAY);

    if(withGaussianBlur)
        GaussianBlur(aux,aux,Size(9,9),2,2);//se ha jugado con anterioridad estos parametros y dan buenos resultados para el propósito que se desea

    return aux;
}

QPixmap OpenCV::Mat2QPixmap(Mat m, bool resize, int x)
{
    QImage img;

    if(resize==true)
        cv::resize(m,m,Size(x,x),0,0,INTER_LINEAR);


    switch (m.type())
    {
        case CV_8UC1:
        {
            // Set the color table (used to translate colour indexes to qRgb values)
            QVector<QRgb> colorTable;
            for (int i=0; i<256; i++)
                colorTable.push_back(qRgb(i,i,i));
            // Copy input Mat
            const uchar *qImageBuffer = (const uchar*)m.data;
            // Create QImage with same dimensions as input Mat
            img = QImage(qImageBuffer, m.cols, m.rows, m.step, QImage::Format_Indexed8);
            img.setColorTable(colorTable);
        }
        break;

        case CV_8UC3:
        {
            // Copy input Mat
            const uchar *qImageBuffer = (const uchar*)m.data;
            // Create QImage with same dimensions as input Mat
            img = QImage(qImageBuffer, m.cols, m.rows, m.step, QImage::Format_RGB888);
            img = img.rgbSwapped();
        }
        break;

        default:
        {
            qDebug() << "ERROR: Mat could not be converted to QImage.";
            img = QImage();
        }
        break;
    }

    return QPixmap::fromImage(img);
}

QPixmap OpenCV::Mat2QPixmap(Mat m, int scale)
{
    if(m.cols <= 0 && m.rows <= 0)
        return Mat2QPixmap( Mat(20, 20, CV_8UC3, Scalar(0,0,0)) );

    cv::resize(m,m,Size(m.cols/scale,m.rows/scale),0,0,INTER_LINEAR);
    return Mat2QPixmap(m);
}

Rect OpenCV::contenedorMasGrande(vector<vector<Point> > contours)
{
    if(contours.size()>0)
    {
        int larguestArea = -1;
        int largest_contour_index = 0;

        if (contours.size() > 1)
            for( int i = 0; i< contours.size(); i++ )
            {
                 double area = boundingRect(contours[ i ] ).area();

                 if(area > larguestArea )
                 {
                     larguestArea = area;
                     largest_contour_index=i;
                 }
            }

        return boundingRect(contours[ largest_contour_index ] );
    }
    else
        return Rect();
}

void OpenCV::dibujarPunto(Mat &m,Point p, Scalar color)
{
    circle(m, p, 4, color, -1);
}

void OpenCV::DetectarCirculos(Mat mat, vector<Vec3f> &ListacirculosDetectados, int n, bool dibujar)
{
    int tasaDeError = 7; //pixeles

        if(Tools::general::DEBUG)
            qDebug()<<"*******************iteración para detectar circulos*******************";

        Mat matClone = mat.clone(); //se evita que a mitad de camino cambie la matriz
        Mat matGrayScale = OpenCV::tratamientoDeImagenStantdar(matClone);

        vector<Vec3f> circulosDetectados;

        //acomodar con los parametros correctos
        int param1 = 116;//79
        int param2 = 20;//23

        HoughCircles( matGrayScale,
                      circulosDetectados,
                      CV_HOUGH_GRADIENT,
                      1, 35, param1, param2, 5);

        if( circulosDetectados.size() <= n )
        {
            if(ListacirculosDetectados.size() == 0 )
                ListacirculosDetectados = circulosDetectados;
            else
            {
                for (int i = 0; i < circulosDetectados.size(); i++)
                {
                    bool circuloUnico = false;
                    Vec3f Cdetectado = circulosDetectados.at(i);

                    Point centroDect(Cdetectado[0], Cdetectado[1]);
                    float radioDect = Cdetectado[2];

                    for (int j = 0; j < ListacirculosDetectados.size(); j++)
                    {
                        Vec3f Cguardado = ListacirculosDetectados.at(j);

                        Point centroGuar(Cguardado[0], Cguardado[1]);
                        float radioGuar = Cguardado[2];

                        /*verificar que sea el mismo RADIO y el mismo PUNTO, como los circulos
                         * los detecta con un ligero error, se verifica que esté fuera de ese error*/

                            //centro del circulo
                        if( (abs(centroDect.x - centroGuar.x) + abs(centroDect.y - centroGuar.y) > tasaDeError) &&
                            //radio del circulo
                             abs(radioDect - radioGuar) > tasaDeError )
                        {
                            if(!Tools::math::circulo::isCircleInsideOther( math::circulo(centroDect, radioDect),
                                                                            math::circulo(centroGuar, radioGuar)))
                            {
                                circuloUnico = true;
                                break;
                            }
                        }
                    }

                    if(circuloUnico)
                        ListacirculosDetectados.push_back(   Cdetectado );
                }
            }
        }


    qDebug()<<"Ciruclos Detectados"<<ListacirculosDetectados.size();

    if( ListacirculosDetectados.size() > n || numeroDeVecesDetectadoUnCirculo++ > 10)
    {
        ListacirculosDetectados = vector<Vec3f>();
        numeroDeVecesDetectadoUnCirculo = 0;
    }

    if(dibujar)
        dibujarCirculos(mat,ListacirculosDetectados);
}

void OpenCV::dibujarRecta( Mat &mat, math::lineaRecta linea, bool colorRojo, bool dibujarCentro)
{
    /*donde m1 es la pendiente de la recta que sirve de lado inicial del ángulo y m2 es la pendiente
    de la recta que sirve de lado terminal del ángulo.*/
    Scalar Color = colorRojo ? Scalar(0,0,255): Scalar(0,255,0);

    line(mat, linea.B, linea.A, Color, 2  );

    if(dibujarCentro)
        circle( mat, linea.puntoMedio, 5, Scalar(0,0,0), -1, 8, 0 ); //centro de la línea
}

void OpenCV::dibujarCirculo(Mat &mat, Point center, int radio, int BaseAngle, int startAngle, int endAngle, Scalar color,int grosor)
{
    ellipse( mat, center, Size( radio, radio ),
             BaseAngle, startAngle, endAngle,
             color, grosor );
}

void OpenCV::dibujarAnguloEntreRectas(Mat &mat, math::lineaRecta R1, math::lineaRecta R2, float teta, Scalar color)
{
    //siempre R1 será la recta del robot
    math::lineaRecta rectaRobot = R1;
    math::lineaRecta::OrganizarRectas(R1,R2);

    math::lineaRecta ra = R1;
    math::lineaRecta rb = math::lineaRecta::ejeX();
    math::lineaRecta::OrganizarRectas(ra,rb);

    float anguloInicial = math::lineaRecta::anguloEntre2Rectas(ra, rb,false)*
                    /*por alguna razón el angulo inicial hay que cambiarle de signo para que se pueda gráficar bien
                    cuando las 2 rectas son negativas*/
                   ((!R1.isM_positivo() && !R2.isM_positivo()) ? -1:1);

    if(Tools::general::DEBUG)
    {
        qDebug()<<"Angulo entre la recta robot y el eje X"<<anguloInicial;
        qDebug()<<"Angulo entre las 2 rectas"<<teta;
    }

        int angle = 180-anguloInicial,
        startAngle = 0,
        endAngle = -teta;

        //así sabrá si dibujar el centro de la recta o no.
        bool a = rectaRobot == R1;

        dibujarRecta(mat,R1,false, a );
        dibujarRecta(mat,R2,true, !a);
        dibujarCirculo(mat,rectaRobot.puntoMedio,rectaRobot.distanciaDelaRecta/2,angle,startAngle,endAngle,color);
        dibujarCirculo(mat,rectaRobot.puntoMedio,rectaRobot.distanciaDelaRecta/2,angle+180,startAngle,endAngle, color);
}

void OpenCV::dibujarCirculos(Mat mat, vector<Vec3f> circles)
{
    for( size_t i = 0; i < circles.size(); i++ )
    {
       Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
       int radius = cvRound(circles[i][2]);
       // circle center
       circle( mat, center, 3, Scalar(0,255,0), -1, 8, 0 );
       // circle outline
       circle( mat, center, radius, Scalar(0,0,255), 3, 8, 0 );
     }
}

//////////////////////////////////////////////////////

QString general::IntMat2QString(int **mat, int n)
{

    QString matQSt;

    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            matQSt.append(QString::number( mat[i][j] ));
        }

        matQSt.append( i!=n-1?"\n":"");
    }

    return matQSt;
}

std::vector<string> general::split(string s, const string delim)
{
    std::vector< std::string >vector;
    std::string token;
    size_t pos = 0;

    while ((pos = s.find(delim)) != std::string::npos)
    {
        token = s.substr(0, pos);
        vector.push_back(token);
        s.erase(0, pos + delim.length());
    }
    vector.push_back(s);

   return vector;
}

std::vector<QString> general::split(QString s, const QString delim)
{
    vector<QString> vecQS;
    vector<string> vec = split( s.toStdString(), delim.toStdString() );

    foreach( string str, vec)
        vecQS.push_back( QString( str.data() ) );

    return vecQS;
}

//////////////////////////////////////////////////////
math::circulo::circulo(Point centro, int radio)
{
    this->centro = centro;
    this->radio = radio;
}

bool math::circulo::isCircleInsideOther(math::circulo c1, math::circulo c2)
{
    double d = math::distanciaEntre2Puntos(c1.centro, c2.centro);

    return d - c1.radio <= c2.radio;
}

math::circulo::circulo()
{
    radio = -1;
    centro = Point(-1,-1);
}

////////////////////////////////////////////////////////
void math::lineaRecta::calcularDistancia()
{
    distanciaDelaRecta = distanciaEntre2Puntos(A,B);
}

void math::lineaRecta::OrganizarRectas(math::lineaRecta &R1, math::lineaRecta &R2)
{
    float m1 = R1.m;
    float m2 = R2.m;

    if( m1 >= 0 && m2 >=0 ) //primer caso que ambas sean positivas, m2 será el mayor
    {
        if( m1 > m2 )
        {
            math::lineaRecta aux = R2;
            R2 = R1;
            R1 = aux;
        }
        return;
    }
    else
    {
        if( ( m1 < 0 && m2 > 0 ) || ( m1 > 0 && m2 < 0 ) ) //segundo caso que una sea positiva y otra negativa, m2 será el negativo
        {
            if(m1 < 0)
            {
                math::lineaRecta aux = R2;
                R2 = R1;
                R1 = aux;
            }
            return;
        }
        else
        {
            if( m1 < 0 && m2 < 0 ) //tercer caso que ambas sean negativas, m2 será el que tenga el menos valor absoluto
            {
                if( abs(m1) < abs(m2) )
                {
                    math::lineaRecta aux = R2;
                    R2 = R1;
                    R1 = aux;
                }
            }
        }//else
    }//else
}

math::lineaRecta math::lineaRecta::ejeX()
{
    return lineaRecta(0,0);
}

math::lineaRecta math::lineaRecta::ejeX(Point puntoMedio)
{
    //Point(0,rectaRobot.puntoMedio.y),
    //Point(1000,rectaRobot.puntoMedio.y)
    int distancia = 40;
    return lineaRecta(Point2f(puntoMedio.x - distancia, puntoMedio.y),
                      Point2f(puntoMedio.x + distancia, puntoMedio.y));
}

bool math::lineaRecta::isRectaR1( math::lineaRecta Recta, math::lineaRecta R1, math::lineaRecta R2)
{
    OrganizarRectas(R1,R2);

    return Recta == R1;
}

math::lineaRecta::lineaRecta(float m, float b, Point2f A, Point2f B)
{
    this->m = m;
    this->b = b;
    this->A = A;
    this->B = B;

    puntoMedio = Point( (A.x+B.x)/2 , (A.y+B.y)/2 );
    calcularDistancia();
}

math::lineaRecta::lineaRecta(float m, float b)
{
    this->m = m;
    this->b = b;
}

math::lineaRecta::lineaRecta(Point2f A, Point2f B)
{
    this->A = A;
    this->B = B;

    m = (float)(this->B.y*-1 - this->A.y*-1)/(this->B.x - this->A.x);

        if( abs(m) == std::numeric_limits<float>::infinity())
        {
            m = std::numeric_limits<int>::max();
            b = -1* std::numeric_limits<int>::max();
        }

    b = (float)(this->A.y*-1 - this->A.x*m);

    puntoMedio = Point2f( (this->A.x+this->B.x)/2 , (this->A.y+this->B.y)/2 );
    calcularDistancia();
}

math::lineaRecta::lineaRecta()
{
    this->m = std::numeric_limits<float>::min();
    this->b = std::numeric_limits<float>::min();

    this->A = Point( std::numeric_limits<int>::min(),
                     std::numeric_limits<int>::min() );

    this->B  = Point( std::numeric_limits<int>::min(),
                      std::numeric_limits<int>::min() );
}

float math::lineaRecta::puntoEnY(float puntoX)
{
    return m*puntoX + b;
}

float math::lineaRecta::puntoEnX(float puntoY)
{
    return (puntoY-b) / m;
}

float math::lineaRecta::anguloEntre2Rectas(math::lineaRecta lA, math::lineaRecta lB,
                                           bool radianes,bool dibujar,
                                           Mat *m, Scalar color, bool dibujarInvertio)
{
    //siempre R1 será la recta del robot
    math::lineaRecta rectaRobot = lA;
    math::lineaRecta rectaDestino = lB;

    OrganizarRectas(lA,lB);

    if(Tools::general::DEBUG)
    {
        qDebug()<<"------------------------";
        qDebug()<<"m1"<<lA.m<<" - "<<"m2"<<lB.m;
        qDebug()<<"------------------------";
    }

    float m1 = lA.m;
    float m2 = lB.m;

    if( general::DEBUG &&
        (lA.m == std::numeric_limits<int>::max() || lB.m == std::numeric_limits<int>::max()) )
        qDebug()<<"infinito";

    long double tangTeta = ((long double)(m2-m1))/((long double)(1.0+m2*m1));

    float teta = ( atan( tangTeta )*(180.0/M_PI) );

    if(dibujar && m != NULL)
        if(dibujarInvertio)
            Tools::OpenCV::dibujarAnguloEntreRectas(*m, rectaDestino, rectaRobot, teta, color);
        else
            Tools::OpenCV::dibujarAnguloEntreRectas(*m, rectaRobot, rectaDestino,teta, color);

    return teta/( radianes ? 180.0/M_PI:1);
}

bool math::lineaRecta::isM_positivo()
{
    return m > 0;
}

bool math::lineaRecta::operator==(const math::lineaRecta &other)
{
    return this->m == other.m &&
           this->b == other.b &&
           this->A == other.A &&
           this->B == other.B;
}

void math::lineaRecta::operator=(const math::lineaRecta &other)
{
    this->A = other.A;
    this->B= other.B;
    this->puntoMedio = other.puntoMedio;

    this->b = other.b;
    this->m = other.m;
    this->distanciaDelaRecta = other.distanciaDelaRecta;
}
////////////////////////////////////////////////////////
double math::distanciaEntre2Puntos(Point2f A, Point2f B)
{
    return sqrt( qPow(A.x - B.x, 2) + qPow(A.y - B.y, 2) );
}


bool math::PointAisCloserTo(Point A, Point B, Point Destino)
{
    lineaRecta rectaA_to_Destino( A, Destino );
    lineaRecta rectaB_to_Destino( B, Destino );

    if( rectaA_to_Destino.distanciaDelaRecta < rectaB_to_Destino.distanciaDelaRecta )
        return true;

    return false;
}

int math::cuadranteDeUnPunto(Point2f p)
{
    if( p.x >= 0 && p.y >= 0)
        return math::Cuadrante_I;

    if( p.x <= 0 && p.y >= 0)
        return math::Cuadrante_II;

    if( p.x <= 0 && p.y <= 0)
        return math::Cuadrante_III;

    return Cuadrante_IV;
}

Point2f math::translateAPoint(Point2f p, float teta)
{
    float x = p.x,
          y = p.y;

    return Point2f( x*cos(teta) - y*sin(teta),
                    x*sin(teta) + y*cos(teta));
}
