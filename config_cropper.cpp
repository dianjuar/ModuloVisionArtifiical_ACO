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
    findContours(canny_edges, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0,0) );

    set_MayorContenedor( STAND::Tools::contenedorMasGrande(contours) );

    rectangle(imagen_Rayada,contenedor,Scalar(0,0,255), 2);


}

void cropper::cortarImagen(Mat &m)
{
    m = m( contenedor );

    bool ahorroDeCodigo = m.rows > m.cols;
    tamano_XxX_MatrizCroped = ahorroDeCodigo ? m.rows : m.cols;
    int centrado = ahorroDeCodigo ? (tamano_XxX_MatrizCroped-m.cols)/2:(tamano_XxX_MatrizCroped-m.rows)/2;

    copyMakeBorder(m.clone(), m,
                   ahorroDeCodigo ?  0:centrado,
                   ahorroDeCodigo ?  0:(centrado /*+ (tamano_XxX_MatrizCroped/2)%2*/),
                   ahorroDeCodigo ?  centrado:0,
                   ahorroDeCodigo ?  (centrado /*+ (tamano_XxX_MatrizCroped/2)%2*/):0  ,
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


