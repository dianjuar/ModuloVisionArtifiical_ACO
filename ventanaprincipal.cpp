#include "ventanaprincipal.h"
#include "ui_ventanaprincipal.h"

int VentanaPrincipal::FASE_NumeroFases;

VentanaPrincipal::VentanaPrincipal(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VentanaPrincipal)
{
    //para no tener problemas con los acentos
    QTextCodec *linuxCodec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(linuxCodec);
    QTextCodec::setCodecForCStrings(linuxCodec);
    QTextCodec::setCodecForLocale(linuxCodec);

    ui->setupUi(this);

    config_index = 0;
    //ui->tabWidget->setCurrentIndex( config_index );
    FASE_NumeroFases = ui->tabWidget->count();

    int modoElegido = STAND::capturadorImagen::Modo_ImagenStatica;

    cap = new STAND::capturadorImagen( modoElegido, ui->Q_Ndispositivo_SpinBox->value() );

    crop = new CONFIG::cropper( ui->slider_CannyU_1->value(), ui->slider_CannyU_2->value() );
    umb = new CONFIG::umbralizador( ui->slider_umbralBlackAndWhite->value() );
    /*cirD = new CONFIG::circleDetect( ui->slider_HOUGH_min_dist->value(),
                                     ui->slider_HOUGH_param_1->value(), ui->slider_HOUGH_param_2->value(),
                                     ui->slider_HOUGH_min_radius->value(), ui->slider_HOUGH_max_radius->value());*/
    PNcuadros = new CONFIG::partirNcuadros( ui->slider_n->value(), crop->get_tamano_MatrizCroped_SEGUIMIENTO() );
    IntMatB = new CONFIG::INTMatBuilder(umb->get_BlackAndWhite_SEGUIMIENTO(), PNcuadros->get_n(), crop->get_tamano_MatrizCroped_SEGUIMIENTO() );
    mSender = new CONFIG::matIntSender(ui->lineEdit_setverDir_F5->text());
    calib = new CONFIG::calibrador(ui->doubleSpinBox_F1_1->value(), Size(ui->slider_BoardSizeW_F1_1->value(), ui->slider_BoardSizeH_F1_1->value()),
                                   ui->slider_NFotos_F1_1->value(), ui->slider_delay_F1_1->value());

    ui->label_error_F5->setText( mSender->MSJ_sinComprobar );
    ui->label_connectionTest_F5->setPixmap( QPixmap( mSender->RUTAIMG_incorrecto ) );



    //variables utilizadas para manipular la barra de progreso del proceso de calibración
    numeroParametrosPorFaseCalib = new int[FASE_NumeroFases];

    numeroParametrosPorFaseCalib[ FASE_eleccionDeDispositivoDeGrabacion ] = 0;
    numeroParametrosPorFaseCalib[ FASE_calibracion ] = 1;
    numeroParametrosPorFaseCalib[ FASE_cortarContenido ] = 2;
    numeroParametrosPorFaseCalib[ FASE_seleccionUmbral ] = 1;
    numeroParametrosPorFaseCalib[ FASE_InicioFin ] = 5;
    numeroParametrosPorFaseCalib[ FASE_PartinN ] = 1;
    numeroParametrosPorFaseCalib[ FASE_EnvioEstacionCentral ] = 1;

    config_Nparametros = 0;

    for (int var = 0; var < FASE_NumeroFases; var++)
        config_Nparametros += numeroParametrosPorFaseCalib[var];

    //el hilo hará una llamada cada vez que capture una imagen por la cámara y la ventana principal atenderá esa llamada
    connect(cap, SIGNAL(tell()),
            this, SLOT(listen_matFromVideoCapture()));

    connect(calib, SIGNAL(tell()),
            this, SLOT(listen_matFromCalibrate()));

    connect(calib, SIGNAL(fotoTomada(int)),
            this, SLOT(set_FotoTomada_calibracion(int)));

    connect(calib, SIGNAL(CalibracionExitosa(bool)),
            this, SLOT(set_calibracionExitosa(bool)));

    connect(ui->label_display_IF, SIGNAL(clicked()),
            this, SLOT(Mouse_Pressed()) );

    //coloca todas las otras opciones desavilitadas para que el usuario no se salte los pasos
    for(int i=1;i<FASE_NumeroFases;i++)
        ui->tabWidget->setTabEnabled(i,false);
}

