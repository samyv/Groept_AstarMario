#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    world = new World();

    vector<std::unique_ptr<Tile>> tiles = world->createWorld(":/worldmap4.png");
    vector<std::shared_ptr<QGraphicsRectItem>> rects;

    QBrush brush;
    QPen pen;
    pen.setWidth(0);
    for(auto & tile: tiles){
        float greyscale = 0.0f;

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
        shared_ptr<QGraphicsRectItem> rect = make_shared<QGraphicsRectItem>(tile->getXPos()*displaySize,tile->getYPos()*displaySize, displaySize,displaySize,nullptr);
        rects.push_back(rect);
        scene->addRect(tile->getXPos()*displaySize, tile->getYPos()*displaySize, displaySize, displaySize,pen,brush);
    }


}

MainWindow::~MainWindow()
{
    delete ui;
}
