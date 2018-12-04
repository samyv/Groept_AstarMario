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
    vector<std::unique_ptr<Tile>> tiles;
    vector<std::unique_ptr<Tile>> createWorld();
    void drawWorld();
    World * world;
    double displaySize = 1.5;
    QGraphicsScene * scene;
    void makeModel();
    void drawBackground();
    void drawMarioInit();
public slots:
     void updateProtagonist(int x, int y);
private:
    Ui::Gview *ui;
};

#endif // Gview_H
