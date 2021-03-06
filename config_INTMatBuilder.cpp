#include "config_INTMatBuilder.h"

using namespace CONFIG;

Point INTMatBuilder::P_Fin;
Point INTMatBuilder::P_Inicio;
int INTMatBuilder::n;

void INTMatBuilder::construir_INTMat_and_cartoon()
{
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
        {
           Mat miniMat;

           miniMat = mat_original_BlackAndWhite->rowRange(i*tamano_cuadroAnalizar_MatrizCroped,
                                                          i*tamano_cuadroAnalizar_MatrizCroped + tamano_cuadroAnalizar_MatrizCroped);

           miniMat = miniMat.colRange(j*tamano_cuadroAnalizar_MatrizCroped,
                                      j*tamano_cuadroAnalizar_MatrizCroped + tamano_cuadroAnalizar_MatrizCroped);

           //****ANALIZAR SI TIENE PIXELES NEGROS
           if( contieneNegro(miniMat) == false)
               INT_mat[i][j]= MAPA_libre;
           //************************************************

           miniMat.~Mat();
        }

        crear_MartCartooned();

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

    tamano_cuadroAnalizar_MatrizCroped = cropper::tamano_MatrizCortada/ n;
}

void INTMatBuilder::copiar_CuadroMatCartoon_a_Mat(Mat &mat, int i, int j, int valor)
{
    Mat m_cartoon;

    int size= mat.cols/n;

    switch (valor)
    {
        case MAPA_libre:
            m_cartoon = MAT_libre;
        break;

        case MAPA_obstaculo:
            m_cartoon = MAT_obstaculo;
        break;

        case MAPA_inicio:
            m_cartoon = MAT_inicio;
        break;

        case MAPA_fin:
            m_cartoon = MAT_fin;
        break;
    }

    Rect roi = Rect(j*size,i*size,
                    size, size);


    //Mat subView = big(roi)
    Mat subView = mat(roi);

    Mat aux;
    resize(m_cartoon.clone(), aux,Size( size, size ));
    aux.copyTo(subView);

    m_cartoon.~Mat();
}

void INTMatBuilder::crear_MartCartooned()
{
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            copiar_CuadroMatCartoon_a_Mat(mat_cartooned,i,j, INT_mat[i][j]);
}

Point INTMatBuilder::convert_PointScreen2PointMat(Point p)
{
    return Point(p.x/tamano_cuadroMatCartooned,p.y/tamano_cuadroMatCartooned);
}

void INTMatBuilder::write(FileStorage &fs) const
{
    fs << "{" <<"QSINT_mat" << QSINT_mat.toUtf8().data()  <<
                "n" << n;
        fs << "INT_mat"<<"[";
            for( int i = 0; i < n; i++ )
            {
                fs <<"[:";
                for( int j = 0; j < n; j++ )
                    fs << INT_mat[i][j];
                fs << "]";
            }
        fs << "]";
    fs << "}";
}

void INTMatBuilder::read(const FileNode &node)
{
    std::string stdSINT_mat;

    node["QSINT_mat"] >> stdSINT_mat;
    QSINT_mat = QString( stdSINT_mat.c_str() );

    n = (int)node["n"];

    set_TamanosYEscalas();

    FileNode features = node["INT_mat"];
    FileNodeIterator it = features.begin(), it_end = features.end();

    INT_mat = new int* [n];
    for( int i=0; it != it_end; it++, i++ )
    {
        INT_mat[i] = new int[n];

        std::vector<int> vec;
        (*it) >> vec;

        for (int j = 0; j < n; j++)
        {
            INT_mat[i][j] = vec.at(j);

            if( INT_mat[i][j] == MAPA_fin )
                P_Fin = Point( j,i );

            if( INT_mat[i][j] == MAPA_inicio )
                P_Inicio = Point( j,i );
        }
    }

}

void INTMatBuilder::buildQSINTmat()
{
    QSINT_mat = Tools::general::IntMat2QString( INT_mat, n );
}

void INTMatBuilder::Cartoon_dibujarEnsima(Mat &m)
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if(INT_mat[i][j]==MAPA_obstaculo || INT_mat[i][j]==MAPA_inicio || INT_mat[i][j]==MAPA_fin)
                copiar_CuadroMatCartoon_a_Mat(m,i,j,INT_mat[i][j]);
}

INTMatBuilder::INTMatBuilder(Mat *mat_original_BlackAndWhite, int n)
{
    this->mat_original_BlackAndWhite = mat_original_BlackAndWhite;
    this->n = n;

    MAT_libre = imread("./media/libre.png");
    MAT_obstaculo = imread("./media/obstaculo.png");
    MAT_inicio = imread("./media/inicio.png");
    MAT_fin = imread("./media/fin.png");

    tamano_imagenCartoonOriginal = MAT_libre.rows;

    tamano_MatCartooned = 350;

    P_Inicio = Point(-1,-1);
    P_Fin = Point(-1,-1);

    bool_settedPuntoF = bool_settedPuntoI = false;
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

void INTMatBuilder::set_P_Inicio(Point Inicio)
{
    Inicio = convert_PointScreen2PointMat(Inicio);

    if(INT_mat[Inicio.y][Inicio.x]==MAPA_libre || INT_mat[Inicio.y][Inicio.x]==MAPA_fin || INT_mat[Inicio.y][Inicio.x]==MAPA_inicio)
    {

        if(INT_mat[Inicio.y][Inicio.x]==MAPA_fin)
        {
            bool_settedPuntoF = false;
            emit settedPuntoF(bool_settedPuntoF);
        }

        if(P_Inicio.x != -1 && P_Inicio.y != -1 )
        {
            INT_mat[P_Inicio.y][P_Inicio.x] = MAPA_libre;
            copiar_CuadroMatCartoon_a_Mat(mat_cartooned,P_Inicio.y,P_Inicio.x, MAPA_libre);
        }


        P_Inicio = Inicio;

        INT_mat[Inicio.y][Inicio.x] = MAPA_inicio;
        copiar_CuadroMatCartoon_a_Mat(mat_cartooned,Inicio.y,Inicio.x, MAPA_inicio);

        bool_settedPuntoI = true;
        emit settedPuntoI(bool_settedPuntoI);
    }

}

bool INTMatBuilder::isTodoEnOrden()
{
    todoEnOrden = bool_settedPuntoF&&bool_settedPuntoI;
    return todoEnOrden;
}


void INTMatBuilder::set_P_Fin(Point Fin)
{
    Fin = convert_PointScreen2PointMat(Fin);

    //validar que sea un espacio válido
    if(INT_mat[Fin.y][Fin.x]==MAPA_libre || INT_mat[Fin.y][Fin.x]==MAPA_fin || INT_mat[Fin.y][Fin.x]==MAPA_inicio)
    {

        if(INT_mat[Fin.y][Fin.x]==MAPA_inicio)
        {
            bool_settedPuntoI = false;
            emit settedPuntoI(bool_settedPuntoI);
        }

        if(P_Fin.x != -1 && P_Fin.y != -1 )
        {
            INT_mat[P_Fin.y][P_Fin.x] = MAPA_libre;
            copiar_CuadroMatCartoon_a_Mat(mat_cartooned, P_Fin.y,P_Fin.x, MAPA_libre);
        }

        P_Fin = Fin;

        INT_mat[Fin.y][Fin.x] = MAPA_fin;
        copiar_CuadroMatCartoon_a_Mat(mat_cartooned,Fin.y,Fin.x, MAPA_fin);

        bool_settedPuntoF = true;
        emit settedPuntoF(bool_settedPuntoF);

    }
}
