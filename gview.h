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
    void gamePause();
    void poisonExplosion(qreal,qreal);
    void changeweight(int, double);
    void geneticTrigger();
    void geneticStop();
    void enemyDeadUser(int,int);
    void hpUser(int,int);
    void sendSound(QString);
    void bowserDrawed();
    void setnumbers(int,int);
public slots:
    virtual void updateProtagonist(int x, int y);
    virtual void explodeEnemy(int,int);
    virtual void triggerHealthpack(Tile * hp);
    virtual void drawCurrentBest(vector<tile_t*> path);
    virtual void enemyDead();
    virtual void drawPoisoned(qreal,qreal);
    virtual void penemyDead();
    virtual void collisonDetect();
    virtual void changeEnergybar(int energy);
    virtual void updatePoisonedTiles(int,int,int);
    virtual void displayBowser(int,int);
    virtual void changeHealthbar(int);
    virtual void displayEnd(bool won);

private slots:

    void on_startGame_clicked();

    void on_startGenetic_toggled(bool checked);
    void on_enemiesval_sliderMoved(int position);

    void on_hpval_sliderMoved(int position);

    void on_horizontalSlider_sliderMoved(int position);

    void on_Pause_clicked();

private:
    Ui::Gview *ui;
    ProtagonistUser * mariopix;
};

#endif // Gview_H
