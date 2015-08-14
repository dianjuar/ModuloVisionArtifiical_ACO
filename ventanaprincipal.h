#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H

#include "INCLUDE_QTstuff.h"
#include "stand_capturadorimagen.h"
#include "stand_Tools.h"

#include "config_cropper.h"
#include "config_umbralizador.h"
#include "config_partirncuadros.h"
#include "config_INTMatBuilder.h"
#include "config_matintsender.h"
#include "config_calibrador.h"
#include "config_guardarycargarparametros.h"
#include "config_connectsistemamultiagente.h"

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

    static int FASE_NumeroFases;
    static const int FASE_eleccionDeDispositivoDeGrabacion = 0;
    static const int FASE_calibracion = 1;
    static const int FASE_cortarContenido = 2;
    static const int FASE_seleccionUmbral = 3;
    static const int FASE_PartinN = 4;
    static const int FASE_InicioFin = 5;
    static const int FASE_EnvioEstacionCentral = 6;
    static const int FASE_EnvioSMA = 7;
    
private slots:
    void setted_PuntoI(bool);
    void setted_PuntoF(bool);

    void listen_matFromVideoCapture();

    void on_Q_Ndispositivo_SpinBox_valueChanged(int arg1);
    void on_btn_siguiente_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_btn_atras_clicked();
    void on_slider_umbralBlackAndWhite_valueChanged(int value);

    void on_lineEdit_setverDir_F5_textEdited(const QString &arg1);

    void on_slider_CannyU_1_valueChanged(int value);

    void on_slider_CannyU_2_valueChanged(int value);

    void on_pushButton_2_clicked();

    void on_pushButton_ProbarConexion_EstCentral_clicked();

    void on_pushButton_setPI_IF_clicked();

    void on_pushButton_setPF_IF_clicked();

    void Mouse_Pressed_DeteccionCirculos(int x, int y);

    void on_actionCargar_Configuraci_n_triggered();

    void on_lineEdit_setverDir_SMA_textEdited(const QString &arg1);

    void on_pushButton_ProbarConexion_SMA_clicked();

    void on_pushButton_F1_1_buscarArchivo_clicked();

    void on_slider_n_valueChanged(int value);

private:
    Ui::VentanaPrincipal *ui;

    //objetos de las clases necesarias para la calibración
    STAND::capturadorImagen *cap;
    CONFIG::cropper *crop;
    CONFIG::umbralizador *umb;
    CONFIG::partirNcuadros *PNcuadros;
    CONFIG::INTMatBuilder *IntMatB;
    CONFIG::matIntSender *mSender;
    CONFIG::calibrador *calib;
    CONFIG::guardarYCargarParametros *GCparam;
    CONFIG::connectSistemaMultiAgente *conSMA;

    void contectar_HiloCapturadorWITHVentanaPrincipal();
    void set_PorcenAvance_IN_progressBar(int NFaseCalib);
    void pasarALaSiguienteEtapa();
    void crearVentanaAfterCalibracion();
    void inhabilitarTodasLasPestanas();

    int config_index;
    int config_Nparametros;

    int *numeroParametrosPorFaseCalib;

    bool calibrando;
};

#endif // VENTANAPRINCIPAL_H
