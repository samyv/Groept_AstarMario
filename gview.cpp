#include "ui_mainwindow.h"
#include "model.h"
#include "gview.h"
#include <iostream>
#include <QDesktopWidget>
#include <QGraphicsScene>
#include "game.h"
#include <QPushButton>

using namespace std;

Gview::Gview(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Gview)
{
    this->setUpdatesEnabled(true);

    QPushButton * test = ui->centralWidget->findChild<QPushButton*>("pushButton");

    connect(test, SIGNAL(test->clicked()), this, SLOT(updateview()));

    //QPushButton * p = new QPushButton("okay", this);

    //QObject::connect(this)

    //setup the scene for grahpicalview
    setupScene();

    //create the world and returns all the tiles
    tiles = createWorld();

    //create a Qimage for the background
    drawBackground();

    drawMarioInit();

    //draw rects for each tile (not used for now)
//    drawWorld();

    //draw a* pathfindi ng
//    makeModel();
}

 vector<std::unique_ptr<Tile>> Gview::createWorld(){
    world = new World();
    return world->createWorld(":/worldmap4.png");
}

void Gview::drawBackground(){
     QImage image = QImage(":/worldmap4.png");
     image = image.scaled(int(image.width() * displaySize), int(image.height() * displaySize), Qt::KeepAspectRatio);
     QPixmap pix = QPixmap::fromImage(image);
     scene->addPixmap(pix);
 }

void Gview::drawMarioInit(){
    QImage mario = QImage(":/mario.png");
    mario = mario.scaled(int(displaySize*24),int(displaySize*24));
    QPixmap protapix = QPixmap::fromImage(mario);
    mariopix = new QGraphicsPixmapItem(protapix);
//    QPainter *paint = new QPainter(protapix)
    QTransform transform;
    transform.translate(20,0);
    //QPixmap * newMario = new QPixmap(protapix.transformed(transform));
    //scene->addPixmap(*newMario);

//    qreal ancho = game::protagonist->getXPos()+50;
//    qreal alto = game::protagonist->getYPos()+50;
    //mariopix->setPos(300,500);
    mariopix->setFlag(QGraphicsItem::ItemIsMovable);
    scene->addItem(mariopix);
}
void Gview::makeModel(){
     vector<shared_ptr<tile_t>> map;

     Model * m = new Model();
     map = m->makeMap(tiles, world->getRows(), world->getCols());
     cout << "success " << map.size() << endl;
     vector<tile_t *> path = m->aStar(map.at(5 + 6*world->getCols()), map.at(1 + 492*world->getCols()), map);
     int count = 0;
     QBrush brush(Qt::SolidPattern);
     QPen pen(Qt::NoPen);
     brush.setColor(QColor(255,0,0));
     pen.setColor(QColor(255,0,0));
     for(auto & tile: path){
         //shared_ptr<QGraphicsRectItem> rect = make_shared<QGraphicsRectItem>(tile->t->getXPos()*displaySize,tile->t->getYPos()*displaySize, displaySize,displaySize,nullptr);
         //rects.push_back(rect);
         scene->addRect(tile->t->getXPos()*displaySize, tile->t->getYPos()*displaySize, displaySize, displaySize,pen,brush);
         //cout << tile->t->getXPos()*displaySize << " " << tile->t->getYPos()*displaySize << endl;
         count++;
     }
     cout << count<< endl;
 }

void Gview::drawWorld(){
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
 }

void Gview::setupScene(){
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
}

void Gview::updateProtagonist(int x, int y){
    cout << x << " " << y << endl;
    //mariopix->setScale(50);
    mariopix->setPos(x,y);


    scene->update();
    this->update();

//    cout << protagonist->getXPos() << endl;
   //auto x = getProtagonist();
   //cout << x->getXPos() << ", " << x->getYPos() << ", " << x->getHealth() << endl;
   // cout << x->getXPos() << endl;

}

void Gview::updateview(){
    for(int i = 0; i < world->getCols(); i++){
        mariopix->setPos(100,i);
        //Sleep(500);
    }
}
Gview::~Gview()
{
    delete ui;
}

