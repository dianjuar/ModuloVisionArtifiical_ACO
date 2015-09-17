#ifndef CONFIG_COLORDETECTOR_H
#define CONFIG_COLORDETECTOR_H

#include "INCLUDE_opencv.h"
#include "INCLUDE_QTstuff.h"
#include "stand_Tools.h"
#include "stand_capturadorimagen.h"


namespace CONFIG
{
    namespace coTra //correccion de trayectoria
    {
        //Class prototyping
        class colorDetector;
        class sesgador;
        class correctorTrayectoria;

        class colorDetector
        {

        private:
            //estos 5 valores fueron sacados despues de varias pruebas en el laboratorio de prototipos y fueron los que mejores resultado arrojaron
            int low_diff;
            int high_diff;
            int conn;
            int val;
            int flags;

            Mat kernel_rectangular;
            Mat kernel_ovalado;

            int NumeroDeColores;

            void recortar();

        public:
            colorDetector();

            void write(FileStorage &fs) const;
            void read(const FileNode& node);

            void calibrar(int Nsesgo);

            //getter
            int get_numeroDecolores(){ return NumeroDeColores; }

            sesgador *sesgador3colores;
        };
        //////////////////////////////////////////////////////////////////
        class sesgador : public QThread
        {
           Q_OBJECT
        private:

            const int *low_diff;
            const int *high_diff;
            const int *conn;
            const int *val;
            const int *flags;

            const Mat *kernel_rectangular;
            const Mat *kernel_ovalado;

            Mat *frame;
            Mat frame_hsv;
            Mat mask;
            Mat frame_thresholded;
            Mat frame_sesgado;
            bool selected;
            int ID;

            QMutex sync;

            double h_h, l_h, h_s, l_s;

        public:

            sesgador();

            double get_h_h(){ return h_h; }
            double get_l_h(){ return l_h; }
            double get_h_s(){ return h_s; }
            double get_l_s(){ return l_s; }

            void set_values4sesgo(double h_h, double l_h, double h_s, double l_s);
            void set_parameters4sesgo(int ID,
                                      const int *low_diff, const int *high_diff,
                                      const int *conn,
                                      const int *val,
                                      const int *flags,
                                      const Mat *kernel_rectangular, const Mat *kernel_ovalado);

            void calibrar();
            void recortar();

            //setters
            void set_seedPoint(Point p);

            //getters
            Mat get_frame_thresholded(){ return frame_thresholded; }
            Mat get_frame_sesgado(){ return frame_sesgado; }

            void corregirTrayectoria(const int direccionRobot_Nominal, const Point RobotPoint_Nominal);

            void run();

        public slots:

        signals:
           void correccionDeTrayectoriaProcesada(int ID, float correc_grados, float correc_dist);

        };
        //////////////////////////////////////////////////////////////////

    }
}
#endif // CONFIG_COLORDETECTOR_H
