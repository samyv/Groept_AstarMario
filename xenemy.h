#ifndef XENEMY_H
#define XENEMY_H

#include <QObject>
#include <world_global.h>
#include <world.h>

class XEnemy : public Enemy
{
    Q_OBJECT
public:
    XEnemy(int);

    int getLives() const;
    void setLives(int value);

public slots:
    void showRandomPosition(int x, int y);

signals:
   void showBowser(int x,int y);
   void livesChanged();
private:
    int lives = 5;
    void newPosition();
};
#endif // XENEMY_H
