#include "game.h"
#include "math.h"
#include "tview.h"
#include "gview.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>

using namespace std;
Game::Game(Gview * gview)
{
    world = new World();
    tiles = world->createWorld(":/worldmap4.png");
    makeModel();
    gview->show();
    //generateWorld();
    background = new QMediaPlayer();
    background->setMedia(QUrl::fromLocalFile("/home/samy/Qt_Projects/Media_processing_pathfinding/backgroundmusic.mp3"));
    background->setVolume(50);
    background->play();
    player = new QMediaPlayer();
    ////    Tview * tview = new Tview(move(greyTiles), move(enemies), move(protagonist), world->getCols(), world->getRows());
    protagonist = world->getProtagonist();
    start = protagonist.get();
    enemies = world->getEnemies(enemiesCount);
    healthpacks = world->getHealthPacks(healthpackCount);
    copyEnemies();
    cout << "healthpacks size: " << healthpacks.size() << endl;
    gview->initDisplay(enemies,healthpacks);
    QTimer * timer = new QTimer(gview);
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(step()));
    QObject::connect(protagonist.get(),SIGNAL(posChanged(int,int)), gview,SLOT(updateProtagonist(int, int)));
    QObject::connect(this,SIGNAL(enemyDefeated(float,Enemy *)), gview,SLOT(explodeEnemy(float,Enemy *)));
    QObject::connect(this,SIGNAL(healthpackGained(float,Tile *)), gview,SLOT(triggerHealthpack(float,Tile *)));
    QObject::connect(this,SIGNAL(sendSound(QString)), this,SLOT(playSound(QString)));
    QObject::connect(gview,SIGNAL(gameStart()), this,SLOT(startGame()));
    timer->start(10);
    distanceBetweenEnemies = calculateDistances();
    enemiesInOrderIndexes = dotheSalesman();
    
}

vector<vector<double>> Game::calculateDistances(){
    vector<vector<double>> localArray(enemiesCount + 1, vector<double>(enemiesCount + 1, 0));
    for(unsigned long i = 0; i<enemiesCount + 1;i++){
        for(unsigned long j = 0; j<enemiesCount + 1;j++){
            if(j == i){
                localArray.at(i).at(j) = 0;
            } else if(i==0 && j != 0){
                vector<tile_t *> pathBetweenEnemies = m->aStar(map.at(protagonist->getXPos() + protagonist->getYPos()*world->getCols()),map.at(enemiesToDefeat.at(j-1)->getXPos()+enemiesToDefeat.at(j-1)->getYPos()*world->getCols()),map);
                m->resetMap(map);
                localArray.at(i).at(j) = double(pathBetweenEnemies.size());
                localArray.at(j).at(i) = double(pathBetweenEnemies.size());
            } else if(i != 0 && j != 0 && j > i){
                vector<tile_t *> pathBetweenEnemies = m->aStar(map.at(enemiesToDefeat.at(i-1)->getXPos() + enemiesToDefeat.at(i-1)->getYPos()*world->getCols()),map.at(enemiesToDefeat.at(j-1)->getXPos()+enemiesToDefeat.at(j-1)->getYPos()*world->getCols()),map);
                m->resetMap(map);
                localArray.at(i).at(j) = double(pathBetweenEnemies.size());
                localArray.at(j).at(i) = double(pathBetweenEnemies.size());
            }
        }
    }
    return localArray;
}
vector<int> Game::dotheSalesman(){
    vector<int> order(enemiesCount + 1, 0);
    vector<int> bestOrder(enemiesCount + 1, 0);
    double bestD = INFINITY;
    bool flag = false;
    for(int i = 0; i< int(enemiesToDefeat.size() + 1);i++){
        order.at(i) = i;
    }
    while(1){
        double d = 0;
        //print to check
        for(int i = 0; i < enemiesCount; i++){
            cout << order[i] << "-->";
        }
        cout << order[enemiesCount] << endl;
        for(int i = 0; i<enemiesToDefeat.size();i++){
            cout << order[i] << ", " << order[i+1] << endl;
            d += distanceBetweenEnemies[order[i]][order[i+1]];
        }
        if(d<bestD){
            bestOrder = order;
            bestD = d;
        }
        cout << "sum: " << d << endl;
        //


        int x,y;
        // 0,5,1,7,6,3,9,8,4,2
        //FIND THE BIGGEST X SO THAT order[x] < order[x+1]
        x = -1;
        for(int i = 1;i<enemiesCount;i++){
            if(order[i] < order[i + 1]){
                x = i;
            }
        }
        if(x == -1){
            cout << "BREAK" << endl;
            break;
        }

        for(int i = 1;i>1;i++){
            if(order[x] < order[i]){
                y = i;
            }
        }

        int swap = order[x];
        order[x] = order[y];
        order[y] = swap;

        //reverse
        int reversed[enemiesCount + 1];

        //dont reverse everything before x
        for(int i = 0; i <= x; i++){
            reversed[i] = order[i];
        }

        for(int i = x + 1, j = enemiesCount; i < enemiesCount; i++, j--){
            reversed[j] = order[i];
        }

        //copy reversed to order
        for(int i = 0; i < enemiesCount +1; i++){
            order[i] = reversed[i];
        }



    }

    return bestOrder;
}

