#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H

#include "INCLUDE_QTstuff.h"
#include "stand_capturadorimagen.h"
#include "config_cropper.h"
#include "config_umbralizador.h"
#include "stand_Tools.h"

namespace Ui
{
    class VentanaPrincipal;
}

class VentanaPrincipal : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit VentanaPrincipal(QWidget *parent = 0);
    void set_labelDisplay( Mat );
    ~VentanaPrincipal();
    
private slots:
    void on_Q_Ndispositivo_SpinBox_valueChanged(int arg1);
    void listen_matFromVideoCapture();
    void on_btn_siguiente_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_btn_atras_clicked();

    void on_slider_umbralBlackAndWhite_valueChanged(int value);

private:
    Ui::VentanaPrincipal *ui;

    //objetos de las clases necesarias para la calibración
    STAND::capturadorImagen *cap;
    CONFIG::cropper *crop;
    CONFIG::umbralizador *umb;



    void contectar_HiloCapturadorWITHVentanaPrincipal();

    int config_index;
    int config_Netapas;

    bool F1_dispositivoValido;
};

#endif // VENTANAPRINCIPAL_H
