#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H

#include "INCLUDE_QTstuff.h"
#include "stand_capturadorimagen.h"
#include "stand_Tools.h"

#include "config_cropper.h"
#include "config_umbralizador.h"
#include "config_circledetect.h"
#include "config_partirncuadros.h"
#include "config_INTMatBuilder.h"
#include "config_matintsender.h"

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
    void listen_matFromVideoCapture();

    void on_Q_Ndispositivo_SpinBox_valueChanged(int arg1);
    void on_btn_siguiente_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_btn_atras_clicked();
    void on_slider_umbralBlackAndWhite_valueChanged(int value);
    void on_slider_HOUGH_min_dist_valueChanged(int value);
    void on_slider_HOUGH_param_1_valueChanged(int value);
    void on_slider_HOUGH_param_2_valueChanged(int value);
    void on_slider_HOUGH_min_radius_valueChanged(int value);
    void on_slider_HOUGH_max_radius_valueChanged(int value);

    void on_slider_n_valueChanged(int value);

    void on_pushButton_clicked();

    void on_lineEdit_setverDir_F5_textEdited(const QString &arg1);

    void on_slider_CannyU_1_valueChanged(int value);

    void on_slider_CannyU_2_valueChanged(int value);

    void on_pushButton_2_clicked();

private:
    Ui::VentanaPrincipal *ui;

    //objetos de las clases necesarias para la calibración
    STAND::capturadorImagen *cap;
    CONFIG::cropper *crop;
    CONFIG::umbralizador *umb;
    CONFIG::circleDetect *cirD;
    CONFIG::partirNcuadros *PNcuadros;
    CONFIG::INTMatBuilder *IntMatB;
    CONFIG::matIntSender *mSender;

    void contectar_HiloCapturadorWITHVentanaPrincipal();

    int config_index;
    int config_Netapas;

    bool F1_dispositivoValido;
};

#endif // VENTANAPRINCIPAL_H
