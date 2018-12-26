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
    bool ai = true;
    vector<unique_ptr<Tile>> tiles;
    vector<unique_ptr<Enemy>> enemies;
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
    float poisonPercentage = 0.3;
    unsigned int healthpackCount = 8;
    float defaultStrength = 0.5f;
    int generationsAmount = 100;
    unsigned long enemiesCount = 6;
    void makeModel();
    Model * m;
    void copyEnemies();
    QTimer * timer;
    void eventFilter(QKeyEvent *e);
signals:
    void enemyDefeated(float health,Enemy *);
    void healthpackGained(Tile *);
    void sendSound(QString);
    void changeMario(int,int);
    void updateHealthbar(float);
    void checkCollision();
    void poisonedTile(qreal,qreal);
public slots:
    void step();
    void userEnemyDefeated(int,int);
    void stepUser();
    void playSound(QString file);
    void setNeighboursPoison(qreal,qreal);
    void startTime();
    void checkTile(int,int);
    void hpTrigger(int,int);
};
//unique_ptr<Protagonist> Game::protagonist = unique_ptr<Protagonist>(new Protagonist);

#endif // GAME_H
