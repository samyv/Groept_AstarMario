#ifndef MODEL_H
#define MODEL_H

#include <algorithm>
#include <limits>
#include <iostream>
#include "world.h"
#include "world_global.h"


using namespace std;

#define SUCCESS 0
#define WALL 1
#define NO_PATH 2


typedef struct {
    shared_ptr<Tile> t;
    Tile * prev;
    int f;
    int g;
    int h;
    bool open;
    bool closed;
} tile_t;


class Model
{
public:
    Model();
    ~Model();
     vector<shared_ptr<tile_t>> makeMap(vector<unique_ptr<Tile>> & tiles);
    int aStar(shared_ptr<tile_t> start, shared_ptr<tile_t> goal,  vector<shared_ptr<tile_t>> & map);
private:
    vector<shared_ptr<tile_t>> open;
};



#endif // MODEL_H
