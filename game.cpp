#include "game.h"
#include "math.h"
#include "tview.h"
#include "gview.h"
#include <iostream>

using namespace std;
Game::Game()
{
    world = new World();
    tiles = world->createWorld(":/maze1.png");
    //generateWorld();
////    Tview * tview = new Tview(move(greyTiles), move(enemies), move(protagonist), world->getCols(), world->getRows());
    Gview gview;
    //gview.show();
    protagonist = unique_ptr<Protagonist>(new Protagonist);

    QObject::connect(protagonist.get(),SIGNAL(posChanged(int,int)),&gview,SLOT(updateProtagonist(int, int)));

    protagonist->setPos(3,5);
    cout << protagonist->getXPos() << endl;


}

void Game::generateWorld(){
    //generate enemies
    for(unsigned long i =0; i <tiles.size();i++){
        if(tiles.at(i)->getValue() != INFINITY || tiles.at(i)->getValue() < 1.0f){
            greyTiles.push_back(move(tiles.at(i)));
        }
    }
    for(int i =0; i < enemiesCount;i++){
        int random_i = rand() % greyTiles.size();
        Tile * tile = greyTiles.at(random_i).get();
        unique_ptr<Enemy> e = unique_ptr<Enemy>(new Enemy(tile->getXPos(),tile->getYPos(),defaultStrength));
        enemies.push_back(move(e));
    }

}






