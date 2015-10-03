#ifndef CONFIG_COLORDETECTOR_H
#define CONFIG_COLORDETECTOR_H

#include "config_configbase.h"
#include "config_conexion_SMA.h"
#include "INCLUDE_QTstuff.h"
#include "stand_Tools.h"
#include "stand_capturadorimagen.h"
#include "config_INTMatBuilder.h"


namespace CONFIG
{
    namespace coTra //correccion de trayectoria
    {
        //Class prototyping
        class colorDetector_MANAGER;
        class colorDetector_WORKER;

        class colorDetector_MANAGER:public QObject, public configBase
        {
            Q_OBJECT
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

            Network::conexion_SMA *co_SMA;

            void recortar();
            void inicializar_sesgadores(int NumeroDeColores);

        public:
            colorDetector_MANAGER();

            void write(FileStorage &fs) const;
            void read(const FileNode& node);

            void calibrar(int Nsesgo);

            //getter
            int get_numeroDecolores(){ return NumeroDeColores; }

            colorDetector_WORKER **colorDetectorWORKERS;

        public slots:
            void RECIBIRsolicitud_CorreccionTrayectoria(int RobotID, int direccionRobot_Nominal,
                                                         int RobotPoint_Nominal_X, int RobotPoint_Nominal_Y);
        private slots:
            void RECIBIR_Despacho_CorreccionTrayectoria_FromWORKER(int robotID, float teta);

        signals:
            void DESPACHARsolicitud_CorreccionTrayectoria(int robotID, float teta);


        };
        //////////////////////////////////////////////////////////////////
        class colorDetector_WORKER : public QThread
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
            Rect rectanguloSesgador;


            bool selected;
            bool isPeticion;
            int ID;

            int direccionRobot_Nominal;
            Point RobotPoint_Nominal;


            QMutex sync;

            double h_h, l_h, h_s, l_s;
            bool detectarCirculos(Tools::math::circulo &base, Tools::math::circulo &direccional);

        public:

            colorDetector_WORKER(int ID,
                                 const int *low_diff, const int *high_diff,
                                 const int *conn,
                                 const int *val,
                                 const int *flags,
                                 const Mat *kernel_rectangular, const Mat *kernel_ovalado);

            double get_h_h(){ return h_h; }
            double get_l_h(){ return l_h; }
            double get_h_s(){ return h_s; }
            double get_l_s(){ return l_s; }

            void set_values4sesgo(double h_h, double l_h, double h_s, double l_s);

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
            void DESPACHAR_SolicitudDeTratectoria(int ID, float teta);

        };
        //////////////////////////////////////////////////////////////////

    }
}
#endif // CONFIG_COLORDETECTOR_H
