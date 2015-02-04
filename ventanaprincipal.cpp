#include "ventanaprincipal.h"
#include "ui_ventanaprincipal.h"

VentanaPrincipal::VentanaPrincipal(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VentanaPrincipal)
{
    ui->setupUi(this);

    cap = new STAND_capturadorImagen( STAND_capturadorImagen::Modo_Video, ui->Q_Ndispositivo_SpinBox->value() );

    contectar_HiloCapturadorWITHVentanaPrincipal();

}

void VentanaPrincipal::set_labelDisplay(QImage QI)
{
    ui->label_display->setPixmap( QPixmap::fromImage( QI ));
}


VentanaPrincipal::~VentanaPrincipal()
{
    delete ui;
}

void VentanaPrincipal::on_pushButton_clicked()
{
}

void VentanaPrincipal::on_Q_Ndispositivo_SpinBox_valueChanged(int arg1)
{
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
    cap->start();

    if( cap->isCamaraAbierta() )
    {
    connect(cap, SIGNAL(tell_Mat(QImage)),
            this, SLOT(listen_matFromVideoCapture(QImage)));
    }
}
