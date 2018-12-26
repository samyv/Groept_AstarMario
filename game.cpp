#include "game.h"
#include "math.h"
#include "tview.h"
#include "gview.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <QSound>

using namespace std;
Game::Game(Gview * gview)
{ 
    world = new World();
    tiles = world->createWorld(":/worldmap4.png");

    gview->show();
    background = new QMediaPlayer();
    background->setMedia(QUrl("qrc:/sound/backgroundmusic.mp3"));
    background->setVolume(50);
    background->play();
    player = new QMediaPlayer();
    QObject::connect(gview->getMariopix(),SIGNAL(marioMoved(int,int)),this,SLOT(checkTile(int,int)));
    QObject::connect(this,SIGNAL(changeMario(int,int)),gview,SLOT(updateProtagonist(int,int)));

    ////    Tview * tview = new Tview(move(greyTiles), move(enemies), move(protagonist), world->getCols(), world->getRows());
    protagonist = world->getProtagonist();
    enemies = world->getEnemies(enemiesCount);
    healthpacks = world->getHealthPacks(healthpackCount);
    copyEnemies();
    gview->initDisplay(enemies,healthpacks);
    makeModel();
    //CONNECT TIMOUT OF MAIN TIMER TO STEP FUNCTION
    timer = new QTimer(gview);
    if(ai){
        QObject::connect(timer,SIGNAL(timeout()),this,SLOT(step()));

    } else {
        cout << "user is playing " << endl;
        QObject::connect(timer,SIGNAL(timeout()),this,SLOT(stepUser()));
        timer->start(0);
    }

    //CONNECTIONS WHEN PROTAGONIST DATA CHANGES
    QObject::connect(protagonist.get(),SIGNAL(posChanged(int,int)), gview,SLOT(updateProtagonist(int, int)));
    QObject::connect(protagonist.get(),SIGNAL(healthChanged(int)), gview,SLOT(changeHealthbar(int)));

    //CONNECTIONS WHEN SPECIAL TILES DATA CHANGES
    for(unique_ptr<Enemy> & a: enemies){
        if(typeid (*a) == typeid (Enemy)){
            connect(a.get(), SIGNAL(dead()), gview, SLOT(enemyDead()));
            cout << typeid (*a).name() << endl;

        } else if(typeid (*a) == typeid (PEnemy)){
            connect(a.get(), SIGNAL(dead()), gview, SLOT(penemyDead()));
            cout << typeid (*a).name() << endl;
        }

    }
    connect(gview, SIGNAL(poisonExplosion(qreal,qreal)), this, SLOT(setNeighboursPoison(qreal,qreal)));
    connect(this, SIGNAL(poisonedTile(qreal,qreal)), gview, SLOT(drawPoisoned(qreal,qreal)));

    QObject::connect(this,SIGNAL(healthpackGained(Tile *)), gview,SLOT(triggerHealthpack(Tile *)));

    QObject::connect(this,SIGNAL(sendSound(QString)), this,SLOT(playSound(QString)));
    QObject::connect(gview,SIGNAL(gameStart()), m,SLOT(startGame()));
    connect(gview, SIGNAL(changeweight(int,double)), m, SLOT(weightchanged(int,double)));
    QObject::connect(m,SIGNAL(newBest(vector<tile_t*>)), gview,SLOT(drawCurrentBest(vector<tile_t*>)));
    QObject::connect(gview,SIGNAL(geneticTrigger()), m,SLOT(dotheSalesmanG()));
    QObject::connect(this,SIGNAL(checkCollision()), gview,SLOT(collisonDetect()));
    QObject::connect(gview,SIGNAL(enemyDeadUser(int,int)),this,SLOT(userEnemyDefeated(int,int)));
    QObject::connect(gview,SIGNAL(hpUser(int,int)),this,SLOT(hpTrigger(int,int)));

    connect(m, SIGNAL(salesmanDone()), this, SLOT(startTime()));
}

