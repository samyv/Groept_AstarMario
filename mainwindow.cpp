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
    vector<std::unique_ptr<Tile>> tiles;
    try{
        tiles = world->createWorld(":/maze3.png");
    } catch(QString e){
        cout << e.toStdString() << endl;
    };
    QImage image = QImage(":/maze3.png");
    QImage back = image.scaled(int(image.width() * displaySize), int(image.height() * displaySize), Qt::KeepAspectRatio);
    QPixmap pix = QPixmap::fromImage(back);

    scene->addPixmap(pix);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);

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
        //                scene->addRect(tile->getXPos()*displaySize, tile->getYPos()*displaySize, displaySize, displaySize,pen,brush);
    }
    vector<tile_t *> map;

    Model * m = new Model();
    map = m->makeMap(tiles, world->getRows(), world->getCols());
    cout << "success " << map.size() << endl;
    vector<tile_t *> path = m->aStar(map.at(49 + 36*world->getCols()), map.at(1183 + 1185*world->getCols()), map); // maze1: 6,6 ; 1,492 worldmap4: 0, 45;  999, 937  maze3 : 49,36 ; 1183, 1185
    int count = 0;
    brush.setColor(QColor(255,0,0));
    pen.setColor(QColor(255,0,0));
    cout << "path " << path.size() << endl;
    for(auto & tile: path){
        //shared_ptr<QGraphicsRectItem> rect = make_shared<QGraphicsRectItem>(tile->t->getXPos()*displaySize,tile->t->getYPos()*displaySize, displaySize,displaySize,nullptr);
        //rects.push_back(rect);
        //if(tile->open == true){
        scene->addRect(tile->t->getXPos()*displaySize, tile->t->getYPos()*displaySize, displaySize, displaySize,pen,brush);
        //}
        //cout << tile->t->getXPos()*displaySize << " " << tile->t->getYPos()*displaySize << endl;
        count++;
    }
    cout << count<< endl;
}

MainWindow::~MainWindow()
{
    delete ui;
}
