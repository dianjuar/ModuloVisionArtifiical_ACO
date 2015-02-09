#include "config_INTMatBuilder.h"

using namespace CONFIG;

void INTMatBuilder::construir_INTMat_and_cartoon()
{
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
        {
           Mat miniMat = mat_original_BlackAndWhite->rowRange(i*tamano_cuadroAnalizar_MatrizCroped,
                                                             i*tamano_cuadroAnalizar_MatrizCroped + tamano_cuadroAnalizar_MatrizCroped);

           miniMat = miniMat.colRange(j*tamano_cuadroAnalizar_MatrizCroped,
                                                         j*tamano_cuadroAnalizar_MatrizCroped + tamano_cuadroAnalizar_MatrizCroped);

           //****ANALIZAR SI TIENE PIXELES NEGROS
           if( contieneNegro(miniMat) == false)
               INT_mat[i][j]= MAPA_libre;
           //************************************************

           miniMat.~Mat();
        }

    if(Validos_PuntosInicioFin())//crear matriz caricaturizada
    {
        contieneError = false;
        crear_MartCartooned();
    }
    else
    {
        contieneError = true;
        mat_cartooned = imread("./media/error.png");
    }

}

bool INTMatBuilder::Validos_PuntosInicioFin()
{
    Point ini_INTmat,fin_INTmat;

    ini_INTmat = Point(P_Inicio->x/tamano_cuadroAnalizar_MatrizCroped,
                       P_Inicio->y/tamano_cuadroAnalizar_MatrizCroped);
    fin_INTmat = Point(P_Fin->x/tamano_cuadroAnalizar_MatrizCroped,
                       P_Fin->y/tamano_cuadroAnalizar_MatrizCroped);

    //if( INT_mat[C1ii][C1jj] == MAPA_obstaculo || INT_mat[C2ii][C2jj] == MAPA_obstaculo )
    if( INT_mat[ini_INTmat.y][ini_INTmat.x] == MAPA_obstaculo || INT_mat[fin_INTmat.y][fin_INTmat.x] == MAPA_obstaculo )
        return false;
    else
    {
        INT_mat[ini_INTmat.y][ini_INTmat.x] = MAPA_inicio;
        INT_mat[fin_INTmat.y][fin_INTmat.x] = MAPA_fin;
        return true;
    }

}

bool INTMatBuilder::contieneNegro(Mat m)
{
    for( int y = 0; y < m.rows; y++ )
      for( int x = 0; x < m.cols; x++ )
          if ( m.at<uchar>(y,x) == 0 )
            return true;

    return false;
}

void INTMatBuilder::set_TamanosYEscalas()
{
    tamano_cuadroMatCartooned = tamano_MatCartooned / n;
    tamano_cuadroAnalizar_MatrizCroped = *tamano_MatrizCropped/ n;
}

void INTMatBuilder::copiar_CuadroMatCartoon_a_MatCartoon(int i, int j, int valor)
{
    Mat m;

    switch (valor)
    {
        case MAPA_libre:
            m = MAT_libre;
        break;

        case MAPA_obstaculo:
            m = MAT_obstaculo;
        break;

        case MAPA_inicio:
            m = MAT_inicio;
        break;

        case MAPA_fin:
            m = MAT_fin;
        break;
    }

    Rect roi = Rect(j*tamano_cuadroMatCartooned,i*tamano_cuadroMatCartooned,
                    tamano_cuadroMatCartooned, tamano_cuadroMatCartooned);

    //Mat subView = big(roi)
    Mat subView = mat_cartooned(roi);

    Mat aux;
    resize(m.clone(), aux,Size( tamano_cuadroMatCartooned, tamano_cuadroMatCartooned ));
    aux.copyTo(subView);

    m.~Mat();
}

void INTMatBuilder::crear_MartCartooned()
{
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            copiar_CuadroMatCartoon_a_MatCartoon(i,j, INT_mat[i][j]);
}

INTMatBuilder::INTMatBuilder(Mat *mat_original_BlackAndWhite, int n,int *tamano_MatrizCropped)
{
    this->mat_original_BlackAndWhite  =mat_original_BlackAndWhite;
    this->n = n;
    this->tamano_MatrizCropped = tamano_MatrizCropped;

    contieneError = true;

    MAT_libre = imread("./media/libre.png");
    MAT_obstaculo = imread("./media/obstaculo.png");
    MAT_inicio = imread("./media/inicio.png");
    MAT_fin = imread("./media/fin.png");

    tamano_imagenCartoonOriginal = MAT_libre.rows;

    tamano_MatCartooned = 350;
}

void INTMatBuilder::set_n(int n)
{
    this->n = n;
    set_TamanosYEscalas();

    mat_cartooned = Mat::zeros(Size(tamano_cuadroMatCartooned*(n),tamano_cuadroMatCartooned*(n)), CV_8UC3);


    INT_mat = new int* [n];

    for (int i = 0; i < n; i++)
    {
         INT_mat[i] = new int[n];

         for(int j=0;j<n;j++)
             INT_mat[i][j]=MAPA_obstaculo;
    }

    construir_INTMat_and_cartoon();
}

void INTMatBuilder::set_P_InicioYFin(Point *Inicio, Point *Fin)
{
    this->P_Inicio = Inicio;
    this->P_Fin = Fin;
}
