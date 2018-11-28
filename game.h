#ifndef GAME_H
#define GAME_H
#include <world_global.h>
#include <world.h>

using namespace std;
class game
{
public:
    game();
    World * world;
    vector<unique_ptr<Tile>> tiles;
    vector<unique_ptr<Enemy>> enemies;
//    vector<unique_ptr<Tile>> healthpacks;
    static unique_ptr<Protagonist> protagonist;
    vector<unique_ptr<Tile>> greyTiles;

    void generateWorld();
    int enemiesCount = 5;
    int healthpackCount = 3;
    float defaultStrength = 0.5f;
};

#endif // GAME_H
