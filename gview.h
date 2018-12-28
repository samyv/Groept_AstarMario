#ifndef GVIEW_H
#define GVIEW_H

#include <QMainWindow>
//from library
#include <world.h>
#include <world_global.h>
#include "ui_mainwindow.h"

#include <QtCore>
#include <QtGui>
#include "math.h"

#include "model.h"
#include <iostream>
#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QDesktopWidget>
#include "protagonistuser.h"


namespace Ui {
class Gview;
}

class Gview : public QMainWindow
{
    Q_OBJECT

public:
    explicit Gview(QWidget *parent=nullptr);
    ~Gview();
    void setupScene();
    int i = 0;
    vector<std::unique_ptr<Tile>> tiles;
    vector<std::unique_ptr<Tile>> createWorld();
    vector<QGraphicsPixmapItem *> enemiesPixs;
    vector<QGraphicsPixmapItem *> hpPixs;
    World * world;
    double displaySize = 0.8;
    QGraphicsScene * scene;
    QGraphicsOpacityEffect *eff;

    QGraphicsPixmapItem * bowserPix;
    void makeModel();
    void drawBackground();
    void drawMarioInit();
    int prevX = 0;
    void initDisplay(vector<unique_ptr<Tile>> & tiles,vector<unique_ptr<Enemy>> & enemies,vector<unique_ptr<Tile>> & healthpacks);
    vector<unique_ptr<QGraphicsRectItem>> bestPath;
    vector<QGraphicsRectItem *> rects;

    ProtagonistUser *getMariopix() const;

signals:
    void buttonClicked(QString);
    void gameStart();
    void poisonExplosion(qreal,qreal);
    void changeweight(int, double);
    void geneticTrigger();
    void geneticStop();
    void enemyDeadUser(int,int);
    void hpUser(int,int);
    void sendSound(QString);
public slots:
    void updateProtagonist(int x, int y);
    void explodeEnemy(int,int);
    void triggerHealthpack(Tile * hp);
    void drawCurrentBest(vector<tile_t*> path);
    void enemyDead();
    void drawPoisoned(qreal,qreal);
    void penemyDead();
    void collisonDetect();
    void changeEnergybar(int energy);
    void updatePoisonedTiles(int,int,int);
    void displayBowser(Tile *);

private slots:

    void changeHealthbar(int);

    void on_startGame_clicked();

    void on_startGenetic_toggled(bool checked);
private:
    Ui::Gview *ui;
    ProtagonistUser * mariopix;
};

#endif // Gview_H
