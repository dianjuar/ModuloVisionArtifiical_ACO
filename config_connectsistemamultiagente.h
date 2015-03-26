#ifndef CONFIG_CONNECTSISTEMAMULTIAGENTE_H
#define CONFIG_CONNECTSISTEMAMULTIAGENTE_H

#include "config_senderBase.h"
#include "INCLUDE_QTstuff.h"
#include "INCLUDE_opencv.h"

namespace CONFIG {

class connectSistemaMultiAgente: public senderBase
{
    static const int port = 6667;

public:
    connectSistemaMultiAgente(QString serverDir);

    void write(FileStorage &fs) const;
    void read(const FileNode& node);

    void sendConex();
};

} // namespace CONFIG

#endif // CONFIG_CONNECTSISTEMAMULTIAGENTE_H