void Game::step(){
    if(!path.empty()){
        Tile * nextTile = path.back()->t;
        protagonist->setPos(nextTile->getXPos(),nextTile->getYPos());
        //CHECK COLLISIONS WITH ENEMIES
        for(auto &enemy : enemies){
            if((protagonist->getXPos() == enemy->getXPos()) && (protagonist->getYPos() == enemy->getYPos())){
                protagonist->setHealth(protagonist->getHealth()-enemy->getValue());
                enemy->setDefeated(true);
                //Erase–remove idiom
                enemies.erase(remove(enemies.begin(),enemies.end(),enemy),enemies.end());
                emit sendSound("qrc:/sound/smw_kick.wav");
                break;
            }
        }

        //CHECK COLLISIONS WITH HEALTHPACKS
        for(auto &hp : healthpacks){
            if((protagonist->getXPos() == hp->getXPos()) && (protagonist->getYPos() == hp->getYPos())){
                protagonist->setHealth(protagonist->getHealth()+hp->getValue());
                emit healthpackGained(hp.get());
                emit sendSound("qrc:/sound/smw_1-up.wav");
                break;
            }
        }
        if(enemies.empty()){
            cout << "you won" << endl;
            background->stop();
            emit sendSound("qrc:/sound/smw_castle_clear.wav");
        }
        path.pop_back();
    } else{
        //cout << "PATH EMPTY" << endl;
    }
}

void Game::userEnemyDefeated(int x, int y)
{
    for(auto &enemy : enemies){
        if((x == enemy->getXPos()) && (y == enemy->getYPos())){
            protagonist->setHealth(protagonist->getHealth()-enemy->getValue());
            enemy->setDefeated(true);
            //Erase–remove idiom
            enemies.erase(remove(enemies.begin(),enemies.end(),enemy),enemies.end());
            emit sendSound("qrc:/sound/smw_kick.wav");
            break;
        }
    }
}

void Game::hpTrigger(int x, int y)
{
    for(auto &hp : healthpacks){
        if((x == hp->getXPos()) && (y == hp->getYPos())){
            protagonist->setHealth(protagonist->getHealth()+hp->getValue());
            emit healthpackGained(hp.get());
            //Erase–remove idiom
            healthpacks.erase(remove(healthpacks.begin(),healthpacks.end(),hp),healthpacks.end());
            emit sendSound("qrc:/sound/smw_1-up.wav");
            break;
        }
    }
}



void Game::stepUser(){
    emit checkCollision();
}



void Game::makeModel(){
    m = new Model(enemiesToDefeat, healthpacksOver, enemiesCount, generationsAmount, protagonist.get(), &path);
    m->makeMap(tiles, world->getRows(), world->getCols());
}


void Game::playSound(QString file){
    player->setMedia(QUrl(file));
    player->setVolume(100);
    player->play();
}



void Game::copyEnemies(){
    for(unsigned i = 0; i<enemies.size();i++){
        enemiesToDefeat.push_back(enemies.at(i).get());
    }
    
    for(unsigned i = 0; i<healthpacks.size();i++){
        healthpacksOver.push_back(healthpacks.at(i).get());
    }
}



Game::~Game(){
    /*for(tile_t * t: map){
        delete t;
    }*/
    delete background;
    delete player;
}





void Game::setNeighboursPoison(qreal x, qreal y){
    //    Tile * base;
    //    cout << "x: " << x << "y: " << y<< endl;
    //    for(auto &t : tiles){
    //        if(t->getXPos() == x && t->getYPos()== y){

    //            base = t.get();
    //            break;
    //        }
    //    }
    //    cout << base->getXPos() << " " << base->getYPos() << endl;
}

void Game::startTime(){
    timer->start(4);
}

void Game::checkTile(int x, int y)
{
    Tile * t = tiles.at(uint(protagonist->getXPos()+x+(protagonist->getYPos()+y)*world->getCols())).get();
    cout << t->getValue() << endl;
    if(t->getValue() != INFINITY & t->getValue() != 0.0f){
        cout << "set x: "<<x<<" y: "<<y<< endl;
        protagonist->setPos(protagonist->getXPos()+x,protagonist->getYPos()+y);
    }
}



void Game::eventFilter(QKeyEvent *e)
{
    cout << "NIET gay" << endl;
    if(e->key() == Qt::ControlModifier){
        cout << "gay" << endl;
    }
}
