#include "ventanaprincipal.h"
#include "ui_ventanaprincipal.h"

VentanaPrincipal::VentanaPrincipal(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VentanaPrincipal)
{
    ui->setupUi(this);

    //coloca todas las otras opciones desavilitadas para que el usuario no se salte los pasos
    for(int i=1;i<ui->tabWidget->count();i++)
        ui->tabWidget->setTabEnabled(i,false);

    cap = new STAND::capturadorImagen( STAND::capturadorImagen::Modo_ImagenStatica, ui->Q_Ndispositivo_SpinBox->value() );
    contectar_HiloCapturadorWITHVentanaPrincipal();

    crop = new CONFIG::cropper( ui->slider_CannyU_1->value(), ui->slider_CannyU_2->value() );
    umb = new CONFIG::umbralizador( ui->slider_umbralBlackAndWhite->value() );
    cirD = new CONFIG::circleDetect( ui->slider_HOUGH_min_dist->value(),
                                     ui->slider_HOUGH_param_1->value(), ui->slider_HOUGH_param_2->value(),
                                     ui->slider_HOUGH_min_radius->value(), ui->slider_HOUGH_max_radius->value());
    PNcuadros = new CONFIG::partirNcuadros( ui->slider_n->value(), crop->get_tamano_MatrizCroped_SEGUIMIENTO() );
    IntMatB = new CONFIG::INTMatBuilder(umb->get_BlackAndWhite_SEGUIMIENTO(), PNcuadros->get_n_SEGUIMIENTO());

    config_index =0;
    config_Netapas = ui->tabWidget->count();
}

void VentanaPrincipal::set_labelDisplay(Mat m)
{
    switch(config_index)
    {
        case 0:
        {
            ui->label_displayF0->setPixmap( QPixmap::fromImage( STAND::Tools::Mat2QImage(m) ) );
            break;
        }

        case 1:
        {
            crop->calibracion( m );
            ui->label_displayF1->setPixmap( QPixmap::fromImage( STAND::Tools::Mat2QImage( crop->get_ImagenRayada() ) ) );
            break;
        }

        case 2:
        {
            Mat mCropped = m.clone();
            crop->cortarImagen(mCropped);
            umb->calibrar( mCropped );
            ui->label_displayF2->setPixmap( QPixmap::fromImage( STAND::Tools::Mat2QImage( umb->get_BlackAndWhite(),true,420 ) ) );
            break;
        }

        case 3:
        {
            Mat mCropped = m.clone();
            crop->cortarImagen(mCropped);
            cirD->calibrar(mCropped);

            ui->label_displayF3->setPixmap( QPixmap::fromImage( STAND::Tools::Mat2QImage( mCropped,true ) ) );
            break;
        }

        case 4:
        {
            Mat mCropped = m.clone();
            crop->cortarImagen(mCropped);
            PNcuadros->calibrar(mCropped);

            //ui->label_CuadrosRedondeo->setText( QString::number(PNcuadros->get_cuantosCuadrosSonNecesarios()) );

            ui->label_displayF4->setPixmap( QPixmap::fromImage( STAND::Tools::Mat2QImage( mCropped,true,350) ) );
            ui->label_displayF4_Cartoon->setPixmap( QPixmap::fromImage( STAND::Tools::Mat2QImage( mCropped,true,350 ) ) );
            break;
        }

        case 5:
        {
            break;
        }

    }

}

VentanaPrincipal::~VentanaPrincipal()
{
    delete ui;
}

void VentanaPrincipal::on_Q_Ndispositivo_SpinBox_valueChanged(int arg1)
{
    if(cap->isCamaraAbierta())
        disconnect(cap, SIGNAL(tell_Mat(QImage)),
                   this, SLOT(listen_matFromVideoCapture(QImage)));

    cap->~capturadorImagen();
    ui->label_displayF0->setPixmap( QPixmap::fromImage(QImage("./media/CamNOT_FOUND.png")) );

    cap = new STAND::capturadorImagen(STAND::capturadorImagen::Modo_Video, arg1 );
    contectar_HiloCapturadorWITHVentanaPrincipal();
}

void VentanaPrincipal::listen_matFromVideoCapture()
{
    set_labelDisplay( cap->getImagen() );
}

