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
    ////    Tview * tview = new Tview(move(greyTiles), move(enemies), move(protagonist), world->getCols(), world->getRows());
    protagonist = world->getProtagonist();
    enemies = world->getEnemies(enemiesCount);
    healthpacks = world->getHealthPacks(healthpackCount);

    Tview * tview = new Tview();
    tview->setup(tiles, healthpacks, enemies, world->getCols(), world->getRows());
    tview->updateProtagonist(protagonist->getXPos(), protagonist->getYPos());
    for(int i = 0; i < healthpackCount; i++){
        //        healthpacks.at(i)->setValue(30);
    }
    copyEnemies();
    gview->initDisplay(tiles,enemies,healthpacks);
    makeModel();

    bowser = new XEnemy(5);
    QObject::connect(bowser,SIGNAL(livesChanged()),m,SLOT(findBowserPos()));
    QObject::connect(m,SIGNAL(BowerPosFound(Tile *)),this,SLOT(setBowser(Tile *)));
    QObject::connect(bowser,SIGNAL(posChanged(int,int)),gview,SLOT(displayBowser(int,int)));
    QObject::connect(bowser,SIGNAL(posChanged(int,int)),tview,SLOT(displayBowser(int,int)));
    QObject::connect(bowser,SIGNAL(posChanged(int,int)),m,SLOT(findBowser(int,int)));

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
    QObject::connect(protagonist.get(),SIGNAL(posChanged(int,int)), tview,SLOT(updateProtagonist(int, int)));
    QObject::connect(protagonist.get(),SIGNAL(healthChanged(int)), gview,SLOT(changeHealthbar(int)));

    QObject::connect(protagonist.get(),SIGNAL(healthChanged(int)), tview,SLOT(changeHealthbar(int)));
    QObject::connect(m,SIGNAL(startGameTimer()),this,SLOT(gametimer()));

    //CONNECTIONS WHEN SPECIAL TILES DATA CHANGES
    for(unique_ptr<Enemy> & a: enemies){
        if(typeid (*a) == typeid (Enemy)){
            connect(a.get(), SIGNAL(dead()), gview, SLOT(enemyDead()));
        } else if(typeid (*a) == typeid (PEnemy)){
            connect(a.get(), SIGNAL(dead()), gview, SLOT(penemyDead()));
            connect(a.get(), SIGNAL(poisonLevelUpdated(int)), m,SLOT(setPoisonedTiles(int)));
        }
    }

    connect(this, SIGNAL(PEnemyTrigger(int,int)), gview, SLOT(explodeEnemy(int,int)));
    connect(gview, SIGNAL(poisonExplosion(qreal,qreal)), this, SLOT(setNeighboursPoison(qreal,qreal)));
    connect(this, SIGNAL(poisonedTile(qreal,qreal)), gview, SLOT(drawPoisoned(qreal,qreal)));
    connect(m,SIGNAL(setTilesPoisoned(int,int,int)),gview,SLOT(updatePoisonedTiles(int,int,int)));
    connect(m,SIGNAL(setTilesPoisoned(int,int,int)),tview,SLOT(updatePoisonedTiles(int,int,int)));
    QObject::connect(this,SIGNAL(healthpackGained(Tile *)), gview,SLOT(triggerHealthpack(Tile *)));
    QObject::connect(gview,SIGNAL(sendSound(QString)), this,SLOT(playSound(QString)));
    QObject::connect(this,SIGNAL(sendSound(QString)), this,SLOT(playSound(QString)));
    QObject::connect(gview,SIGNAL(gameStart()), m,SLOT(dotheSalesman()));
    QObject::connect(gview,SIGNAL(gamePause()), this,SLOT(pauseTimer()));
    connect(gview, SIGNAL(changeweight(int,double)), m, SLOT(weightchanged(int,double)));
    QObject::connect(m,SIGNAL(newBest(vector<tile_t*>)), gview,SLOT(drawCurrentBest(vector<tile_t*>)));
    QObject::connect(gview,SIGNAL(geneticTrigger()), m,SLOT(dotheSalesmanG()));
    QObject::connect(this,SIGNAL(checkCollision()), gview,SLOT(collisonDetect()));
    QObject::connect(gview,SIGNAL(enemyDeadUser(int,int)),this,SLOT(userEnemyDefeated(int,int)));
    QObject::connect(gview,SIGNAL(hpUser(int,int)),this,SLOT(hpTrigger(int,int)));

    //    QObject::connect(m,SIGNAL(BowerPosFound(Tile *)),this,SLOT(initBowser(Tile *)));



    connect(protagonist.get(), SIGNAL(energyChanged(int)), gview, SLOT(changeEnergybar(int)));
    connect(protagonist.get(), SIGNAL(energyChanged(int)), tview, SLOT(changeEnergybar(int)));
    connect(m, SIGNAL(salesmanDone()), this, SLOT(startTime()));
}

