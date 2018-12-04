#ifndef GAME_H
#define GAME_H
#include <world_global.h>
#include <world.h>

using namespace std;
class Game
{
public:
    Game();
    World * world;

    vector<unique_ptr<Tile>> tiles;
    vector<unique_ptr<Enemy>> enemies;
//    vector<unique_ptr<Tile>> healthpacks;

    vector<unique_ptr<Tile>> greyTiles;
    void generateWorld();
    int enemiesCount = 5;
    int healthpackCount = 3;
    float defaultStrength = 0.5f;
};
static unique_ptr<Protagonist> protagonist = unique_ptr<Protagonist>(new Protagonist);

static Protagonist * getProtagonist(){
    auto test = protagonist.get();
    return protagonist.get();
}
//unique_ptr<Protagonist> Game::protagonist = unique_ptr<Protagonist>(new Protagonist);
#endif // GAME_H
