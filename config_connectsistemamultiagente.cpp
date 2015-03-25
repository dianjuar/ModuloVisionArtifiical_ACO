#include "config_connectsistemamultiagente.h"

using namespace CONFIG;

connectSistemaMultiAgente::connectSistemaMultiAgente(QString serverDir):senderBase(serverDir,port)
{

}

void connectSistemaMultiAgente::write(FileStorage &fs) const
{
    fs<<"{"<< "serverDir" << serverDir.toUtf8().data()<<"}";
}

void connectSistemaMultiAgente::read(const FileNode &node)
{
    std::string server;

    node["serverDir"] >> server;

    serverDir = QString::fromUtf8( server.c_str() );
}

