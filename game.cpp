#include "game.h"
#include "math.h"
#include "tview.h"
#include <iostream>

using namespace std;
game::game()
{
    tiles = world->createWorld(":/worldmap4.png");
    generateWorld();
//    Tview * tview = new Tview(move(greyTiles), move(enemies), move(protagonist), world->getCols(), world->getRows());
}

void game::generateWorld(){
    //generate enemies
    for(unsigned long i =0; i <tiles.size();i++){
        if(tiles.at(i)->getValue() != INFINITY && tiles.at(i)->getValue() < 1.0f){
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




