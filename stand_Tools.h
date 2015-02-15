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

        if(m.type()==CV_8UC1)
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
        // 8-bits unsigned, NO. OF CHANNELS=3
        if(m.type()==CV_8UC3)
        {
            // Copy input Mat
            const uchar *qImageBuffer = (const uchar*)m.data;
            // Create QImage with same dimensions as input Mat
            img = QImage(qImageBuffer, m.cols, m.rows, m.step, QImage::Format_RGB888);
            img = img.rgbSwapped();
        }
        else
        {
            qDebug() << "ERROR: Mat could not be converted to QImage.";
            img = QImage();
        }

        return QPixmap::fromImage(img);
    }

    static QPixmap Mat2QPixmap(Mat m, int scale)
    {
        cv::resize(m,m,Size(m.cols/scale,m.rows/scale),0,0,INTER_LINEAR);
        return Mat2QPixmap(m);
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
