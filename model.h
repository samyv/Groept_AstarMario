#ifndef MODEL_H
#define MODEL_H

#include <algorithm>
#include <limits>
#include <iostream>
#include <queue>
#include "world.h"
#include "world_global.h"


using namespace std;

#define SUCCESS 0
#define WALL 1
#define NO_PATH 2
#define SQRT  1.4142135


struct tile;

typedef tile tile_t;

typedef struct tile{
    Tile * t;
    tile_t * prev;
    int f;
    int g;
    int h;
    bool open;
    bool closed;
} tile_t;

struct CustomCompare
{
    bool operator()(shared_ptr<tile_t> a, shared_ptr<tile_t> b)
    {
        return a->f > b->f;
    }
};

class Model
{
public:
    Model();
    ~Model();
    vector<shared_ptr<tile_t>> makeMap(vector<unique_ptr<Tile>> & tiles);

    int aStar(shared_ptr<tile_t> start, shared_ptr<tile_t> goal,  vector<shared_ptr<tile_t>> & map);
private:
    priority_queue<shared_ptr<tile_t>, vector<shared_ptr<tile_t>>, CustomCompare> open;
    void printqueue(priority_queue<shared_ptr<tile_t>, vector<shared_ptr<tile_t>>, CustomCompare> q);
    double heuristic(shared_ptr<tile_t> a, shared_ptr<tile_t> b);
    void checkNeighbours(shared_ptr<tile_t> t);

};



#endif // MODEL_H
