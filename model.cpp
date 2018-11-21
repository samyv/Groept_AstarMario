#include "model.h"

Model::Model()
{

}

Model::~Model(){

}



vector<shared_ptr<tile_t>> Model::makeMap(vector<unique_ptr<Tile>> & tiles){

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

    return map;
}


int Model::aStar(shared_ptr<tile_t> start, shared_ptr<tile_t> goal, vector<shared_ptr<tile_t>> & map){

    int x = start->t->getXPos();
    int y = start->t->getYPos();
    float val = start->t->getValue();

    if(val >= 1.0f){
        return WALL;
    }
    start->closed = true;
    //make_heap(open.begin(), open.end(), Model::comp);
    shared_ptr<tile_t> current = start;
    goal->f = 5;

    open.push(start);
    open.push(goal);

    cout << "start value " << val << " at " << x << ", " << y << endl;
    cout << "goal value " << goal->t->getValue() << " at " << goal->t->getXPos() << ", " << goal->t->getYPos() << endl;

    printqueue(open);
    //while(!open.empty()){
        //run algorithm


    //}

    return SUCCESS;
}


void Model::checkNeighbours(shared_ptr<tile_t> t){

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

    return dist;
}

void Model::printqueue(priority_queue<shared_ptr<tile_t>, vector<shared_ptr<tile_t>>, CustomCompare> q){
    while(!q.empty()){
        int val = q.top()->t->getXPos();
        cout << val << endl;
        q.pop();
    }
}