void VentanaPrincipal::set_labelDisplay(Mat m)
{

    switch(config_index)
    {
        case FASE_eleccionDeDispositivoDeGrabacion:
        {
            ui->label_displayF0->setPixmap( STAND::Tools::Mat2QPixmap(m) );
            break;
        }

        case FASE_calibracion:
        {
            ui->label_displayF1_1->setPixmap(STAND::Tools::Mat2QPixmap( m,2 ));

            break;
        }

        case FASE_cortarContenido:
        {
            crop->calibracion( m );
            ui->label_displayF1->setPixmap( STAND::Tools::Mat2QPixmap( crop->get_ImagenRayada() , 2)  );
            ui->label_cannyF1->setPixmap( STAND::Tools::Mat2QPixmap( crop->get_ImagenCanny(),2 ) );

            if(crop->hayContenedor())
            {
                ui->label_errorText_F1->setText("Todo en Orden");
                ui->label_errorImg_F1->setPixmap( QPixmap("./media/TestConnection/Right.png")  );
            }
            else
            {
                ui->label_errorText_F1->setText("No se detectó ningún cuadro");
                ui->label_errorImg_F1->setPixmap( QPixmap("./media/TestConnection/Bad.png")  );
            }
            break;
        }

        case FASE_seleccionUmbral:
        {
            Mat mCropped = m.clone();
            crop->cortarImagen(mCropped);
            umb->calibrar( mCropped );
            ui->label_displayF2->setPixmap( STAND::Tools::Mat2QPixmap( umb->get_BlackAndWhite(),true,420 ) );
            break;
        }

        case FASE_InicioFin:
        {
            ui->label_display_IF->setPixmap(  STAND::Tools::Mat2QPixmap( IntMatB->get_MatCartooned() ) );
            break;
        }

        case FASE_PartinN:
        {
            Mat mCropped = m.clone();
            crop->cortarImagen(mCropped);
            PNcuadros->calibrar(mCropped);

            ui->label_displayF4->setPixmap( STAND::Tools::Mat2QPixmap( mCropped,true,
                                                                     IntMatB->get_tamano_MatCartooned()) );
            ui->label_displayF4_Cartoon->setPixmap(  STAND::Tools::Mat2QPixmap( IntMatB->get_MatCartooned(),true,
                                                                                IntMatB->get_tamano_MatCartooned())  );
            break;
        }
    }

}

VentanaPrincipal::~VentanaPrincipal()
{
    delete ui;
}

void VentanaPrincipal::set_FotoTomada_calibracion(int num)
{
    QString t = QString::number( num );
    t.append("/");
    t.append( QString::number(ui->slider_NFotos_F1_1->value()) );
    ui->label_FotoTomada_F1_1->setText( t );
}

void VentanaPrincipal::set_calibracionExitosa(bool exito)
{
    if(exito)
        ui->label_TodoEnOrden_F1_1->setPixmap( QPixmap::fromImage(QImage("./media/TestConnection/Right.png")) );
    else
        ui->label_TodoEnOrden_F1_1->setPixmap( QPixmap::fromImage(QImage("./media/TestConnection/error.png")) );
}

void VentanaPrincipal::on_Q_Ndispositivo_SpinBox_valueChanged(int arg1)
{
    cap->deviceChanged( arg1 );
    ui->label_displayF0->setPixmap( QPixmap::fromImage(QImage("./media/CamNOT_FOUND.png")) );
 }

void VentanaPrincipal::listen_matFromVideoCapture()
{
    set_labelDisplay( cap->getImagen() );
}

void VentanaPrincipal::listen_matFromCalibrate()
{
    set_labelDisplay( calib->get_m()  );
}