void Game::step(){
    if(protagonist->getEnergy() < 0 || protagonist->getHealth() < 0){
        cout << "HELEMAAL DOOD" << endl;
        timer->stop();
    } else {
        //LEVEND
        if(!path.empty()){
            tile_t * t_t = path.back();
            if(t_t->poison > 0){
//                protagonist->setHealth(protagonist->getHealth() - (t_t->poison/100));
            }
            Tile * nextTile = t_t -> t;
            protagonist->setEnergy(protagonist->getEnergy() - 5 * abs(tiles.at(uint(protagonist->getXPos() + protagonist->getYPos() * world->getCols()))->getValue() - nextTile->getValue()));
            protagonist->setPos(nextTile->getXPos(),nextTile->getYPos());

            //CHECK COLLISIONS WITH ENEMIES
            for(auto &enemy : enemies){
                if((protagonist->getXPos() == enemy->getXPos()) && (protagonist->getYPos() == enemy->getYPos())){
                    protagonist->setHealth(protagonist->getHealth()-enemy->getValue());
                    //                cout << protagonist->getHealth() << endl;
                    cout << "Type Enemy: " ;
                    if(typeid (*enemy) == typeid (Enemy)){
                        enemy->setDefeated(true);
                        cout << "Enemy" << endl;

                    } else if(typeid (*enemy) == typeid (PEnemy)){
                        dynamic_cast<PEnemy*>(enemy.get())->poison();
                        cout << "Poison" << endl;
                    }
                    //Erase–remove idiom
                    enemies.erase(remove(enemies.begin(),enemies.end(),enemy),enemies.end());
                    protagonist->setEnergy(100);
                    //                emit sendSound("qrc:/sound/smw_kick.wav");
                    break;

                }

            }
            //CHECK COLLISIONS WITH HEALTHPACKS
            for(auto &hp : healthpacks){
                if((protagonist->getXPos() == hp->getXPos()) && (protagonist->getYPos() == hp->getYPos())){
                    float h = protagonist->getHealth()+hp->getValue();
                    if(h > 100){
                        h = 100;
                    }
                    protagonist->setHealth(h);;
                    emit healthpackGained(hp.get());
                    //cout <<"HEALTHPACK: " << hp->getValue() << endl;
                    emit sendSound("qrc:/sound/smw_1-up.wav");
                    //ptrdiff_t index = find(healthpacks.begin(),healthpacks.end(),hp) - healthpacks.begin();
                    healthpacksOver.erase(remove(healthpacksOver.begin(),healthpacksOver.end(),hp.get()),healthpacksOver.end());
                    //healthpacksOver.erase(index, healthpacksOver.end());
                    break;
                }
            }
            if(finalGameStarted){
                if((protagonist->getXPos() == bowser->getXPos()) && (protagonist->getYPos() == bowser->getYPos())){
                    if(bowser->getLives() != 1){
                        //cout << "BOWSER HAS BEEN TOUCHED #METOO" << endl;
                        protagonist->setHealth(protagonist->getHealth() - bowser->getValue() * 100);
                        bowser->setLives(bowser->getLives()-1);
                        protagonist->setEnergy(100);
                    } else {
                        cout << "you win' " << endl;
                    }
                }
            }
            path.pop_back();

        } else {
            timer->stop();
            if(!finalGameStarted){
                bowser->setLives(5);
                finalGameStarted = true;
                // enemies.push_back(make_unique<XEnemy>(bowser));
            }
        }
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

void Game::gametimer()
{
    timer->start(0);
}

void Game::setBowser(Tile * t)
{
    bowser->setPos(t->getXPos(),t->getYPos());
    bowser->setValue(t->getValue());
}


void Game::pauseTimer(){
    if(timer->isActive()){
        timer->stop();
    } else {
        timer->start(0);
    }

}


void Game::stepUser(){
    emit checkCollision();
}



void Game::makeModel(){
    m = new Model(enemiesToDefeat, &healthpacksOver, enemiesCount, generationsAmount, protagonist.get(), &path);
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

void Game::here(int h)
{
    cout << "here: " << h << endl;
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
    timer->start(0);
}

void Game::checkTile(int x, int y)
{
    Tile * t = tiles.at(uint(protagonist->getXPos()+x+(protagonist->getYPos()+y)*world->getCols())).get();

    if(t->getValue() != INFINITY & t->getValue() != 0.0f){

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
