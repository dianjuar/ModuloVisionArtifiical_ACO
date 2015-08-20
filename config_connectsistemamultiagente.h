#ifndef CONFIG_CONNECTSISTEMAMULTIAGENTE_H
#define CONFIG_CONNECTSISTEMAMULTIAGENTE_H

#include "stand_senderBase.h"
#include "INCLUDE_QTstuff.h"
#include "INCLUDE_opencv.h"

namespace CONFIG {

class connectSistemaMultiAgente: public STAND::senderBase
{
    static const int port = 5003;

public:
    connectSistemaMultiAgente(QString serverDir);

    void write(FileStorage &fs) const;
    void read(const FileNode& node);

    void sendConex();
};

} // namespace CONFIG

#endif // CONFIG_CONNECTSISTEMAMULTIAGENTE_H
