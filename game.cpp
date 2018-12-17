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
}

//void Game::generateWorld(){
//    //generate enemies
//    for(unsigned long i =0; i <tiles.size();i++){
//        if(tiles.at(i)->getValue() != INFINITY || tiles.at(i)->getValue() < 1.0f){
//            greyTiles.push_back(move(tiles.at(i)));
//        }
//    }
//    for(int i =0; i < enemiesCount;i++){
//        int random_i = rand() % greyTiles.size();
//        Tile * tile = greyTiles.at(random_i).get();
//        unique_ptr<Enemy> e = unique_ptr<Enemy>(new Enemy(tile->getXPos(),tile->getYPos(),defaultStrength));
//        enemies.push_back(move(e));
//    }
//}

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

    //STILL ENEMIES LEFT
    while(!enemiesToDefeat.empty()){
        //FIND THE CLOSEST ENEMY
        unsigned int indexToDelete = findClosestEnemy(x,y);
        closest = enemiesToDefeat.at(indexToDelete);
        //CAN WE DEAFEAT THE ENEMY WITHOUT DYING?
        if(enoughHealth(calc_health,closest->getValue())){
            //---------YES => calculate remaining health----------------
            calc_health -= closest->getValue();

            //PUSH closest enemy in the list to defeat
            enemiesInOrder.push_back(closest);

            //erase from list
            enemiesToDefeat.erase(remove(enemiesToDefeat.begin(), enemiesToDefeat.end(),enemiesToDefeat.at(indexToDelete)), enemiesToDefeat.end());

            //SET THE GOAL COORDINATES
            x = closest->getXPos();
            y = closest->getYPos();

            //FIND PATH BETWEEN STARTPOS EN GOAL
            vector<tile_t *> local_path = m->aStar(map.at(start->getXPos() + start->getYPos()*world->getCols()),map.at(x+y*world->getCols()),map);
            m->resetMap(map);
            //SET GLOBAL PATH = LOCAL PATH (NO FUCKING CLUE WHY THIS WORKS..)
            path.insert(path.begin(),local_path.begin(),local_path.end());

            //WHEN PATH IS FOUND WE SET THE NEW STARTPOSITION = LAST GOAL
            start = closest;

            //WE CLEAR LOCAL PATH
            //            vector<tile_t *>().swap(local_path);
            //---------NO => Search healthpack----------------\\

            //CHECK IF THERE ARE HEALTHPACKS LEFT
        } else if (!healthpacksOver.empty()) {
            //YES => find CLOSEST HEALTHPACK
            unsigned int closest_index = findClosestHealtpack(x,y);
            Tile * closest_hp = healthpacksOver.at(closest_index);
            healtpacksInOrder.push_back(closest_hp);

            //erase from list
            healthpacksOver.erase(remove(healthpacksOver.begin(), healthpacksOver.end(),healthpacksOver.at(closest_index)), healthpacksOver.end());

            calc_health = 100;
            //SET THE GOAL COORDINATES
            x = closest_hp->getXPos();
            y = closest_hp->getYPos();

            //FIND PATH BETWEEN STARTPOS EN GOAL
            vector<tile_t *> local_path_hp = m->aStar(map.at(start->getXPos() + start->getYPos()*world->getCols()),map.at(x+y*world->getCols()),map);
            m->resetMap(map);
            path.insert(path.begin(),local_path_hp.begin(),local_path_hp.end());

            //SET  NEW STARTPOSITION = LAST GOAL
            start = closest_hp;
        } else if(healthpacksOver.empty() && calc_health > 0){
            calc_health -= closest->getValue();

            //PUSH closest enemy in the list to defeat
            enemiesInOrder.push_back(closest);

            //erase from list
            enemiesToDefeat.erase(remove(enemiesToDefeat.begin(), enemiesToDefeat.end(),enemiesToDefeat.at(indexToDelete)), enemiesToDefeat.end());

            //SET THE GOAL COORDINATES
            x = closest->getXPos();
            y = closest->getYPos();

            //FIND PATH BETWEEN STARTPOS EN GOAL
            vector<tile_t *> local_path = m->aStar(map.at(start->getXPos() + start->getYPos()*world->getCols()),map.at(x+y*world->getCols()),map);
            m->resetMap(map);
            //SET GLOBAL PATH = LOCAL PATH (NO FUCKING CLUE WHY THIS WORKS..)
            path.insert(path.begin(),local_path.begin(),local_path.end());

            //WHEN PATH IS FOUND WE SET THE NEW STARTPOSITION = LAST GOAL
            start = closest;
            break;
        }
        cout << "calc health: " << calc_health << endl;
    }
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





