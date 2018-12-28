#ifndef PROTAGONISTUSER_H
#define PROTAGONISTUSER_H

#include <QGraphicsItem>

class ProtagonistUser : public QObject,public QGraphicsPixmapItem{
     Q_OBJECT
public:
    explicit ProtagonistUser(QPixmap qp);
    int stepsize = 10;
signals:
    void marioMoved(int,int);
protected:
    void keyPressEvent(QKeyEvent * ev);
};

#endif // PROTAGONISTUSER_H
