#include "config_cropper.h"

using namespace CONFIG;

void cropper::set_cannyU_1(int c)
{
    canny_umbral_1 =c;
}
void cropper::set_cannyU_2(int c)
{
    canny_umbral_2 =c;
}

void cropper::calibracion(Mat mat)
{
    imagen_Rayada = mat.clone();

    Mat matTratada;

    matTratada = STAND::Tools::tratamientoDeImagenStantdar(imagen_Rayada);

    Canny(matTratada, canny_edges, canny_umbral_1, canny_umbral_2);

    vector< vector<Point> > contours;
    findContours(canny_edges, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );

    set_MayorContenedor( STAND::Tools::contenedorMasGrande(contours) );

    rectangle(imagen_Rayada,contenedor,Scalar(0,0,255), 2);


}

void cropper::cortarImagen(Mat &m)
{
    m = m( contenedor );

    bool altoMayorAncho = m.rows > m.cols;
    tamano_MatrizCortada = altoMayorAncho ? m.rows : m.cols;
    int centrado = altoMayorAncho ? (tamano_MatrizCortada-m.cols)/2:(tamano_MatrizCortada-m.rows)/2;

    bool esPar = (tamano_MatrizCortada/2) % 2 == 0;

    copyMakeBorder(m.clone(), m,
                   altoMayorAncho ?  0:centrado,
                   altoMayorAncho ?  0:( centrado + (esPar ? 0:1) )  ,
                   altoMayorAncho ?  centrado:0,
                   altoMayorAncho ?  ( centrado + (esPar ? 0:1) ):0  ,
                   BORDER_CONSTANT);
}

void cropper::reset_contenedor()
{
    hay_Contenedor = false;
    contenedor = Rect();
}

void cropper::write(FileStorage &fs) const
{
    fs << "{" <<"contenedor" << contenedor  << "}";
}

void cropper::read(const FileNode &node)
{
    std::vector<int> contenedorVec;
    node["contenedor"] >> contenedorVec;
    contenedor = Rect(contenedorVec[0], contenedorVec[1], contenedorVec[2], contenedorVec[3]);
}

void cropper::set_MayorContenedor(Rect r)
{
    if(r.area() > contenedor.area())
    {
        hay_Contenedor = true;
        contenedor = r;
    }

}

cropper::cropper(int canny_umbral_1, int canny_umbral_2)
{
    this->canny_umbral_1 = canny_umbral_1;
    this->canny_umbral_2 = canny_umbral_2;
    hay_Contenedor = false;

    contenedor = Rect();
}


