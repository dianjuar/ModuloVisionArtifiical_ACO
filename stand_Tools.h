#ifndef CV_TOOLS_H
#define CV_TOOLS_H

#include "INCLUDE_opencv.h"
#include "INCLUDE_QTstuff.h"

namespace STAND
{

class Tools
{
public:
    static Mat tratamientoDeImagenStantdar(Mat source, bool withGaussianBlur=true) //pasar a escala de grises y aplicar guassianBlur
    {
        Mat aux;
        cvtColor(source,aux,CV_RGBA2GRAY);

        if(withGaussianBlur)
        GaussianBlur(aux,aux,Size(9,9),2,2);//se ha jugado con anterioridad estos parametros y dan buenos resultados para el propósito que se desea

        return aux;
    }


    static QPixmap Mat2QPixmap(Mat m, bool resize = false, int x=400)
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


    static QPixmap Mat2QPixmap(Mat m, int scale)
    {
        cv::resize(m,m,Size(m.cols/scale,m.rows/scale),0,0,INTER_LINEAR);
        return Mat2QPixmap(m);
    }

    static QString IntMat2QString(int **mat, int n)
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

    static Rect contenedorMasGrande( vector< vector<Point> > contours)
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
};

class circulo
{
public:
    Point centro;
    int radio;

    circulo(Point centro, int radio)
    {
        this->centro = centro;
        this->radio = radio;
    }

    circulo(){ radio = -1; }
};

}


#endif // CV_TOOLS_H
