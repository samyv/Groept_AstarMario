#include "model.h"
#include <time.h>
Model::Model()
{

}

Model::~Model(){

}



vector<shared_ptr<tile_t>> Model::makeMap(vector<unique_ptr<Tile>> & tiles, int rows, int cols){

    vector<shared_ptr<tile_t>> map;

    for(unique_ptr<Tile> & tile : tiles){
        shared_ptr<tile_t> t = shared_ptr<tile_t>(new tile_t);
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


vector<tile_t *> Model::aStar(shared_ptr<tile_t> start, shared_ptr<tile_t> goal, vector<shared_ptr<tile_t>> & map){
    this->goal = goal;

    int x = start->t->getXPos();
    int y = start->t->getYPos();
    float val = start->t->getValue();

    /*if(val >= 1.0f){
        return WALL;
    }*/
    start->open = true;
    make_heap(open.begin(), open.end(), Model::comp);
    shared_ptr<tile_t> current = start;
    open.push_back(start);

    cout << "start value " << val << " at " << x << ", " << y << endl;
    cout << "goal value " << goal->t->getValue() << " at " << goal->t->getXPos() << ", " << goal->t->getYPos() << endl;

    //    printqueue(open);
    clock_t st = clock();
    int count = 0;
    while(!open.empty()){
        //run algorithm
        current = open.front();
        pop_heap(open.begin(), open.end(), comp);
        open.pop_back();

//        cout << "Current: " << current->t->getXPos() << ", " << current->t->getYPos() << " F: " << current->f << endl;

        current->closed = true;

        if(current == goal){
            //get path and initiate draw

            path.push_back(current.get());
            tile_t * temp = current->prev;
            while(temp != nullptr){
                path.push_back(temp);
                if(temp->prev != nullptr){
                    temp = temp->prev;
                } else {
                    temp = nullptr;
                }

            }

            cout << "GOT TO GOAL IN " << path.size() << " STEPS" << endl;
            break;
        }

        checkNeighbours(&current, map);
//        printqueue(open);
        count++;
//        if(count >= 5){
//            break;
//        }

    }
    cout << "Time passed: " <<(clock() - st)/(CLOCKS_PER_SEC/1000) << endl;
    return path;
}


void Model::checkNeighbours(shared_ptr<tile_t> * temp, vector<shared_ptr<tile_t>> & map){
    shared_ptr<tile_t> t = *temp;
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

            if(x + i >= 0  && x + j < cols && y + j >= 0 && y + j < rows && ((i != 0) || (j != 0))){

                int get = x + y*rows;

                //cout << x << ", " << y << " get: " << get;

                shared_ptr<tile_t> tile = map.at(uint(get));

                //cout << " "<< tile->t->getXPos() << ", " << tile->t->getYPos() << endl;

                //check if in closedset, openset, if not add it to openset and update scores
                if(!tile->closed){
                    bool newpath = false;
                    if(!tile->open){
                        if(tile->t->getValue() != 1.0f){
                            tile->closed = true;
                        } else {
                            newpath = true;
                        }
                    } else if(newg < tile->g){
                        newpath = true;
                    }

                    if(newpath){

                        if(!tile->open){
                            open.push_back(tile);
                            tile->h = heuristic(tile, goal);
                            tile->open = true;
                        }
                        tile->f = newg + tile->h;
                        tile->g = newg;
                        tile->prev = t.get();
                        push_heap(open.begin(), open.end(), comp);
                    }
                }
            }
        }
    }
    //sort_heap(open.begin(), open.end(), comp);
}

double Model::heuristic(shared_ptr<tile_t> a, shared_ptr<tile_t> b){
    //octile distance
    double dx = abs(a->t->getXPos() - b->t->getXPos());
    double dy = abs(a->t->getYPos() - b->t->getYPos());
    double dist;
    double min;
    double max;
    if(dx < dy){
        min = dx;
        max = dy;
        dist = ((dy-dx) + (SQRT * dx));
    } else {
        dist = ((dx-dy) + (SQRT * dy));
        min = dy;
        max = dx;
    }

    return dist/10;
}

void Model::printqueue(vector<shared_ptr<tile_t>> list){
    cout << endl << "PRINTING HEAP" << endl;
    for(uint i = 0; i < list.size(); i++){
        int x = list.at(i)->t->getXPos();
        int y = list.at(i)->t->getYPos();
        double f = list.at(i)->f;
        cout << x << " , "<< y << ", F: " << f << endl;
    }
    cout << "DONE PRINTING" << endl << endl;
}

bool Model::comp(shared_ptr<tile_t> a, shared_ptr<tile_t> b)
{
    return a->f > b->f;
}
