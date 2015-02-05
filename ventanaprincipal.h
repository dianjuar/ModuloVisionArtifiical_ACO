#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H

#include "INCLUDE_QTstuff.h"
#include "stand_capturadorimagen.h"

namespace Ui {
class VentanaPrincipal;
}

class VentanaPrincipal : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit VentanaPrincipal(QWidget *parent = 0);
    void set_labelDisplay( QImage QI);
    ~VentanaPrincipal();
    
private slots:
    void on_pushButton_clicked();
    void on_Q_Ndispositivo_SpinBox_valueChanged(int arg1);
    void listen_matFromVideoCapture(QImage qi);


private:
    Ui::VentanaPrincipal *ui;
    STAND_capturadorImagen *cap;

    void contectar_HiloCapturadorWITHVentanaPrincipal();

    bool F1_dispositivoValido;
    QImage convert_Mat2QImage(Mat);
};

#endif // VENTANAPRINCIPAL_H
