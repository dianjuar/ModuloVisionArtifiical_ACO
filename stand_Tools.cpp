#include "stand_Tools.h"

using namespace Tools;

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

vector<Vec3f> OpenCV::DetectarCirculos(Mat mat, bool dibujar)
{
    Mat matGrayScale = OpenCV::tratamientoDeImagenStantdar(mat);

    vector<Vec3f> circulosDetectados;

    //acomodar con los parametros correctos
    HoughCircles( matGrayScale,
                  circulosDetectados,
                  CV_HOUGH_GRADIENT,
                  1, 15, 20, 20, 5,75);

    qDebug()<<"Circulos detectados = "<<circulosDetectados.size();

    if(dibujar)
        dibujarCirculos(mat,circulosDetectados);

    return circulosDetectados;
}

void OpenCV::dibujarRecta(Mat &mat, math::lineaRecta linea, bool colorRojo, bool dibujarCentro)
{
    /*donde m1 es la pendiente de la recta que sirve de lado inicial del ángulo y m2 es la pendiente
    de la recta que sirve de lado terminal del ángulo.*/
    Scalar Color = colorRojo ? Scalar(0,0,255): Scalar(0,255,0);

    line(mat, linea.A, linea.B, Color, 2  );

    if(dibujarCentro)
        circle( mat, linea.puntoMedio, 5, Scalar(0,0,0), -1, 8, 0 ); //centro de la línea
}

void OpenCV::dibujarCirculo(Mat &mat, Point center, int radio, int BaseAngle, int startAngle, int endAngle)
{
    ellipse( mat, center, Size( radio, radio ),
             BaseAngle, startAngle, endAngle,
             Scalar( 255, 0, 0 ), 2 );
}

void OpenCV::dibujarAnguloEntreRectas(Mat &mat, math::lineaRecta R1, math::lineaRecta R2, float &teta, float &anguloInicial)
{
    math::lineaRecta rectaRobot = R1;

    math::lineaRecta::OrganizarRectas(R1,R2);

    dibujarRecta(mat,R1,false);
    dibujarRecta(mat,R2);

    teta = math::lineaRecta::anguloEntre2Rectas(R1,R2);

    math::lineaRecta ejeX( Point(0,rectaRobot.puntoMedio.y),
                           Point(1000,rectaRobot.puntoMedio.y) );

    math::lineaRecta ra = R1;
    math::lineaRecta rb = ejeX;
    math::lineaRecta::OrganizarRectas(ra,rb);

    anguloInicial = math::lineaRecta::anguloEntre2Rectas(ra, rb) *
                   /* por alguna razón el angulo inicial hay que cambiarle de signo para que se pueda gráficar bien
                    cuando las 2 rectas son negativas*/
                   ((R1.m<0 && R2.m<0) ? -1:1);

    qDebug()<<"Angulo entre la recta robot y el eje X"<<anguloInicial;
    qDebug()<<"Angulo entre las 2 rectas"<<teta;

        int angle = anguloInicial,
        startAngle = 0,
        endAngle = teta;

        dibujarRecta(mat,R1,false);
        dibujarRecta(mat,R2);

        OpenCV::dibujarCirculo(mat,rectaRobot.puntoMedio,rectaRobot.distanciaDelaRecta/2,angle,startAngle,endAngle);
        OpenCV::dibujarCirculo(mat,rectaRobot.puntoMedio,rectaRobot.distanciaDelaRecta/2,angle+180,startAngle,endAngle);

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

QString Cfunctions::IntMat2QString(int **mat, int n)
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

std::vector<string> Cfunctions::split(string s, const string delim)
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

std::vector<QString> Cfunctions::split(QString s, const QString delim)
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

math::circulo::circulo()
{
    radio = -1;
    centro = Point(-1,-1);
}

////////////////////////////////////////////////////////
void math::lineaRecta::calcularDistancia()
{
    distanciaDelaRecta = sqrt( pow(B.x - A.x ,2) + pow( B.y - A.y ,2) );
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

math::lineaRecta::lineaRecta(float m, float b, Point A, Point B)
{
    this->m = m;
    this->b = b;
    this->A = A;
    this->B = B;

    puntoMedio = Point( (A.x+B.x)/2 , (A.y+B.y)/2 );
    calcularDistancia();
}

math::lineaRecta::lineaRecta(Point A, Point B)
{
    this->A = A;
    this->B = B;

    m = (float)(this->B.y*-1 - this->A.y*-1)/(this->B.x - this->A.x);
    b = (float)(this->A.y*-1 - this->A.x*m);

    puntoMedio = Point( (this->A.x+this->B.x)/2 , (this->A.y+this->B.y)/2 );
    calcularDistancia();
}

float math::lineaRecta::puntoEnY(float puntoX)
{
    return m*puntoX + b;
}

float math::lineaRecta::puntoEnX(float puntoY)
{
    return (puntoY-b) / m;
}

float math::lineaRecta::anguloEntre2Rectas(math::lineaRecta lA, math::lineaRecta lB)
{
    OrganizarRectas(lA,lB);
    qDebug()<<"------------------------";
    qDebug()<<"m1"<<lA.m<<" - "<<"m2"<<lB.m;
    qDebug()<<"------------------------";

    float m1 = lA.m;
    float m2 = lB.m;

    float tangTeta = (m1-m2)/(1+m1*m2);

    return atan( tangTeta )*(180/M_PI);
}


