#ifndef GAME_H
#define GAME_H
#include <world_global.h>
#include <world.h>
#include "gview.h"

using namespace std;
class Game: public QObject
{
    Q_OBJECT
public:
    explicit Game(Gview * gview);
    World * world;
    vector<unique_ptr<Tile>> tiles;
    vector<unique_ptr<Enemy>> enemies;
    vector<unique_ptr<Tile>> healthPacks;
//    vector<unique_ptr<Tile>> healthpacks;
    Protagonist * getProtagonist();
    vector<unique_ptr<Tile>> greyTiles;
    unique_ptr<Protagonist> protagonist;
    void generateWorld();
    vector<tile_t *> path;
    int enemiesCount = 5;
    int healthpackCount = 3;
    float defaultStrength = 0.5f;
    void makeModel();
    vector<tile_t *> map;
public slots:
    void step();
};
//unique_ptr<Protagonist> Game::protagonist = unique_ptr<Protagonist>(new Protagonist);

#endif // GAME_H
