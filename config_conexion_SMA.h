#ifndef CONFIG_CONNECTSISTEMAMULTIAGENTE_H
#define CONFIG_CONNECTSISTEMAMULTIAGENTE_H

#include "INCLUDE_QTstuff.h"
#include "config_configbase.h"
#include "stand_networking.h"
#include "stand_capturadorimagen.h"
#include "config_colordetector.h"
#include "config_INTMatBuilder.h"

namespace CONFIG
{
    namespace Network
    {
        class conexion_SMA: public ::Network::Client, public configBase
        {
            Q_OBJECT
        private:
            static const int port = 5003;
            coTra::colorDetector_MANAGER *colorDetector;

            void AnalizadorDeMensajes(QString msj);

        public:
            conexion_SMA(QString serverDir, ::CONFIG::coTra::colorDetector_MANAGER *colorDetector);

            void write(FileStorage &fs) const;
            void read(const FileNode& node);

        signals:
            void EMITIRsolicitud_CorreccionTrayectoria(int RobotID, int direccionRobot_Nominal,
                                                       int RobotPoint_Nominal_X, int RobotPoint_Nominal_Y);
        private slots:
            void RECIBIR_DESPACHO_solicitud_CorreccionTrayectoria(int robotID, float teta, double distanciaDesface, float anguloDesface);

        };
    } //namespace Network
} // namespace CONFIG

#endif // CONFIG_CONNECTSISTEMAMULTIAGENTE_H
