#ifndef MODEL_H
#define MODEL_H

#include <algorithm>
#include <limits>
#include <iostream>
#include <vector>
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
    double f;
    double g;
    double h;
    bool open;
    bool closed;
} tile_t;

typedef struct {
    vector<tile_t *> path;
    double cost;
} path_t;

struct comp //: public std::binary_function<tile_t *, tile_t *, bool>
{
    bool operator()(tile_t * a, tile_t * b){
        return a->f > b->f;
    }
};

class Model: public QObject
{
    Q_OBJECT
public:
    explicit Model(vector<Enemy *> & enemiesToDefeat,vector<Tile *> & healthpacksOver, unsigned long e, int g, Protagonist * pro, vector<tile_t *> * p);
    Protagonist * protagonist;
    vector<Enemy *> enemiesToDefeat;
    vector<Tile *> healthpacksOver;
    vector<tile_t *> * path;
    void makeMap(vector<unique_ptr<Tile>> & tiles, int rows, int cols);
    vector<tile_t *> map;
    path_t * aStar(tile_t * start, tile_t * goal,  vector<tile_t *> & map);
    void resetMap(vector<tile_t *> map);
    double distanceweight = 1;
    double stepweight = 1;
    double energyweight = 1;
    vector<vector<path_t *>> distanceBetweenEnemies;
    vector<vector<path_t *>> calculateDistances();
    unsigned long enemiesCount = 6;
    int generationsAmount = 100;
    void printElement(vector<int> e);
    path_t * calculateDistance(Tile *,Tile *);
    bool enoughHealth(float curr_health,float strength);
    unsigned int findClosestEnemy(Tile * t);
    unsigned int findClosestHealtpack(Tile * t);
private:
    priority_queue<tile_t *, vector<tile_t *>, comp> open;
    //vector<tile_t *> open;
    void printqueue(priority_queue<tile_t *, vector<tile_t *>, comp> list);
    //void printqueue(vector<tile_t *> list);
    double heuristic(tile_t * a, tile_t * b);
    void checkNeighbours(tile_t ** t, vector<tile_t *> & map);
    int cols;
    int rows;
    tile_t * goal;

    //static bool comp(const tile_t * a, const tile_t * b);
signals:
    void newBest(vector<tile_t*> newBest);
    void salesmanDone();
public slots:
    void weightchanged(int,double);
    void dotheSalesmanG();
    void dotheSalesman();
    void startGame();
};


#endif // MODEL_H