void VentanaPrincipal::set_EnableSliderF1_1(bool enable, QPushButton *bMother,  QPushButton *bOther)
{
    ui->pushButton_startF1_1->setEnabled(enable);
    ui->pushButton_restartF1_1->setEnabled(enable);
    ui->doubleSpinBox_F1_1->setEnabled(enable);

    ui->slider_BoardSizeH_F1_1->setEnabled(enable);
    ui->slider_BoardSizeW_F1_1->setEnabled(enable);

    ui->slider_NFotos_F1_1->setEnabled(enable);
    ui->slider_delay_F1_1->setEnabled(enable);

    bMother->setEnabled(false);
    bOther->setEnabled(true);
}

void VentanaPrincipal::set_PorcenAvance_IN_progressBar(int NFaseCalib)
{
    int por_Progress=0;

    for(int i;i<NFaseCalib;i++)
        por_Progress += numeroParametrosPorFaseCalib[i];

     por_Progress = por_Progress*100/FASE_NumeroFases;

     ui->progressBar->setValue( por_Progress );
}

void VentanaPrincipal::pasarALaSiguienteEtapa()
{
    ui->tabWidget->setTabEnabled(++config_index,true);
    ui->tabWidget->setCurrentIndex(config_index);
}

void VentanaPrincipal::on_btn_siguiente_clicked()
{
    switch(config_index)
    {
        case FASE_eleccionDeDispositivoDeGrabacion:
        {
            if(cap->isCamaraAbierta())
            {
                ui->btn_atras->setEnabled(true);
                pasarALaSiguienteEtapa();
            }
            break;
        }

        case FASE_calibracion:
        {
            if(calib->get_todoEnOrden() || STAND::capturadorImagen::modo_elegido == STAND::capturadorImagen::Modo_ImagenStatica)
                pasarALaSiguienteEtapa();

            break;
        }

        case FASE_cortarContenido:
       {
            if(crop->hayContenedor())            
               pasarALaSiguienteEtapa();

            break;
        }

        case FASE_seleccionUmbral:
        {
            pasarALaSiguienteEtapa();

            IntMatB->set_n( PNcuadros->get_n() );
            break;
        }

        case FASE_InicioFin:
        {
            if(cirD->get_calibracionCorrecta())
            {
                pasarALaSiguienteEtapa();
            }
            break;
        }
        break;

        case FASE_PartinN:
        {            
            pasarALaSiguienteEtapa();

            if(STAND::capturadorImagen::modo_elegido == STAND::capturadorImagen::Modo_Video)
                calib->set_distanciaEntreCuadros( PNcuadros->get_distanciaEnPXentreCuadros(), PNcuadros->get_n() );
            else
                calib->set_distanciaEntreCuadros(1.0);
        }
        break;

        case FASE_EnvioEstacionCentral:
        {
            if( mSender->get_buenaConexion() )
            {
                mSender->enviarInformacion(IntMatB->get_INT_mat(), IntMatB->get_n(), calib->get_distanciaEntreCuadros() );
            }
        }
        break;
    }
}

void VentanaPrincipal::on_tabWidget_currentChanged(int index)
{
    config_index=index;

    if(index!=FASE_calibracion && (!cap->isCamaraAbierta() || STAND::capturadorImagen::modo_elegido == STAND::capturadorImagen::Modo_ImagenStatica) )
    {
        calib->stop();
        cap->InicicarHilo();
    }

    set_PorcenAvance_IN_progressBar( index );

    //casos especiales
    switch(config_index)
    {
        case FASE_calibracion:
            cap->stop();
            calib->InicicarHilo( cap->get_device() );
        break;
    }
}

void VentanaPrincipal::on_btn_atras_clicked()
{
    if(config_index != FASE_eleccionDeDispositivoDeGrabacion)
    {
        ui->tabWidget->setCurrentIndex(--config_index);
        if(config_index == FASE_eleccionDeDispositivoDeGrabacion)
            ui->btn_atras->setEnabled(false);
    }
}

void VentanaPrincipal::on_slider_umbralBlackAndWhite_valueChanged(int value)
{
    umb->setUmbral( value );
}

