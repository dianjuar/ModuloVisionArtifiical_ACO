#ifndef CONFIG_MATINTSENDER_H
#define CONFIG_MATINTSENDER_H

#include "config_configbase.h"
#include "stand_Tools.h"
#include "stand_networking.h"

namespace CONFIG
{
    namespace Network
    {
        class conexion_ACO: public ::Network::Client, public configBase
        {
        public:
            static const int port = 5000;
            conexion_ACO(QString serverDir);

            void enviarInformacion(int **mat, int n, float dist);
            void enviarInformacion(QString mat, float dist);

            void write(FileStorage &fs) const;
            void read(const FileNode& node);

        private:
            QString prepararMSJ_enviarMatrix(int **mat, float dist, int n);
            QString prepararMSJ_enviarMatrix(QString mat, float dist);
            void AnalizadorDeMensajes(QString msj);

        };
    }//namespace Network
}//namespace CONFIG
#endif // CONFIG_MATINTSENDER_H