void Game::makeModel(){

    m = new Model();
    map = m->makeMap(tiles, world->getRows(), world->getCols());
    //path = m->aStar(map.at(45*world->getCols()), map.at(999 + 937*world->getCols()), map);
    //    int count = 0;
    //    QBrush brush(Qt::SolidPattern);
    //    QPen pen(Qt::NoPen);
    //    brush.setColor(QColor(255,0,0));
    //    pen.setColor(QColor(255,0,0));
    //    for(auto & tile: path){
    //        //shared_ptr<QGraphicsRectItem> rect = make_shared<QGraphicsRectItem>(tile->t->getXPos()*displaySize,tile->t->getYPos()*displaySize, displaySize,displaySize,nullptr);
    //        //rpath.empty()ects.push_back(rect);
    //        scene->addRect(tile->t->getXPos()*displaySize, tile->t->getYPos()*displaySize, displaySize, displaySize,pen,brush);
    //        //cout << tile->t->getXPos()*displaySize << " " << tile->t->getYPos()*displaySize << endl;
    //        count++;
    //    }healthpacksOver
    //    cout << count<< endl;
}
void Game::step(){
    if(!path.empty()){
        tile_t * nextTile = path.back();
        protagonist->setPos(nextTile->t->getXPos(),nextTile->t->getYPos());
        //CHECK IF COLLISION WITH ENEMY
        if((protagonist->getXPos() == enemiesInOrder.front()->getXPos()) && (protagonist->getYPos() == enemiesInOrder.front()->getYPos())){
            cout << "ENEMY COLLISION xpos:" << protagonist->getXPos() << endl;
            cout << "health before set : " << protagonist->getHealth() << endl;
            // protagonist->setHealth(protagonist->getHealth()-enemiesInOrder.front()->getValue());
            cout << "health after emit : " << protagonist->getHealth() << endl;
            emit enemyDefeated(protagonist->getHealth(),enemiesInOrder.front());
            cout << "health prota: " << protagonist->getHealth() << endl;
            emit sendSound("/home/samy/Qt_Projects/Media_processing_pathfinding/smw_kick.wav");
            enemiesInOrder.erase(enemiesInOrder.begin());
        }        //CHECK IF COLLISION WITH MUSHROOM
        else if((protagonist->getXPos() == healtpacksInOrder.front()->getXPos()) && (protagonist->getYPos() == healtpacksInOrder.front()->getYPos())){
            cout << "HP COLLISION" << endl;
            protagonist->setHealth(100);
            emit healthpackGained(protagonist->getHealth(),healtpacksInOrder.front());
            //protagonist->setHealth(protagonist->getHealth()+healtpacksInOrder.front()->getValue());
            //cout << "health prota: " << protagonist->getHealth() << endl;
            emit sendSound("/home/samy/Qt_Projects/Media_processing_pathfinding/smw_1-up.wav");
            if(healtpacksInOrder.size()>0){
                healtpacksInOrder.erase(healtpacksInOrder.begin());
            }
        }
        //CHECK IF GAME STATE HAS CHANGED
        if(enemiesToDefeat.empty() && enemiesInOrder.empty()){
            background->stop();
            emit sendSound("/home/samy/Qt_Projects/Media_processing_pathfinding/smw_castle_clear.wav");
        } else if(enemiesInOrder.empty() && !enemiesToDefeat.empty()) {
            background->stop();
            emit sendSound("/home/samy/Qt_Projects/Media_processing_pathfinding/game_over.wav");
        }

        path.pop_back();
    }
}

void Game::playSound(QString file){
    player->setMedia(QUrl::fromLocalFile(file));
    player->setVolume(100);
    player->play();
}

