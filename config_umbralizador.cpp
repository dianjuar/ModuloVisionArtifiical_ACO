#include "config_umbralizador.h"

using namespace CONFIG;

umbralizador::umbralizador(int umbral)
{
    this->umbral = umbral;
    kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
}

void umbralizador::calibrar(Mat m)
{
    Mat grises = Tools::OpenCV::tratamientoDeImagenStantdar(m,false);
    threshold(grises,blackAndWhite,umbral,255,THRESH_BINARY);

    morphologyEx(blackAndWhite, blackAndWhite, MORPH_CLOSE, kernel);
    morphologyEx(blackAndWhite, blackAndWhite, MORPH_OPEN, kernel);

}
