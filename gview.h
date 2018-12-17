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
    void drawWorld();
    World * world;
    double displaySize = 0.8;
    QGraphicsScene * scene;
    void makeModel();
    void drawBackground();
    void drawMarioInit();
    int prevX = 0;
    void initDisplay(vector<unique_ptr<Enemy>> & enemies,vector<unique_ptr<Tile>> & healthpacks);
signals:
    void buttonClicked(QString);
    void gameStart();
    void updateHealthbar(float);
public slots:
     void updateProtagonist(int x, int y);
     void explodeEnemy(float health,Enemy * enemy);
     void triggerHealthpack(float health,Tile * hp);
private slots:
     void on_pushButton_clicked();
     void step();
     void changeHealthbar(float);

     void on_startButton_clicked();

private:
    Ui::Gview *ui;
    QGraphicsPixmapItem * mariopix;
};

#endif // Gview_H
