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
