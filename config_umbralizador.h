#ifndef CONFIG_UMBRALIZADOR_H
#define CONFIG_UMBRALIZADOR_H

#include "config_configbase.h"
#include "stand_Tools.h"

namespace CONFIG
{

class umbralizador: public configBase
{
    int umbral;
    Mat blackAndWhite;
    Mat kernel;

public:
    umbralizador(int);

    void calibrar(Mat m);

    Mat get_BlackAndWhite(){ return blackAndWhite; }
    Mat* get_BlackAndWhite_SEGUIMIENTO(){ return &blackAndWhite; }
    int getUmbral(){ return umbral; }    

    void setUmbral(int umbral){ this->umbral = umbral; }
};

}
#endif // CONFIG_UMBRALIZADOR_H
