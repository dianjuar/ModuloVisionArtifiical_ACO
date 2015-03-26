#ifndef CONFIG_MATINTSENDER_H
#define CONFIG_MATINTSENDER_H

#include "config_senderBase.h"
#include "stand_Tools.h"

namespace CONFIG
{

class matIntSender : public senderBase
{
public:
    static const int port = 6666;
    matIntSender(QString serverDir);

    void enviarInformacion(int **mat, int n, float dist);
    void enviarInformacion(QString mat, float dist);

    void write(FileStorage &fs) const;
    void read(const FileNode& node);

private:


};

}
#endif // CONFIG_MATINTSENDER_H
