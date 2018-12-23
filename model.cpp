#include "model.h"
#include <time.h>
#include <queue>
Model::Model()
{

}

Model::~Model(){

}



vector<tile_t *> Model::makeMap(vector<unique_ptr<Tile>> & tiles, int rows, int cols){

    vector<tile_t *> map;

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

    return map;
}


vector<tile_t *> Model::aStar(tile_t * start, tile_t * goal, vector<tile_t *> & map){
    vector<tile_t *> path;
    open = priority_queue<tile_t *, vector<tile_t *>, comp>();
    this->goal = goal;
    int x = start->t->getXPos();
    int y = start->t->getYPos();
    float val = start->t->getValue();

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

                //cout << " "<< tile->t->getXPos() << ", " << tile->t->getYPos() << endl;

                //check if in closedset, openset, if not add it to openset and update scores
                if(!tile->closed){
                    bool newpath = false;
                    if(!tile->open){
                        if(tile->t->getValue() == 0.0f || tile->t->getValue() == float(INFINITY)){
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
                        tile->f = newg + tile->h;
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

    return dist/1.01;
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

/*void Model::printqueue(vector<tile_t *> list){
    cout << endl << "PRINTING HEAP" << endl;

    for(uint i = 0; i < list.size(); i++){
        int x = list.at(i)->t->getXPos();
        int y = list.at(i)->t->getYPos();
        double f = list.at(i)->f;
        cout << x << " , "<< y << ", F: " << f << endl;
    }
    cout << "DONE PRINTING" << endl << endl;
}

bool Model::comp(const tile_t * a, const tile_t * b){
    return a->f > b->f;
}*/
