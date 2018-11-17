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
    // Optimization
    ui->graphicsView->setOptimizationFlags(QGraphicsView::DontClipPainter);
    ui->graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    ui->graphicsView->setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing);
    ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);

    ui->graphicsView->setScene(scene);
    //ui->graphicsView->setSceneRect(0, 0, width(), height());
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

    //better way would be to store in array
    //TODO: change to 2d array structure, and in struct tile_t other way get Tile into struct
    //atm shared_ptr to make_shared from unique_ptr
    vector<shared_ptr<tile_t>> map;

    Model * m = new Model();
    map = m->makeMap(tiles);
    cout << "success " << map.size() << endl;
    m->aStar(map.at(0), map.at(999999), map);
}

MainWindow::~MainWindow()
{
    delete ui;
}
