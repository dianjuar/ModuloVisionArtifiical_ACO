#ifndef CONFIG_UMBRALIZADOR_H
#define CONFIG_UMBRALIZADOR_H

#include "INCLUDE_opencv.h"
#include "stand_Tools.h"

namespace CONFIG
{

class umbralizador
{
    int umbral;
    Mat blackAndWhite;

public:
    umbralizador(int);

    void calibrar(Mat m);
    Mat get_BlackAndWhite(){ return blackAndWhite; }

    int getUmbral(){ return umbral; }    
    void setUmbral(int umbral){ this->umbral = umbral; }
};

}
#endif // CONFIG_UMBRALIZADOR_H
