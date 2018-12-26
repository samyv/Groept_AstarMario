#include "model.h"
#include <time.h>
#include <queue>

Model::Model(vector<Enemy *> & enemiesToDefeat, vector<Tile *> & healthpacksOver, unsigned long e, int g, Protagonist * pro, vector<tile_t *> * p): enemiesToDefeat{enemiesToDefeat}, healthpacksOver{healthpacksOver},
    enemiesCount{e}, generationsAmount{g}, protagonist{pro}, path{p}
{

}

void Model::makeMap(vector<unique_ptr<Tile>> & tiles, int rows, int cols){

    for(unique_ptr<Tile> & tile : tiles){
        tile_t * t = new tile_t;
        t->t = tile.get();
        t->f = 0;
        t->g = 0;
        t->h = 0;
        t->open = false;
        t->closed = false;
        t->prev = nullptr;
        map.push_back(t);
    }

    this->rows = rows;
    this->cols = cols;
}


vector<tile_t *> Model::aStar(tile_t * start, tile_t * goal, vector<tile_t *> & map){
    vector<tile_t *> path;
    open = priority_queue<tile_t *, vector<tile_t *>, comp>();
    this->goal = goal;
    /*if(val >= 1.0f){
        return WALL;
    }*/
    start->open = true;
    //make_heap(open.begin(), open.end(), comp);
    tile_t * current = start;
    tile_t * prev = nullptr;
    open.push(start);
    //open.push_back(start);
    //goal->f = 5;
    //cout << "start value " << val << " at " << x << ", " << y << endl;
    //cout << "goal value " << goal->t->getValue() << " at " << goal->t->getXPos() << ", " << goal->t->getYPos() << endl;
    //open.push(goal);
    //printqueue(open);
    clock_t st = clock();
    int count = 0;
    try {


        while(!open.empty()){
            //run algorithm
            current = open.top();
            if(current == prev){
                cout << "jaja tis dees" << endl;
            }
            prev = current;

            //current = open.front();
            //pop_heap(open.begin(), open.end(), comp);
            //open.pop_back();
            open.pop();

            //cout << "Current: " << current->t->getXPos() << ", " << current->t->getYPos() << " F: " << current->f << endl;
            /*if(current->t->getXPos() == 1181 && current->t->getYPos() == 1185){
            cout << "Goal at current??" << endl;
        }*/
            current->closed = true;

            if(current == goal){
                //get path and initiate draw

                path.push_back(current);
                tile_t * temp = current->prev;
                while(temp != nullptr){
                    path.push_back(temp);
                    if(temp->prev != nullptr){
                        temp = temp->prev;
                    } else {
                        temp = nullptr;
                    }

                }

                //cout << "GOT TO GOAL IN " << path.size() << " STEPS" << endl;
                break;
            }

            checkNeighbours(&current, map);
            //printqueue(open);
            count++;
            //if(count >= 50){
            // break;
            //}

        }
    } catch (const std::bad_alloc) {
        cout << open.size() << endl;
    }

    if(open.empty()){
        cout << "OPEN EMPTY" << endl;
    }
    //cout << "Time passed: " <<(clock() - st)/(CLOCKS_PER_SEC/1000) << endl;
    return path;
}