void VentanaPrincipal::on_slider_n_valueChanged(int value)
{
    PNcuadros->set_n( ui->slider_n->value() );
    IntMatB->set_n( PNcuadros->get_n() );
    ui->label_CuadrosRedondeo->setText( QString::number(PNcuadros->get_cuantosCuadrosSonNecesarios()) );
}

void VentanaPrincipal::on_lineEdit_setverDir_F5_textEdited(const QString &arg1)
{
    mSender->set_buenaConexion(false);
    ui->label_error_F5->setText( mSender->MSJ_sinComprobar );
    ui->label_connectionTest_F5->setPixmap( QPixmap( mSender->RUTAIMG_incorrecto ) );
}

void VentanaPrincipal::on_slider_CannyU_1_valueChanged(int value)
{
    crop->set_cannyU_1( value );
}

void VentanaPrincipal::on_slider_CannyU_2_valueChanged(int value)
{
    crop->set_cannyU_2( value );
}

void VentanaPrincipal::on_pushButton_2_clicked()
{
    crop->reset_contenedor();
}

void VentanaPrincipal::on_pushButton_startF1_1_clicked()
{
    set_EnableSliderF1_1(false, ui->pushButton_startF1_1, ui->pushButton_restartF1_1);
    calib->set_calibrar();
}

void VentanaPrincipal::on_pushButton_restartF1_1_clicked()
{
    set_EnableSliderF1_1(true, ui->pushButton_restartF1_1, ui->pushButton_startF1_1);
    ui->label_TodoEnOrden_F1_1->setPixmap( QPixmap("./media/TestConnection/Bad.png") );
    calib->set_standbay();
}

void VentanaPrincipal::on_slider_BoardSizeW_F1_1_valueChanged(int value)
{
    calib->set_boardSize( Size( ui->slider_BoardSizeW_F1_1->value(), ui->slider_BoardSizeH_F1_1->value()) );
}

void VentanaPrincipal::on_slider_BoardSizeH_F1_1_valueChanged(int value)
{
    calib->set_boardSize( Size( ui->slider_BoardSizeW_F1_1->value(), ui->slider_BoardSizeH_F1_1->value()) );
}

void VentanaPrincipal::on_slider_NFotos_F1_1_valueChanged(int value)
{
    set_FotoTomada_calibracion(0);

    calib->set_NFotos( ui->slider_NFotos_F1_1->value() );
}

void VentanaPrincipal::on_slider_delay_F1_1_valueChanged(int value)
{
    calib->set_delay( ui->slider_delay_F1_1->value() );
}

void VentanaPrincipal::on_doubleSpinBox_F1_1_valueChanged(double arg1)
{
    calib->set_squareSize( ui->doubleSpinBox_F1_1->value() );
}

void VentanaPrincipal::on_pushButton_ProbarConexion_EstCentral_clicked()
{
    ui->label_error_F5->setText( mSender->MSJ_comprobando );
    QMovie *movie = new QMovie( mSender->RUTAIMG_comprobando );
    ui->label_connectionTest_F5->setMovie(movie);
    movie->start();

    mSender->testConnection();

    if(mSender->get_buenaConexion())
    {
        ui->label_error_F5->setText( mSender->MSJ_correcto );
        ui->label_connectionTest_F5->setPixmap( QPixmap( mSender->RUTAIMG_correcto ) );
    }
    else
    {
        ui->label_error_F5->setText( mSender->MSJ_incorrecto );
        ui->label_connectionTest_F5->setPixmap( QPixmap( mSender->RUTAIMG_incorrecto ) );
    }
}

void VentanaPrincipal::on_pushButton_setPI_IF_clicked()
{
    ui->label_I_IF->setEnabled(true);
    ui->label_F_IF->setEnabled(false);
}

void VentanaPrincipal::on_pushButton_setPF_IF_clicked()
{
    ui->label_I_IF->setEnabled(false);
    ui->label_F_IF->setEnabled(true);
}

void VentanaPrincipal::Mouse_Pressed()
{
    qDebug()<<ui->label_display_IF->x<<ui->label_display_IF->y;
}

