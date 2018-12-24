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
    makeModel();
    gview->show();
    background = new QMediaPlayer();
    background->setMedia(QUrl("qrc:/sound/backgroundmusic.mp3"));
    background->setVolume(50);
    background->play();
    player = new QMediaPlayer();
    ////    Tview * tview = new Tview(move(greyTiles), move(enemies), move(protagonist), world->getCols(), world->getRows());
    protagonist = world->getProtagonist();
    start = protagonist.get();
    enemies = world->getEnemies(enemiesCount);
    healthpacks = world->getHealthPacks(healthpackCount);
    copyEnemies();
    gview->initDisplay(enemies,healthpacks);
    timer = new QTimer(gview);
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(step()));
    QObject::connect(protagonist.get(),SIGNAL(posChanged(int,int)), gview,SLOT(updateProtagonist(int, int)));
    QObject::connect(this,SIGNAL(enemyDefeated(float,Enemy *)), gview,SLOT(explodeEnemy(float,Enemy *)));
    QObject::connect(this,SIGNAL(healthpackGained(float,Tile *)), gview,SLOT(triggerHealthpack(float,Tile *)));
    QObject::connect(this,SIGNAL(sendSound(QString)), this,SLOT(playSound(QString)));
    QObject::connect(gview,SIGNAL(gameStart()), this,SLOT(startGame()));
    QObject::connect(this,SIGNAL(newBest(vector<tile_t*>)), gview,SLOT(drawCurrentBest(vector<tile_t*>)));
    QObject::connect(gview,SIGNAL(geneticTrigger()), this,SLOT(dotheSalesmanG()));

}

vector<vector<vector<tile_t *>>> Game::calculateDistances(){
    vector<vector<vector<tile_t *>>> localArray(enemiesCount + 1, vector<vector<tile_t *>>(enemiesCount + 1, vector<tile_t *>(0,nullptr)));
    for(unsigned long i = 0; i<enemiesCount + 1;i++){
        for(unsigned long j = 0; j<enemiesCount + 1;j++){
            if(j == i){
                localArray.at(i).at(j) = vector<tile_t *>(0,nullptr);
            } else if(i==0 && j != 0){
                vector<tile_t *> pathBetweenEnemies = calculateDistance(protagonist.get(),enemiesToDefeat.at(j-1));
                localArray.at(i).at(j) = pathBetweenEnemies;
                vector<tile_t *> pathBetweenEnemiesRevert(pathBetweenEnemies.rbegin(),pathBetweenEnemies.rend());
                localArray.at(j).at(i) = pathBetweenEnemiesRevert;
            } else if(i != 0 && j != 0 && j > i){
                vector<tile_t *> pathBetweenEnemies = m->aStar(map.at(enemiesToDefeat.at(i-1)->getXPos() + enemiesToDefeat.at(i-1)->getYPos()*world->getCols()),map.at(enemiesToDefeat.at(j-1)->getXPos()+enemiesToDefeat.at(j-1)->getYPos()*world->getCols()),map);
                m->resetMap(map);
                localArray.at(i).at(j) = pathBetweenEnemies;
                vector<tile_t *> pathBetweenEnemiesRevert(pathBetweenEnemies.rbegin(),pathBetweenEnemies.rend());
                localArray.at(j).at(i) = pathBetweenEnemiesRevert;
            }
        }
    }
    return localArray;
}

