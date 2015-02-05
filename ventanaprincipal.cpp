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

    cap = new STAND_capturadorImagen( STAND_capturadorImagen::Modo_Video, ui->Q_Ndispositivo_SpinBox->value() );

    contectar_HiloCapturadorWITHVentanaPrincipal();

    config_index =0;
    config_Netapas = ui->tabWidget->count();
    config_etapaCompletada = new bool[ config_Netapas ];
    config_progresStatus =0;

    for(int i =0; i < config_Netapas; i++)
        config_etapaCompletada[i] = false;



}

void VentanaPrincipal::set_labelDisplay(QImage QI)
{
    ui->label_display->setPixmap( QPixmap::fromImage(QI) );
}


VentanaPrincipal::~VentanaPrincipal()
{
    delete ui;
}

void VentanaPrincipal::on_pushButton_clicked()
{
    ui->tabWidget->setTabEnabled(2,false);
}

void VentanaPrincipal::on_Q_Ndispositivo_SpinBox_valueChanged(int arg1)
{
    if(cap->isCamaraAbierta())
        disconnect(cap, SIGNAL(tell_Mat(QImage)),
                   this, SLOT(listen_matFromVideoCapture(QImage)));

    cap->~STAND_capturadorImagen();
    set_labelDisplay( QImage("./media/CamNOT_FOUND.png") );

    cap = new STAND_capturadorImagen(STAND_capturadorImagen::Modo_Video, arg1 );

    contectar_HiloCapturadorWITHVentanaPrincipal();
}

void VentanaPrincipal::listen_matFromVideoCapture(QImage qi)
{
    set_labelDisplay(qi);
}

//el hilo lanzará la matriz proveniente de la camara y VentanaPrincipal la captará y la pondrá en la pantalla
void VentanaPrincipal::contectar_HiloCapturadorWITHVentanaPrincipal()
{


    F1_dispositivoValido = false;
    cap->start();

    if( cap->isCamaraAbierta() )
    {
        F1_dispositivoValido=true;

        connect(cap, SIGNAL(tell_Mat(QImage)),
            this, SLOT(listen_matFromVideoCapture(QImage)));
    }
    else
    {
        qDebug()<<"no se pudo abrir el dispositivo";
    }
}

void VentanaPrincipal::set_valueProgresBar(int value)
{
    config_progresStatus = value;
    ui->progressBar->setValue( config_progresStatus );
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

                set_valueProgresBar( config_progresStatus+10 );

                qDebug()<<config_index;
            }
        break;
    }
}

void VentanaPrincipal::on_tabWidget_currentChanged(int index)
{
    config_index=index;
}

void VentanaPrincipal::on_btn_atras_clicked()
{
    if(config_index!=0)
    {
        ui->tabWidget->setCurrentIndex(--config_index);
        if(config_index ==0)
            ui->btn_atras->setEnabled(false);

        switch(config_index)
        {
            case 0:
                set_valueProgresBar(0);
            break;
        }
    }
}
