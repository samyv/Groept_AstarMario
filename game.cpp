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
    tiles = world->createWorld(":/worldmap4.png");
    makeModel();
    Tview * tview = new Tview(move(tiles), move(healthPacks), move(enemies), world->getCols(), world->getRows());
    //gview->show();
    //generateWorld();
    ////    Tview * tview = new Tview(move(greyTiles), move(enemies), move(protagonist), world->getCols(), world->getRows());
    protagonist = world->getProtagonist();
    tview->updateProtagonist(protagonist->getXPos(), protagonist->getYPos());
    /*QObject::connect(protagonist.get(),SIGNAL(posChanged(int,int)), gview,SLOT(updateProtagonist(int, int)));
    QObject::connect(protagonist.get(),SIGNAL(posChanged(int,int)), tview,SLOT(updateProtagonist(int, int)));
    QTimer * timer = new QTimer(gview);
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(step()));
    timer->start(20); */
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
    enemies = world->getEnemies(15);

    healthPacks = world->getHealthPacks(2);
}
int i = 0;
void Game::step(){
    if(!path.empty()){
        tile_t * nextTile = path.back();

        protagonist->setPos(nextTile->t->getXPos(),nextTile->t->getYPos());
        path.pop_back();
    }
}