void Game::dotheSalesmanG(){
    unsigned long populationSize = 80;
    vector<vector<int>> population(populationSize,vector<int>(enemiesCount+1,0));
    vector<double> fitness(populationSize,0);
    float mutationRate = 0.05;
    srand (time(NULL));
    distanceBetweenEnemies = calculateDistances();
    vector<int> bestOrder(enemiesCount + 1, 0);
    double bestD = double(INFINITY);

    //GENERATE A NEW STARTING POPULATION!
    for(unsigned long j = 0; j < populationSize;j++){
        vector<int> order(enemiesCount + 1, 0);
        //CREATE A NEW ORDER 0123456...
        for(unsigned long i = 0; i< (enemiesToDefeat.size() + 1);i++){
            order.at(i) = i;
        }
        //MAKE IT RANDOM
        for(unsigned long i = 0;i<10;i++){
            unsigned long indexA = 1+ rand() % (order.size()-1);
            //            cout << "indexA: " << indexA ;
            unsigned long indexB = 1+ rand() % (order.size()-1);
            //            cout << " and indexB: " << indexB << endl;
            unsigned long swap = order[indexA];
            order[indexA] = order[indexB];
            order[indexB] = swap;
        }
        population[j] = order;
    }


    for(int g = 0; g<generationsAmount;g++){
        cout << "GENERATE POPULATION: " <<g+1 << endl;
        //CALCULATE TOTAL DISTANCE FOR EVERY MEMBER
        //cout << "CALCULATE TOTAL DISTANCE FOR EVERY MEMBER" << endl;
        for(unsigned long i = 0; i< populationSize;i++){
            vector<int> member = population[i];
            //printElement(member);
            //CALC DISTANCE FOR EVERY MEMBER OF THE POPULATION
            double d = 0;
            for(int i = 0; i<enemiesToDefeat.size();i++){
                d += distanceBetweenEnemies[member[i]][member[i+1]].size();
            }

            if(d < bestD){
                bestD = d;
                bestOrder = member;
                cout << "BEST ONE!" << endl;
                vector<tile_t *> bestOrderPath;
                for(int i = 0; i < enemiesCount;i++){
                    vector<tile_t*> pathBetweenEnemies  = distanceBetweenEnemies[uint(bestOrder[uint(i)])][uint(bestOrder[uint(i+1)])];
                    bestOrderPath.insert(bestOrderPath.begin(),pathBetweenEnemies.begin(),pathBetweenEnemies.end());
                }
                emit newBest(bestOrderPath);
                printElement(bestOrder);
            }
            fitness[i] = 1/(d+1);
        }
        //        cout << "distance done" << endl;
        //NORMALIZE FITNESS
        // cout << "NORMALIZE FITNESS" << endl;
        double sum = 0;
        for(unsigned long i = 0; i< fitness.size();i++){
            sum += fitness[i];
        }
        for(unsigned long i = 0; i< fitness.size();i++){
            fitness[i] = fitness[i] / sum;
        }
        //MAKE NEXT GENERATION!
        vector<vector<int>> newPopulation(populationSize,vector<int>(enemiesCount+1,0));
        for(unsigned long i = 0; i < populationSize;i++){
            //POOLING ALGORITHM TO PICK TWO ACCORDING TO FITNESS LEVEL
            vector<vector<int>> picks(2,vector<int>(enemiesCount+1,0));
            for(int c = 0; c<2;c++){
                unsigned long index = 0;
                double r = ((double) rand() / (RAND_MAX)); //GENERATES RANDOM NUMBER BETWEEN 0 AND 1
                while(r>0){
                    r = r - fitness[index];
                    index++;
                }
                index--;
                picks[c] = population[index];
                //printElement(picks[c]);
            }


            //CROSSOVERRR
            //cout << "CROSSOVER" << endl;
            //DEFINE RANDOM RANGE
            int startA = 1 + (rand() % (picks[0].size()-2));
            int endA = startA + rand() % (picks[0].size()-startA-1);
            int range = endA - startA+1; // +1 because we also need to count first element

            //cout << startA << "->" << endA << endl;

            //copy first dna part
            vector<int> crossOver(range,0);
            for(int n = 0; n <range;n++){
                crossOver.at(n) = picks[0][startA+n];
            }
            for(int c = 1;c<picks[1].size();c++){
                int enemy = picks[1][c];
                vector<int>::iterator it;
                it = find(crossOver.begin(),crossOver.end(),enemy);
                if (it == crossOver.end()) //FOUND NOT IN VECTOR c1
                    crossOver.push_back(enemy);
            }

            vector<int>::iterator it;
            it = crossOver.begin();
            it = crossOver.insert(it,0);

            //MUTATIOOON
            for(unsigned long m = 0; m < crossOver.size();m++){
                if(((double) rand() / (RAND_MAX)) < mutationRate){
                    unsigned long indexA = 1+ rand() % (crossOver.size()-1);
                    unsigned long indexB = 1+ rand() % (crossOver.size()-1);
                    unsigned long swap = crossOver[indexA];
                    crossOver[indexA] = crossOver[indexB];
                    crossOver[indexB] = swap;
                }
            }
            newPopulation[i] = crossOver;
        }
        population = newPopulation;
    }
    printElement(bestOrder);
    for(int i = 0; i < enemiesCount;i++){
        vector<tile_t*> pathBetweenEnemies  = distanceBetweenEnemies[uint(bestOrder[uint(i)])][uint(bestOrder[uint(i+1)])];
        path.insert(path.begin(),pathBetweenEnemies.begin(),pathBetweenEnemies.end());
    }
    timer->start(4);
}

