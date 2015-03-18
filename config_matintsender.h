#ifndef CONFIG_MATINTSENDER_H
#define CONFIG_MATINTSENDER_H

#include "config_senderBase.h"
#include "INCLUDE_QTstuff.h"

namespace CONFIG
{

class matIntSender : public senderBase
{
public:
    static const int port = 6666;
    matIntSender(QString serverDir);

    void enviarInformacion(int **mat, int n, float dist);

private:    
    QString IntMat2QString(int **mat, int n);


};

}
#endif // CONFIG_MATINTSENDER_H