void Model::resetMap(vector<tile_t *> map){
    for(tile_t * t : map){
        t->open = false;
        t->closed = false;
        t->prev = nullptr;
    }
}
void Model::checkNeighbours(tile_t ** temp, vector<tile_t *> & map){
    tile_t * t = *temp;
    for(int i = -1; i < 2; i++){
        for(int j = -1; j < 2; j++){
            int x = t->t->getXPos() + i;
            int y = t->t->getYPos() + j;
            double newg;

            if(i == 0 || j == 0){
                newg = t->g + 1;
            } else {
                newg = t->g + SQRT;
            }

            if(x + i >= 0  && x + i < cols && y + j >= 0 && y + j < rows && ((i != 0) || (j != 0))){

                int get = x + y*cols;

                //cout << x << ", " << y << " get: " << get;

                tile_t * tile = map.at(uint(get));

                double energy = double(abs(t->t->getValue() - tile->t->getValue()));
                newg += (energy * energyweight);

                //cout << " "<< tile->t->getXPos() << ", " << tile->t->getYPos() << endl;

                //check if in closedset, openset, if not add it to openset and update scores
                if(!tile->closed){
                    bool newpath = false;
                    if(!tile->open){
                        if(tile->t->getValue() == 0.0f || tile->t->getValue() == float(Q_INFINITY)){
                            tile->closed = true;
                            continue;
                        } else {
                            newpath = true;
                        }
                    } else if(newg < tile->g){
                        newpath = true;
                    }

                    if(newpath){


                        tile->f = newg + tile->h;
                        tile->g = newg;
                        tile->prev = t;

                        if(!tile->open){
                            //open.push_back(tile);

                            tile->h = heuristic(tile, goal);
                            tile->open = true;
                            tile->f = stepweight * newg + tile->h * distanceweight;
                            open.push(tile);
                        }

                        //push_heap(open.begin(), open.end(), comp);
                    }
                }
            }
        }
    }
    //sort_heap(open.begin(), open.end(), comp);
}

double Model::heuristic(tile_t * a, tile_t * b){
    //octile distance
    double dx = abs(a->t->getXPos() - b->t->getXPos());
    double dy = abs(a->t->getYPos() - b->t->getYPos());
    double dist;
    if(dx < dy){
        dist = ((dy-dx) + (SQRT * dx));
    } else {
        dist = ((dx-dy) + (SQRT * dy));
    }

    return dist;
}

void Model::printqueue(priority_queue<tile_t *, vector<tile_t *>, comp> list){
    cout << endl << "PRINTING HEAP" << endl;

    while(!list.empty()){
        int x = list.top()->t->getXPos();
        int y = list.top()->t->getYPos();
        double f = list.top()->f;
        list.pop();
        cout << x << " , "<< y << ", F: " << f << endl;
    }
    cout << "DONE PRINTING" << endl << endl;
}

void Model::weightchanged(int i, double val){
    cout << i << ", " << val << endl;
    if(i == 1){
        distanceweight = val;
    } else if(i == 2){
        stepweight = val;
    } else if (i == 3){
        energyweight = val;
    }
}


// SALESMAN


