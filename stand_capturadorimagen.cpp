#include "stand_capturadorimagen.h"


void STAND_capturadorImagen::run()
{
    if(modo_elegido==Modo_Video)
    {
        while(1)
        {
            {
                QMutexLocker locker(&m_mutex);
                if (thread_stop) break;
                ///////////////////////////
                vc >> Imagen;
                this->tell_Mat( Mat2QImage(Imagen) );
                ///////////////////////////
            }
            msleep(1);
        }
    }
}

STAND_capturadorImagen::STAND_capturadorImagen(int modo, int devise)
{
    thread_stop = false;

    this->modo_elegido = modo;
    this->devise = devise;

    switch(modo_elegido)
    {
        case Modo_ImagenStatica:
            Imagen = imread("./media/static.png");
        break;

        case Modo_Video:
            vc.open(devise);
        break;
    }
}

STAND_capturadorImagen::~STAND_capturadorImagen()
{
    stop();
    vc.release();
    //delete this;
}

void STAND_capturadorImagen::stop()
{
    QMutexLocker locker(&m_mutex);
    thread_stop = true;
}

QImage STAND_capturadorImagen::Mat2QImage(Mat m)
{
    if(Imagen.type()==CV_8UC1)
    {
        // Set the color table (used to translate colour indexes to qRgb values)
        QVector<QRgb> colorTable;
        for (int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)Imagen.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, Imagen.cols, Imagen.rows, Imagen.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);

        return img;
    }
    // 8-bits unsigned, NO. OF CHANNELS=3
    if(Imagen.type()==CV_8UC3)
    {
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)Imagen.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, Imagen.cols, Imagen.rows, Imagen.step, QImage::Format_RGB888);
        return img.rgbSwapped();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}

bool STAND_capturadorImagen::isCamaraAbierta()
{
    return vc.isOpened();
}