void Game::makeModel(){
    m = new Model();
    map = m->makeMap(tiles, world->getRows(), world->getCols());
}

void Game::step(){
    if(!path.empty()){
        tile_t * nextTile = path.back();
        protagonist->setPos(nextTile->t->getXPos(),nextTile->t->getYPos());
        //CHECK IF COLLISION WITH ENEMY
        if((protagonist->getXPos() == enemiesInOrder.front()->getXPos()) && (protagonist->getYPos() == enemiesInOrder.front()->getYPos())){
            cout << "ENEMY COLLISION xpos:" << protagonist->getXPos() << endl;
            cout << "health before set : " << protagonist->getHealth() << endl;
            protagonist->setHealth(protagonist->getHealth()-enemiesInOrder.front()->getValue());
            cout << "health after emit : " << protagonist->getHealth() << endl;
            emit enemyDefeated(protagonist->getHealth(),enemiesInOrder.front());
            cout << "health prota: " << protagonist->getHealth() << endl;
            emit sendSound("qrc:/sound/smw_kick.wav");
            enemiesInOrder.erase(enemiesInOrder.begin());
        }      //CHECK IF COLLISION WITH MUSHROOM
        else if((protagonist->getXPos() == healtpacksInOrder.front()->getXPos()) && (protagonist->getYPos() == healtpacksInOrder.front()->getYPos())){
            cout << "HP COLLISION" << endl;
            protagonist->setHealth(100);
            emit healthpackGained(protagonist->getHealth(),healtpacksInOrder.front());
            //protagonist->setHealth(protagonist->getHealth()+healtpacksInOrder.front()->getValue());
            //cout << "health prota: " << protagonist->getHealth() << endl;
            emit sendSound("qrc:/sound/smw_1-up.wav");
            if(healtpacksInOrder.size()>0){
                healtpacksInOrder.erase(healtpacksInOrder.begin());
            }
        }
        //CHECK IF GAME STATE HAS CHANGED
        if(enemiesToDefeat.empty() && enemiesInOrder.empty()){
            background->stop();
            emit sendSound("qrc:/sound/smw_castle_clear.wav");
        } else if(enemiesInOrder.empty() && !enemiesToDefeat.empty()) {
            background->stop();
            emit sendSound("qrc:/sound/game_over.wav");
        }

        path.pop_back();
    }
}

void Game::playSound(QString file){
    player->setMedia(QUrl(file));
    player->setVolume(100);
    player->play();
}

