#ifndef GAME_H
#define GAME_H
#include <world_global.h>
#include <QMediaPlayer>
#include <world.h>
#include "gview.h"

using namespace std;
class Game: public QObject
{
    Q_OBJECT
public:
    explicit Game(Gview * gview);
    ~Game();
    World * world;
    vector<unique_ptr<Tile>> tiles;
    vector<unique_ptr<Enemy>> enemies;
    vector<Enemy *> enemiesInOrder;
    vector<int> enemiesInOrderIndexes;
    vector<int> dotheSalesman();
    vector<int> dotheSalesmanG();
    vector<Tile *> healtpacksInOrder;
    vector<Enemy *> enemiesToDefeat;
    vector<unique_ptr<Tile>> healthpacks;
    vector<Tile *> healthpacksOver;
    QMediaPlayer * background;
    QMediaPlayer * player;
    int enemiesDefeated = 0;
    Protagonist * getProtagonist();
    vector<unique_ptr<Tile>> greyTiles;
    unique_ptr<Protagonist> protagonist;
    void generateWorld();
    vector<tile_t *> path;
    vector<vector<vector<tile_t *>>> distanceBetweenEnemies;
    vector<vector<vector<tile_t *>>> calculateDistances();
    unsigned long enemiesCount = 8;
    unsigned int healthpackCount = 8;
    float defaultStrength = 0.5f;
    int generationsAmount = 100;
    void makeModel();
    void printElement(vector<int> e);
    Model * m;
    vector<tile_t *> map;
    Tile * start;
    bool enoughHealth(float curr_health,float strength);
    unsigned int findClosestEnemy(int start_x,int start_y);
    unsigned int findClosestHealtpack(int start_x,int start_y);
    void copyEnemies();
    vector<tile_t *>  calculateDistance(int x, int y,Tile *);
    QTimer * timer;
signals:
    void enemyDefeated(float health,Enemy *);
    void healthpackGained(float health,Tile *);
    void sendSound(QString);
public slots:
    void step();
    void playSound(QString file);
    void startGame();
};
//unique_ptr<Protagonist> Game::protagonist = unique_ptr<Protagonist>(new Protagonist);

#endif // GAME_H
