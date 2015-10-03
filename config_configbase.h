#ifndef CONFIGBASE_H
#define CONFIGBASE_H

#include "INCLUDE_opencv.h"

namespace CONFIG
{
    class configBase
    {
    protected:
        bool todoEnOrden;

    public:
        configBase();
        virtual void write(FileStorage &fs) const{}
        virtual void read(const FileNode& node){}
        virtual void calibrar(Mat m){}

        bool isTodoEnOrden(){ return todoEnOrden; }
    };
}

#endif // CONFIGBASE_H
