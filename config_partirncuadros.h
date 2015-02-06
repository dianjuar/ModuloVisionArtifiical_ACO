#ifndef CONFIG_PARTIRNCUADROS_H
#define CONFIG_PARTIRNCUADROS_H

#include "INCLUDE_opencv.h"

namespace CONFIG
{

class partirNcuadros
{
    int n;
public:
    partirNcuadros(int n);
    void calibrar(int n);

    void set_n(int n){ this->n = n;}
    int get_n(){ return n; }
};

}
#endif // CONFIG_PARTIRNCUADROS_H
