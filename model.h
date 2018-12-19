#ifndef MODEL_H
#define MODEL_H

#include <algorithm>
#include <limits>
#include <iostream>
#include <vector>
#include <queue>
#include <math.h>
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

struct comp //: public std::binary_function<tile_t *, tile_t *, bool>
{
    bool operator()(tile_t * a, tile_t * b){
        return a->f > b->f;
    }
};

class Model
{
public:
    Model();
    ~Model();
    vector<tile_t *> makeMap(vector<unique_ptr<Tile>> & tiles, int rows, int cols);
    vector<tile_t *> aStar(tile_t * start, tile_t * goal,  vector<tile_t *> & map);
private:
    priority_queue<tile_t *, vector<tile_t *>, comp> open;
    //vector<tile_t *> open;
    vector<tile_t *> path;
    void printqueue(priority_queue<tile_t *, vector<tile_t *>, comp> list);
    //void printqueue(vector<tile_t *> list);
    double heuristic(tile_t * a, tile_t * b);
    void checkNeighbours(tile_t ** t, vector<tile_t *> & map);
    int cols;
    int rows;
    tile_t * goal;
    //static bool comp(const tile_t * a, const tile_t * b);
};


#endif // MODEL_H
