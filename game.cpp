#include "game.h"
#include "math.h"
#include "tview.h"
#include "gview.h"
#include <iostream>
#include <unistd.h>

using namespace std;
Game::Game(Gview * gview)
{
    world = new World();
    tiles = world->createWorld(":/maze1.png");
    makeModel();
    gview->show();
    //generateWorld();
    ////    Tview * tview = new Tview(move(greyTiles), move(enemies), move(protagonist), world->getCols(), world->getRows());
    protagonist = world->getProtagonist();
    QObject::connect(protagonist.get(),SIGNAL(posChanged(int,int)), gview,SLOT(updateProtagonist(int, int)));
    QTimer * timer = new QTimer(gview);
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(step()));
    timer->start(20);
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

void Game::makeModel(){

    Model * m = new Model();
    map = m->makeMap(tiles, world->getRows(), world->getCols());
    cout << "success " << map.size() << endl;
    path = m->aStar(map.at(5 + 6*world->getCols()), map.at(1 + 492*world->getCols()), map);
    //    int count = 0;
    //    QBrush brush(Qt::SolidPattern);
    //    QPen pen(Qt::NoPen);
    //    brush.setColor(QColor(255,0,0));
    //    pen.setColor(QColor(255,0,0));
    //    for(auto & tile: path){
    //        //shared_ptr<QGraphicsRectItem> rect = make_shared<QGraphicsRectItem>(tile->t->getXPos()*displaySize,tile->t->getYPos()*displaySize, displaySize,displaySize,nullptr);
    //        //rects.push_back(rect);
    //        scene->addRect(tile->t->getXPos()*displaySize, tile->t->getYPos()*displaySize, displaySize, displaySize,pen,brush);
    //        //cout << tile->t->getXPos()*displaySize << " " << tile->t->getYPos()*displaySize << endl;
    //        count++;
    //    }
    //    cout << count<< endl;
}
int i = 0;
void Game::step(){
    if(!path.empty()){
        tile_t * nextTile = path.back();

        protagonist->setPos(nextTile->t->getXPos(),nextTile->t->getYPos());
        path.pop_back();
    }
}









