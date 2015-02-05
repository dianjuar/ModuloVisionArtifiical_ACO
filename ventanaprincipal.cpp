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
    crop = CONFIG::cropper( ui->slider_CannyU_1->value(), ui->slider_CannyU_2->value() );

    config_index =0;
    config_Netapas = ui->tabWidget->count();
}

void VentanaPrincipal::set_labelDisplay(Mat m)
{
    switch(config_index)
    {
        case 0:
            ui->label_displayF0->setPixmap( QPixmap::fromImage( STAND::Tools::Mat2QImage(m) ) );
        break;
        case 1:
            crop.calibracion( m );
            ui->label_displayF1->setPixmap( QPixmap::fromImage( STAND::Tools::Mat2QImage( crop.get_ImagenRayada() ) ) );
        break;
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
            if(cap->isCamaraAbierta())
            {
                ui->tabWidget->setTabEnabled(++config_index,true);
                ui->btn_atras->setEnabled(true);
                ui->tabWidget->setCurrentIndex(config_index);
                qDebug()<<config_index;
            }
        break;

        case 1:
            if(crop.hayContenedor())
            {
                ui->tabWidget->setTabEnabled(++config_index,true);
                ui->tabWidget->setCurrentIndex(config_index);

                qDebug()<<config_index;
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
            ui->progressBar->setValue( 0);
        break;

        case 1:
            ui->progressBar->setValue( 10 );
        break;

        case 2:
            ui->progressBar->setValue( 30 );
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
