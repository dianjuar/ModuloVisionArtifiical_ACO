#include "config_INTMatBuilder.h"

using namespace CONFIG;

void INTMatBuilder::construir_INTMat_and_cartoon()
{
    for(int i=0;i<*n;i++)
        for(int j=0;j<*n;j++)
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

}

bool INTMatBuilder::contieneNegroLosPuntos_InicioFin()
{
    Mat miniMat_INICIO = mat_original_BlackAndWhite->rowRange(P_Inicio->y*tamano_cuadroAnalizar_MatrizCroped,
                                                      P_Inicio->y*tamano_cuadroAnalizar_MatrizCroped + tamano_cuadroAnalizar_MatrizCroped);
    miniMat_INICIO = miniMat_INICIO.colRange(P_Inicio->x*tamano_cuadroAnalizar_MatrizCroped,
                                                  P_Inicio->x*tamano_cuadroAnalizar_MatrizCroped + tamano_cuadroAnalizar_MatrizCroped);

    Mat miniMat_FIN = mat_original_BlackAndWhite->rowRange(P_Fin->y*tamano_cuadroAnalizar_MatrizCroped,
                                                      P_Fin->y*tamano_cuadroAnalizar_MatrizCroped + tamano_cuadroAnalizar_MatrizCroped);
    miniMat_FIN = miniMat_FIN.colRange(P_Fin->x*tamano_cuadroAnalizar_MatrizCroped,
                                                  P_Fin->x*tamano_cuadroAnalizar_MatrizCroped + tamano_cuadroAnalizar_MatrizCroped);

    if( !contieneNegro(miniMat_INICIO) && !contieneNegro(miniMat_FIN) )
    {
        contieneError=false;

        INT_mat[P_Inicio->y/tamano_cuadroAnalizar_MatrizCroped][P_Inicio->x/tamano_cuadroAnalizar_MatrizCroped] =
                MAPA_inicio;

        INT_mat[P_Fin->y/tamano_cuadroAnalizar_MatrizCroped][P_Fin->x/tamano_cuadroAnalizar_MatrizCroped] =
                MAPA_fin;

    }
    else
        contieneError=true;

}

bool INTMatBuilder::contieneNegro(Mat m)
{
    for( int y = 0; y < m.rows; y++ )
      for( int x = 0; x < m.cols; x++ )
          if ( m.at<uchar>(y,x) == 0 )
            return true;

    return false;
}

INTMatBuilder::INTMatBuilder(Mat *mat_original_BlackAndWhite, int *n)
{
    this->mat_original_BlackAndWhite;
    this->n = n;

    contieneError = true;
    set_n(n);
}

void INTMatBuilder::set_n(int *n)
{
    this->n = n;

    INT_mat = new int* [*n];

    for (int i = 0; i < *n; i++)
    {
         INT_mat[i] = new int[*n];

         for(int j=0;j<*n;j++)
             INT_mat[i][j]=MAPA_obstaculo;
    }
}

void INTMatBuilder::set_P_InicioYFin(Point *Inicio, Point *Fin)
{
    this->P_Inicio = Inicio;
    this->P_Fin = Fin;
}
