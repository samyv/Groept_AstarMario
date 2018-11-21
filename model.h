#ifndef MODEL_H
#define MODEL_H

#include <algorithm>
#include <limits>
#include <iostream>
#include <vector>
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
    double f;
    double g;
    double h;
    bool open;
    bool closed;
} tile_t;

class Model
{
public:
    Model();
    ~Model();
    vector<shared_ptr<tile_t>> makeMap(vector<unique_ptr<Tile>> & tiles, int rows, int cols);
    int aStar(shared_ptr<tile_t> start, shared_ptr<tile_t> goal,  vector<shared_ptr<tile_t>> & map);
private:
    vector<shared_ptr<tile_t>> open;
    void printqueue(vector<shared_ptr<tile_t>> list);
    double heuristic(shared_ptr<tile_t> a, shared_ptr<tile_t> b);
    void checkNeighbours(shared_ptr<tile_t> t, vector<shared_ptr<tile_t>> & map);
    int cols;
    int rows;
    shared_ptr<tile_t> goal;
    static bool comp(shared_ptr<tile_t> a, shared_ptr<tile_t> b);
};



#endif // MODEL_H