void Model::dotheSalesmanG(){
    unsigned long populationSize = 80;
    vector<vector<int>> population(populationSize,vector<int>(enemiesCount+1,0));
    vector<int> bestOrder(enemiesCount + 1, 0);
    float mutationRate = 0.05;
    double bestD = double(INFINITY);
    vector<double> fitness(populationSize,0);

    srand (time(NULL));
    distanceBetweenEnemies = calculateDistances();
    cout << "lets start salesman G!"<<endl;
    //GENERATE A NEW POPULATION!
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

    //MAKE A NEW GENARION BASED ON PREVIOUS ONE
    for(int g = 0; g<generationsAmount;g++){
        //CALCULATE TOTAL DISTANCE FOR EVERY MEMBER
        for(unsigned long i = 0; i< populationSize;i++){
            vector<int> member = population[i];
            //CALC DISTANCE FOR EVERY MEMBER OF THE POPULATION
            double d = 0;
            for(int i = 0; i<enemiesToDefeat.size();i++){
                d += distanceBetweenEnemies[member[i]][member[i+1]].size();
            }
            //CHECK IF WE HAVE A NEW BEST ONE
            if(d < bestD){
                bestD = d;
                bestOrder = member;
                vector<tile_t *> bestOrderPath;
                for(int i = 0; i < enemiesCount;i++){
                    vector<tile_t*> pathBetweenEnemies  = distanceBetweenEnemies[uint(bestOrder[uint(i)])][uint(bestOrder[uint(i+1)])];
                    bestOrderPath.insert(bestOrderPath.begin(),pathBetweenEnemies.begin(),pathBetweenEnemies.end());
                }
                //DRAW THE CURRENT BEST ONE
                emit newBest(bestOrderPath);
            }
            fitness[i] = 1/(d+1);
        }

        //NORMALIZE FITNESSES
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
                int get = rand();
                if(get >= RAND_MAX){
                    get = RAND_MAX - 1;
                } else if(get <= 0){
                    get = 1;
                }
                double r = (double(get) / (RAND_MAX)); //GENERATES RANDOM NUMBER BETWEEN 0 AND 1
                while(r>0){
                    r = r - fitness[index];
                    index++;
                }
                index--;
                picks[c] = population[index];
            }


            //CROSSOVERRR
            //DEFINE RANDOM RANGE IN FIRST PICK
            int startA = 1 + (rand() % (picks[0].size()-2));
            int endA = startA + rand() % (picks[0].size()-startA-1);
            int range = endA - startA+1; // +1 because we also need to count first element

            //copy first dna part
            vector<int> crossOver(range,0);
            for(int n = 0; n <range;n++){
                crossOver.at(n) = picks[0][startA+n];
            }
            //INSERT 2nd DNA PART
            for(int c = 1;c<picks[1].size();c++){
                int enemy = picks[1][c];
                vector<int>::iterator it;
                it = find(crossOver.begin(),crossOver.end(),enemy);
                if (it == crossOver.end()) //FOUND NOT IN VECTOR c1
                    crossOver.push_back(enemy);
            }

            //INSERT FIRST POSITION: THE PROTAGONIST OF COURSE
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
                    //XOR
                    //crossOver[indexA] = crossOver[indexA] ^ crossOver[indexB];
                    //crossOver[indexB] = crossOver[indexB] ^ crossOver[indexA];
                    //crossOver[indexA] = crossOver[indexA] ^ crossOver[indexB];

                }
            }
            newPopulation[i] = crossOver;

        }
        population = newPopulation;
    }


    printElement(bestOrder);
    for(int i = 0; i < enemiesCount;i++){
        vector<tile_t*> pathBetweenEnemies  = distanceBetweenEnemies[uint(bestOrder[uint(i)])][uint(bestOrder[uint(i+1)])];
        path->insert(path->begin(),pathBetweenEnemies.begin(),pathBetweenEnemies.end());
    }
    emit salesmanDone();
}

vector<vector<vector<tile_t *>>> Model::calculateDistances(){
    vector<vector<vector<tile_t *>>> localArray(enemiesCount + 1, vector<vector<tile_t *>>(enemiesCount + 1, vector<tile_t *>(0,nullptr)));
    for(unsigned long i = 0; i<enemiesCount + 1;i++){
        for(unsigned long j = 0; j<enemiesCount + 1;j++){
            if(j == i){
                localArray.at(i).at(j) = vector<tile_t *>(0,nullptr);
            } else if(i==0 && j != 0){
                vector<tile_t *> pathBetweenEnemies = calculateDistance(protagonist,enemiesToDefeat.at(j-1));
                localArray.at(i).at(j) = pathBetweenEnemies;
                vector<tile_t *> pathBetweenEnemiesRevert(pathBetweenEnemies.rbegin(),pathBetweenEnemies.rend());
                localArray.at(j).at(i) = pathBetweenEnemiesRevert;
            } else if(i != 0 && j != 0 && j > i){
                vector<tile_t *> pathBetweenEnemies = aStar(map.at(enemiesToDefeat.at(i-1)->getXPos() + enemiesToDefeat.at(i-1)->getYPos()*cols),map.at(enemiesToDefeat.at(j-1)->getXPos()+enemiesToDefeat.at(j-1)->getYPos()*cols),map);
                resetMap(map);
                localArray.at(i).at(j) = pathBetweenEnemies;
                vector<tile_t *> pathBetweenEnemiesRevert(pathBetweenEnemies.rbegin(),pathBetweenEnemies.rend());
                localArray.at(j).at(i) = pathBetweenEnemiesRevert;
            }
        }
    }
    return localArray;
}

