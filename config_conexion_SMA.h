#ifndef CONFIG_CONNECTSISTEMAMULTIAGENTE_H
#define CONFIG_CONNECTSISTEMAMULTIAGENTE_H

#include "INCLUDE_QTstuff.h"
#include "INCLUDE_opencv.h"
#include "stand_networking.h"
#include "config_INTMatBuilder.h"
#include "config_colordetector.h"

namespace CONFIG
{
    namespace Network
    {
        class conexion_SMA: public ::Network::DataClient
        {
            Q_OBJECT
        private:
            static const int port = 5003;

            coTra::colorDetector *colorD;

            void AnalizadorDeMensajes(QString msj);

        public:
            conexion_SMA(QString serverDir, coTra::colorDetector *colorD);

            void write(FileStorage &fs) const;
            void read(const FileNode& node);

        public slots:
            void recibirResultado_SolicitudCorreccionDeTrayectoria(int RobotID,float correc_grados, float correc_dist);

        };
    } //namespace Network
} // namespace CONFIG

#endif // CONFIG_CONNECTSISTEMAMULTIAGENTE_H
