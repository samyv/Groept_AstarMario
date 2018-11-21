#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "model.h"

#include <iostream>
#include <QDesktopWidget>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);

    ui->graphicsView->setScene(scene);

    world = new World();
    QImage image = QImage(":/maze1.png");
    QPixmap pix = QPixmap::fromImage(image);
    scene->addPixmap(pix);

    vector<std::unique_ptr<Tile>> tiles = world->createWorld(":/maze1.png");
    vector<std::shared_ptr<QGraphicsRectItem>> rects;
    QBrush brush(Qt::SolidPattern);
    QPen pen(Qt::NoPen);
    float greyscale = 0.0f;
    for(auto & tile: tiles){



        if(tile->getValue() != INFINITY && tile->getValue() != 1.0f){
            greyscale = tile->getValue()*255.0f;
            brush.setColor(QColor(greyscale,greyscale,greyscale));
            pen.setColor(QColor(greyscale,greyscale,greyscale));


        } else if(tile->getValue() == INFINITY){
            brush.setColor(Qt::black);
            pen.setColor(Qt::black);

        } else {
            brush.setColor(Qt::white);
            pen.setColor(Qt::white);
        }
        //        shared_ptr<QGraphicsRectItem> rect = make_shared<QGraphicsRectItem>(tile->getXPos()*displaySize,tile->getYPos()*displaySize, displaySize,displaySize,nullptr);
        //        rects.push_back(rect);
        //        scene->addRect(tile->getXPos()*displaySize, tile->getYPos()*displaySize, displaySize, displaySize,pen,brush);
    }

    vector<shared_ptr<tile_t>> map;

    Model * m = new Model();
    map = m->makeMap(tiles, world->getRows(), world->getCols());
    cout << "success " << map.size() << endl;
    vector<tile_t *> path = m->aStar(map.at(5 + 6*world->getCols()), map.at(1 + 492*world->getCols()), map);

}

MainWindow::~MainWindow()
{
    delete ui;
}