void Game::startGame(){
    timer->start(1);
    Enemy * closest;
    float calc_health = 100.0f;
    //STILL ENEMIES LEFT/**
    while(!enemiesToDefeat.empty()){
        //FIND THE CLOSEST ENEMY
        unsigned int indexToDelete = findClosestEnemy(protagonist.get());
        closest = enemiesToDefeat.at(indexToDelete);
        //CAN WE DEAFEAT THE ENEMY WITHOUT DYING?
        if(enoughHealth(calc_health,closest->getValue())){
            //---------YES => calculate remaining health----------------
            calc_health -= closest->getValue();

            //PUSH closest enemy in the list to defeat
            enemiesInOrder.push_back(closest);

            //erase from list
            enemiesToDefeat.erase(remove(enemiesToDefeat.begin(), enemiesToDefeat.end(),enemiesToDefeat.at(indexToDelete)), enemiesToDefeat.end());



            //FIND PATH BETWEEN STARTPOS EN GOAL
            vector<tile_t *> local_path = calculateDistance(start,protagonist.get());
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
            unsigned int closest_index = findClosestHealtpack(protagonist.get());
            Tile * closest_hp = healthpacksOver.at(closest_index);
            healtpacksInOrder.push_back(closest_hp);

            //erase from list
            healthpacksOver.erase(remove(healthpacksOver.begin(), healthpacksOver.end(),healthpacksOver.at(closest_index)), healthpacksOver.end());

            calc_health = 100;

            //FIND PATH BETWEEN STARTPOS EN GOAL
            vector<tile_t *> local_path_hp = calculateDistance(start,closest_hp);
            path.insert(path.begin(),local_path_hp.begin(),local_path_hp.end());

            //SET NEW STARTPOSITION = LAST GOAL
            start = closest_hp;
        } else if(healthpacksOver.empty() && calc_health > 0){
            calc_health -= closest->getValue();

            //PUSH closest enemy in the list to defeat
            enemiesInOrder.push_back(closest);

            //erase from list
            enemiesToDefeat.erase(remove(enemiesToDefeat.begin(), enemiesToDefeat.end(),enemiesToDefeat.at(indexToDelete)), enemiesToDefeat.end());


            //FIND PATH BETWEEN STARTPOS EN GOAL
            vector<tile_t *> local_path = calculateDistance(start,closest);
            //SET GLOBAL PATH = LOCAL PATH (NO FUCKING CLUE WHY THIS WORKS..)
            //            path.insert(path.begin(),local_path.begin(),local_path.end());

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

unsigned int Game::findClosestEnemy(Tile * t){
    unsigned int index = 0;
    double minD = double(INFINITY);
    for(unsigned int i = 0; i<enemiesToDefeat.size();i++){
        double d = calculateDistance(t,enemiesToDefeat.at(i)).size();
        if (d<minD){
            minD = d;
            index = i;
        }
    }
    return index;
}
unsigned int Game::findClosestHealtpack(Tile * t){
    unsigned int index = 0;
    double minD = double(INFINITY);
    for(unsigned int i = 0; i<healthpacksOver.size();i++){
        double d = calculateDistance(t,healthpacksOver.at(i)).size();
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
}

vector<tile_t *> Game::calculateDistance(Tile * start,Tile * goal){
    vector<tile_t *> local_path = m->aStar(map.at(start->getXPos() + start->getYPos()*world->getCols()),map.at(goal->getXPos()+goal->getYPos()*world->getCols()),map);
    m->resetMap(map);
    return local_path;
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
        //        for(int i = 0; i < enemiesCount; i++){
        //            cout << order[i] << "-->";
        //        }
        //        cout << order[enemiesCount] << endl;
        for(int i = 0; i<enemiesToDefeat.size();i++){
            //d += distanceBetweenEnemies[order[i]][order[i+1]];
        }
        if(d<bestD){
            bestOrder = order;
            bestD = d;
        }
        //        cout << "sum: " << d << endl;
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

        for(int i = 1;i<enemiesCount+1;i++){
            if(order[i] > order[x]){
                y = i;
            }
        }

        //        cout << "x: " << x << endl;
        //        cout << "y: " << y << endl;

        int swap = order[x];
        order[x] = order[y];
        order[y] = swap;
        //        for(int i = 0; i < enemiesCount; i++){
        //            cout << order[i] << "-->";
        //        }
        //        cout << order[8] << endl;
        //reverse
        int reversed[enemiesCount + 1];

        //dont reverse everything before x exept after nukken
        for(int i = 0; i <= x; i++){
            reversed[i] = order[i];
        }

        for(int i = x + 1, j = enemiesCount; i < enemiesCount+1; i++, j--){
            reversed[j] = order[i];
        }

        //copy reversed to order
        for(int i = 0; i < enemiesCount +1; i++){
            order[i] = reversed[i];
        }
    }

    cout << "BEST ORDER: " ;
    for(int i = 0; i < enemiesCount; i++){
        cout << bestOrder[i] << "-->";
    }
    cout << order[8] << endl;
    cout << "BEST D:" << bestD << endl;

    return bestOrder;
}
void Game::printElement(vector<int> e){
    for(int i = 0; i < e.size()-1; i++){
        cout << e[i] << "-->";
    }
    cout << e[e.size()-1] << endl;
}





