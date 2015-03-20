#ifndef STAND_CLICKABLELABEL_H
#define STAND_CLICKABLELABEL_H

#include "INCLUDE_QTstuff.h"

namespace STAND
{

class ClickableLabel: public QLabel
{
    Q_OBJECT
public:
    explicit ClickableLabel( QWidget * parent = 0 );
    ~ClickableLabel();
    int x,y;
    void mousePressEvent ( QMouseEvent * event ) ;

signals:
    void clicked();
protected:

};

}

#endif // STAND_CLICKABLELABEL_H
