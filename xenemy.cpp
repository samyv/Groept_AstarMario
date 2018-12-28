#include "xenemy.h"

XEnemy::XEnemy(int lives):Enemy (0,0,0)
{
    this->lives = lives;

}

int XEnemy::getLives() const
{
    return lives;
}

void XEnemy::setLives(int value)
{
    lives = value;
    if(lives > 0){
        emit livesChanged();
    } else {
        emit dead();
    }
}

void XEnemy::showRandomPosition(int x, int y)
{


}

void XEnemy::newPosition()
{


}