//el hilo hará una llamada cada vez que capture una imagen por la cámara y la ventana principal atenderá esa llamada
void VentanaPrincipal::contectar_HiloCapturadorWITHVentanaPrincipal()
{
    F1_dispositivoValido = false;
    cap->start();

    if( cap->isCamaraAbierta() )
    {
        F1_dispositivoValido=true;

        connect(cap, SIGNAL(tell()),
                this, SLOT(listen_matFromVideoCapture()));
    }
    else
    {
        qDebug()<<"no se pudo abrir el dispositivo";
    }
}

void VentanaPrincipal::on_btn_siguiente_clicked()
{
    switch(config_index)
    {
        case 0:
        {
            if(cap->isCamaraAbierta())
            {
                ui->tabWidget->setTabEnabled(++config_index,true);
                ui->btn_atras->setEnabled(true);
                ui->tabWidget->setCurrentIndex(config_index);
            }
            break;
        }

        case 1:
       {
            if(crop->hayContenedor())
            {
                ui->tabWidget->setTabEnabled(++config_index,true);
                ui->tabWidget->setCurrentIndex(config_index);
            }
            break;
        }

        case 2:
        {
            ui->tabWidget->setTabEnabled(++config_index,true);
            ui->tabWidget->setCurrentIndex(config_index);

            qreal hipotenusa = qSqrt( 2*qPow( crop->get_tamano_MatrizCroped(),2 ) );
            ui->slider_HOUGH_param_2->setMaximum( hipotenusa );
            ui->slider_HOUGH_param_1->setMaximum( hipotenusa );
            ui->slider_HOUGH_min_radius->setMaximum( hipotenusa );
            ui->slider_HOUGH_min_dist->setMaximum( hipotenusa );
            ui->slider_HOUGH_max_radius->setMaximum( hipotenusa );
            break;
        }

        case 3:
        {
            if(cirD->get_calibracionCorrecta())
            {
                ui->tabWidget->setTabEnabled(++config_index,true);
                ui->tabWidget->setCurrentIndex(config_index);

                IntMatB->set_P_InicioYFin( cirD->get_PuntoInicio_SEGUIMIENTO(),cirD->get_PuntoFin_SEGUIMIENTO() );
            }
            break;
        }
        break;

        case 4:
        {

        }
        break;

        case 5:
        {

        }
        break;
    }
}

void VentanaPrincipal::on_tabWidget_currentChanged(int index)
{
    config_index=index;

    switch(config_index)
    {
        case 0:
            ui->progressBar->setValue( 0 );
        break;

        case 1:
            ui->progressBar->setValue( 1*100/11 );
        break;

        case 2:
            ui->progressBar->setValue( 3*100/11 );
        break;

        case 3:
            ui->progressBar->setValue( 4*100/11 );
        break;

        case 4:
            ui->progressBar->setValue( 9*100/11 );
        break;

        case 5:
            ui->progressBar->setValue( 10*100/11);
        break;
    }
}

void VentanaPrincipal::on_btn_atras_clicked()
{
    if(config_index!=0)
    {
        ui->tabWidget->setCurrentIndex(--config_index);
        if(config_index ==0)
            ui->btn_atras->setEnabled(false);
    }
}

void VentanaPrincipal::on_slider_umbralBlackAndWhite_valueChanged(int value)
{
    umb->setUmbral( value );
}

void VentanaPrincipal::on_slider_HOUGH_min_dist_valueChanged(int value)
{
    cirD->set_HOUGH_min_dist( value );
}

void VentanaPrincipal::on_slider_HOUGH_param_1_valueChanged(int value)
{
    cirD->set_HOUGH_param_1( value );
}

void VentanaPrincipal::on_slider_HOUGH_param_2_valueChanged(int value)
{
    cirD->set_HOUGH_param_2( value );
}

void VentanaPrincipal::on_slider_HOUGH_min_radius_valueChanged(int value)
{
    cirD->set_HOUGH_min_radius( value );
}

void VentanaPrincipal::on_slider_HOUGH_max_radius_valueChanged(int value)
{
    cirD->set_HOUGH_max_radius( value );
}


void VentanaPrincipal::on_slider_n_valueChanged(int value)
{
    PNcuadros->set_n( ui->slider_n->value() );
    ui->label_CuadrosRedondeo->setText( QString::number(PNcuadros->get_cuantosCuadrosSonNecesarios()) );
}
