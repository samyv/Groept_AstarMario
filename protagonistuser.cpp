#include "protagonistuser.h"
#include<QKeyEvent>

ProtagonistUser::ProtagonistUser(QPixmap qp)
    : QGraphicsPixmapItem (qp) {

}

void ProtagonistUser::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()){
    case Qt::Key_Left:
        setPos(x()-stepsize,y());
        break;
    case Qt::Key_Right:
        setPos(x()+stepsize,y());
        break;
    case Qt::Key_Up:
        setPos(x(),y()-stepsize);
        break;
    case Qt::Key_Down:
        setPos(x(),y()+stepsize);
        break;
    }
}
