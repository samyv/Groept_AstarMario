#ifndef PROTAGONISTUSER_H
#define PROTAGONISTUSER_H

#include <QGraphicsItem>

class ProtagonistUser : public QGraphicsPixmapItem{
public:
    ProtagonistUser(QPixmap qp);
    int stepsize = 5;
protected:
    void keyPressEvent(QKeyEvent * ev);
};

#endif // PROTAGONISTUSER_H
