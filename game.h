#ifndef GAME_H
#define GAME_H
#include <world_global.h>
#include <QMediaPlayer>
#include <world.h>
#include "gview.h"
#include "xenemy.h"

using namespace std;
class Game: public QObject
{
    Q_OBJECT
public:
    explicit Game(Gview * gview);
    ~Game();
    World * world;
    bool ai = true;
    vector<unique_ptr<Tile>> tiles;
    vector<unique_ptr<Enemy>> enemies;
    XEnemy * bowser;
    vector<Enemy *> enemiesInOrder;
    vector<int> enemiesInOrderIndexes;
    void makeSomePoison();
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
    bool geneticCont = false;
    vector<tile_t *> path;
    bool finalGameStarted = false;
    vector<tile_t *> final_path;
    float poisonPercentage = 0.3;
    unsigned int healthpackCount = 3;
    float defaultStrength = 0.5f;
    int generationsAmount = 300;
    unsigned long enemiesCount = 3;
    void makeModel();
    Model * m;
    void copyEnemies();
    QTimer * timer;
    void here(int);
    void eventFilter(QKeyEvent *e);
signals:
    void enemyDefeated(float health,Enemy *);
    void healthpackGained(Tile *);
    void sendSound(QString);
    void updateHealthbar(float);
    void checkCollision();
    void poisonedTile(qreal,qreal);
    void PEnemyTrigger(int,int);
public slots:
    void step();
    void userEnemyDefeated(int,int);
    void stepUser();
    void playSound(QString file);
    void setNeighboursPoison(qreal,qreal);
    void startTime();
    void checkTile(int,int);
    void hpTrigger(int,int);
    void gametimer();
    void pauseTimer();
    void setBowser(Tile*);

};
//unique_ptr<Protagonist> Game::protagonist = unique_ptr<Protagonist>(new Protagonist);

#endif // GAME_H
