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
        t->t = make_shared<Tile>(*tile);
        t->f = 0;
        t->g = 0;
        t->h = 0;
        t->open = false;
        t->closed = false;
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
    make_heap(open.begin(), open.end());
    shared_ptr<tile_t> current = start;

    cout << "start value " << val << " at " << x << ", " << y << endl;
    cout << "goal value " << goal->t->getValue() << " at " << goal->t->getXPos() << ", " << goal->t->getYPos() << endl;
    /*while(current != goal){
        //run algorithm


    }*/

    return SUCCESS;
}