void Model::dotheSalesman(){
    vector<int> order(enemiesCount + 1, 0);
    vector<int> bestOrder(enemiesCount + 1, 0);
    double bestD = double(INFINITY);
    distanceBetweenEnemies = calculateDistances();
    for(int i = 0; i< int(enemiesToDefeat.size() + 1);i++){
        order.at(i) = i;
    }
    while(1){
        double d = 0;
        for(int i = 0; i<enemiesToDefeat.size();i++){
            d += distanceBetweenEnemies[order[i]][order[i+1]].size();
        }
        if(d<bestD){
            bestOrder = order;
            bestD = d;
        }

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
        //reversevector<int>
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
    for(int i = 0; i < enemiesCount;i++){
        vector<tile_t*> pathBetweenEnemies  = distanceBetweenEnemies[uint(bestOrder[uint(i)])][uint(bestOrder[uint(i+1)])];
        path->insert(path->begin(),pathBetweenEnemies.begin(),pathBetweenEnemies.end());
    }
    emit salesmanDone();
}
void Model::printElement(vector<int> e){
    for(int i = 0; i < e.size()-1; i++){
        cout << e[i] << "-->";
    }
    cout << e[e.size()-1] << endl;
}

vector<tile_t *> Model::calculateDistance(Tile * start,Tile * goal){
    vector<tile_t *> local_path = aStar(map.at(start->getXPos() + start->getYPos()*cols),map.at(goal->getXPos()+goal->getYPos()*cols),map);
    resetMap(map);
    return local_path;
}

void Model::startGame(){
    Enemy * closest;
    float calc_health = 100.0f;
    Tile * start = protagonist;
    //STILL ENEMIES LEFT/**
    vector<Enemy *> enemiesInOrder;
    vector<Tile *> healtpacksInOrder;
    emit salesmanDone();
    while(!enemiesToDefeat.empty()){
        //FIND THE CLOSEST ENEMY
        unsigned int indexToDelete = findClosestEnemy(protagonist);
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
            vector<tile_t *> local_path = calculateDistance(start,protagonist);
            //SET GLOBAL PATH = LOCAL PATH (NO FUCKING CLUE WHY THIS WORKS..)
            path->insert(path->begin(),local_path.begin(),local_path.end());

            //WHEN PATH IS FOUND WE SET THE NEW STARTPOSITION = LAST GOAL
            start = closest;

            //WE CLEAR LOCAL PATH
            //            vector<tile_t *>().swap(local_path);

            //---------NO => Search healthpack----------------\\
            //CHECK IF THERE ARE HEALTHPACKS LEFT
        } else if (!healthpacksOver.empty()) {
            //YES => find CLOSEST HEALTHPACK
            unsigned int closest_index = findClosestHealtpack(protagonist);
            Tile * closest_hp = healthpacksOver.at(closest_index);
            healtpacksInOrder.push_back(closest_hp);

            //erase from list
            healthpacksOver.erase(remove(healthpacksOver.begin(), healthpacksOver.end(),healthpacksOver.at(closest_index)), healthpacksOver.end());

            calc_health = 100;

            //FIND PATH BETWEEN STARTPOS EN GOAL
            vector<tile_t *> local_path_hp = calculateDistance(start,closest_hp);
            path->insert(path->begin(),local_path_hp.begin(),local_path_hp.end());

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
            path->insert(path->begin(),local_path.begin(),local_path.end());

            //WHEN PATH IS FOUND WE SET THE NEW STARTPOSITION = LAST GOAL
            start = closest;
            break;
        }
    }
}

bool Model::enoughHealth(float curr_health,float strength){
    return (curr_health - strength > 0);
}

unsigned int Model::findClosestEnemy(Tile * t){
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

unsigned int Model::findClosestHealtpack(Tile * t){
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
