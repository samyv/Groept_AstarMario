#include "xenemy.h"
#include <iostream>
XEnemy::XEnemy(int lives):Enemy (0,0,0)
{
    this->lives = lives;

}

int XEnemy::getLives() const
{
    return lives;
}

using namespace std;
void XEnemy::setLives(int value)
{
   cout << "set lifes to: " << value << endl;
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

void XEnemy::setPos(int x, int y)
{
    this->setXPos(x);
    this->setYPos(y);
    cout << "pos Bowser set to: " << x << " " << y << endl;
    emit posChanged(x,y);

}

