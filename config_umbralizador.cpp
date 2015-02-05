#include "config_umbralizador.h"

using namespace CONFIG;

umbralizador::umbralizador(int umbral)
{
    this->umbral = umbral;
}

void umbralizador::calibracion(Mat m)
{
    Mat grises = STAND::Tools::tratamientoDeImagenStantdar(m,false);
    threshold(grises,blackAndWhite,umbral,255,THRESH_BINARY);
}
