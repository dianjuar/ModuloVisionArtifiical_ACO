#include "stand_clickablelabel.h"

using namespace STAND;

ClickableLabel::ClickableLabel( QWidget *parent ) :
    QLabel(parent)

{

}

ClickableLabel::~ClickableLabel()
{
}

void ClickableLabel::mousePressEvent ( QMouseEvent * event )
{
    this->x = event->x();
    this->y = event->y();
    qDebug()<<x<<y;
    emit clicked();
}
