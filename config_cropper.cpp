#include "config_cropper.h"

using namespace CONFIG;

int cropper::tamano_MatrizCortada = -255;

void cropper::set_cannyU_1(int c)
{
    canny_umbral_1 =c;
}
void cropper::set_cannyU_2(int c)
{
    canny_umbral_2 =c;
}

void cropper::calibrar(Mat mat)
{
    imagen_Rayada = mat.clone();

    Mat matTratada;

    matTratada = Tools::OpenCV::tratamientoDeImagenStantdar(imagen_Rayada);

    Canny(matTratada, canny_edges, canny_umbral_1, canny_umbral_2);

    vector< vector<Point> > contours;
    findContours(canny_edges, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );

    set_MayorContenedor( Tools::OpenCV::contenedorMasGrande(contours) );

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

    STAND::capturadorImagen::Imagen_Procesada = m;
}

void cropper::reset_contenedor()
{
    todoEnOrden = false;
    contenedor = Rect();
}

void cropper::write(FileStorage &fs) const
{
    fs << "{" <<"contenedor" << contenedor
              <<"tamano_MatrizCortada" << tamano_MatrizCortada
       << "}";
}

void cropper::read(const FileNode &node)
{
    std::vector<int> contenedorVec;
    node["contenedor"] >> contenedorVec;
    node["tamano_MatrizCortada"] >> tamano_MatrizCortada;
    contenedor = Rect(contenedorVec[0], contenedorVec[1], contenedorVec[2], contenedorVec[3]);
}

void cropper::set_MayorContenedor(Rect r)
{
    if(r.area() > contenedor.area())
    {
        todoEnOrden = true;
        contenedor = r;
    }

}

cropper::cropper(int canny_umbral_1, int canny_umbral_2)
{
    this->canny_umbral_1 = canny_umbral_1;
    this->canny_umbral_2 = canny_umbral_2;
    todoEnOrden = false;

    contenedor = Rect();
}