void Game::startGame(){
    Enemy * closest;
    int x = protagonist->getXPos();
    int y = protagonist->getYPos();
    float calc_health = 100.0f;
    //STILL ENEMIES LEFT/**
    //    while(!enemiesToDefeat.empty()){
    //        //FIND THE CLOSEST ENEMY
    //        unsigned int indexToDelete = findClosestEnemy(x,y);
    //        closest = enemiesToDefeat.at(indexToDelete);
    //        //CAN WE DEAFEAT THE ENEMY WITHOUT DYING?
    //        if(enoughHealth(calc_health,closest->getValue())){
    //            //---------YES => calculate remaining health----------------
    //            calc_health -= closest->getValue();

    //            //PUSH closest enemy in the list to defeat
    //            enemiesInOrder.push_back(closest);

    //            //erase from list
    //            enemiesToDefeat.erase(remove(enemiesToDefeat.begin(), enemiesToDefeat.end(),enemiesToDefeat.at(indexToDelete)), enemiesToDefeat.end());

    //            //SET THE GOAL COORDINATES
    //            x = closest->getXPos();
    //            y = closest->getYPos();

    //            //FIND PATH BETWEEN STARTPOS EN GOAL
    //            vector<tile_t *> local_path = m->aStar(map.at(start->getXPos() + start->getYPos()*world->getCols()),map.at(x+y*world->getCols()),map);
    //            m->resetMap(map);
    //            //SET GLOBAL PATH = LOCAL PATH (NO FUCKING CLUE WHY THIS WORKS..)
    //            path.insert(path.begin(),local_path.begin(),local_path.end());

    //            //WHEN PATH IS FOUND WE SET THE NEW STARTPOSITION = LAST GOAL
    //            start = closest;

    //            //WE CLEAR LOCAL PATH
    //            //            vector<tile_t *>().swap(local_path);
    //            //---------NO => Search healthpack----------------\\

    //            //CHECK IF THERE ARE HEALTHPACKS LEFT
    //        } else if (!healthpacksOver.empty()) {
    //            //YES => find CLOSEST HEALTHPACK
    //            unsigned int closest_index = findClosestHealtpack(x,y);
    //            Tile * closest_hp = healthpacksOver.at(closest_index);
    //            healtpacksInOrder.push_back(closest_hp);

    //            //erase from list
    //            healthpacksOver.erase(remove(healthpacksOver.begin(), healthpacksOver.end(),healthpacksOver.at(closest_index)), healthpacksOver.end());

    //            calc_health = 100;
    //            //SET THE GOAL COORDINATES
    //            x = closest_hp->getXPos();
    //            y = closest_hp->getYPos();

    //            //FIND PATH BETWEEN STARTPOS EN GOAL
    //            vector<tile_t *> local_path_hp = m->aStar(map.at(start->getXPos() + start->getYPos()*world->getCols()),map.at(x+y*world->getCols()),map);
    //            m->resetMap(map);
    //            path.insert(path.begin(),local_path_hp.begin(),local_path_hp.end());

    //            //SET  NEW STARTPOSITION = LAST GOAL
    //            start = closest_hp;
    //        } else if(healthpacksOver.empty() && calc_health > 0){
    //            calc_health -= closest->getValue();

    //            //PUSH closest enemy in the list to defeat
    //            enemiesInOrder.push_back(closest);

    //            //erase from list
    //            enemiesToDefeat.erase(remove(enemiesToDefeat.begin(), enemiesToDefeat.end(),enemiesToDefeat.at(indexToDelete)), enemiesToDefeat.end());

    //            //SET THE GOAL COORDINATES
    //            x = closest->getXPos();
    //            y = closest->getYPos();

    //            //FIND PATH BETWEEN STARTPOS EN GOAL
    //            vector<tile_t *> local_path = m->aStar(map.at(start->getXPos() + start->getYPos()*world->getCols()),map.at(x+y*world->getCols()),map);
    //            m->resetMap(map);
    //            //SET GLOBAL PATH = LOCAL PATH (NO FUCKING CLUE WHY THIS WORKS..)
    //            path.insert(path.begin(),local_path.begin(),local_path.end());

    //            //WHEN PATH IS FOUND WE SET THE NEW STARTPOSITION = LAST GOAL
    //            start = closest;
    //            break;
    //        }
    //        cout << "calc health: " << calc_health << endl;
    //    }
    ////*/
}

bool Game::enoughHealth(float curr_health,float strength){
    return (curr_health - strength > 0);
}

unsigned int Game::findClosestEnemy(int start_x,int start_y){
    unsigned int index = 0;
    double minD = double(INFINITY);
    for(unsigned int i = 0; i<enemiesToDefeat.size();i++){
        double d = calculateDistance(start_x,start_y,enemiesToDefeat.at(i));
        if (d<minD){
            minD = d;
            index = i;
        }
    }
    return index;
}
unsigned int Game::findClosestHealtpack(int start_x,int start_y){
    unsigned int index = 0;
    double minD = double(INFINITY);
    for(unsigned int i = 0; i<healthpacksOver.size();i++){
        double d = calculateDistance(start_x,start_y,healthpacksOver.at(i));
        if (d<minD){
            minD = d;
            index = i;
        }
    }
    return index;
}

void Game::copyEnemies(){
    for(unsigned i = 0; i<enemies.size();i++){
        enemiesToDefeat.push_back(enemies.at(i).get());
    }

    for(unsigned i = 0; i<healthpacks.size();i++){
        healthpacksOver.push_back(healthpacks.at(i).get());
    }
    cout << "healthpacksOver size: " << healthpacksOver.size() << endl;
}

double Game::calculateDistance(int x, int y,Tile * enemy){
    double dx = abs(x - enemy->getXPos());
    double dy = abs(y - enemy->getYPos());
    double dist;
    if(dx < dy){
        dist = ((dy-dx) + (SQRT * dx));
    } else {
        dist = ((dx-dy) + (SQRT * dy));
    }

    return dist/1.01;
}





