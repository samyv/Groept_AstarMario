#include "protagonistuser.h"
#include<QKeyEvent>

ProtagonistUser::ProtagonistUser(QPixmap qp)
    : QGraphicsPixmapItem (qp) {

}

void ProtagonistUser::keyPressEvent(QKeyEvent *event)
{
//    switch(event->key()){
//    case Qt::Key_Left:
//        emit marioMoved(-stepsize,0);
//        break;
//    case Qt::Key_Right:
//        emit marioMoved(stepsize,0);
//        break;
//    case Qt::Key_Up:
//        emit marioMoved(0,-stepsize);
//        break;
//    case Qt::Key_Down:
//        emit marioMoved(0,stepsize);
//        break;
//    }
}
