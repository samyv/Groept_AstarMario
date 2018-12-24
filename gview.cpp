#include "ui_mainwindow.h"
#include "model.h"
#include "gview.h"
#include <iostream>
#include <QDesktopWidget>
#include <QGraphicsScene>
#include "game.h"
#include <QPushButton>
#include <QtAlgorithms>

using namespace std;

Gview::Gview(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Gview)
{
    this->setUpdatesEnabled(true);

    //QPushButton * p = new QPushButton("okay", this);

    //QObject::connect(this)

    QObject::connect(this,SIGNAL(updateHealthbar(float)), this,SLOT(changeHealthbar(float)));

    //setup the scene for grahpicalview
    setupScene();

    //create a Qimage for the background
    drawBackground();

    drawMarioInit();

    //draw rects for each tile (not used for now)
    //    drawWorld();

    //draw a* pathfindi ng
    //    makeModel();
}

Gview::~Gview()
{
    delete ui;
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
    mariopix->setOffset(-mario.width()/2,-mario.height());
    mariopix->setOffset(-mario.width()/2,-mario.height());
    scene->addItem(mariopix);
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
    ui->healthbar->setValue(100);
}

void Gview::updateProtagonist(int x, int y){
    mariopix->setPos(x*displaySize,y*displaySize);
    int scale = x - prevX;
    if(scale != 0){
        mariopix->setTransform(QTransform::fromScale(scale,1));
    }

    prevX = x;
    QBrush brush(Qt::SolidPattern);
    QPen pen(Qt::SolidLine);
    pen.setWidth(3);
    brush.setColor(QColor(255,0,0));
    pen.setColor(QColor(255,0,0));
    scene->addRect(x*displaySize, y*displaySize, displaySize, displaySize,pen,brush);
    mariopix->setZValue(1);
    ui->graphicsView->centerOn(x*displaySize,y*displaySize);
}

void Gview::initDisplay(vector<unique_ptr<Enemy>> & enemies,vector<unique_ptr<Tile>> & healthpacks){
    QImage goomba = QImage(":/goombaretro.png");
    goomba = goomba.scaled(int(displaySize*24),int(displaySize*24));
    QPixmap goombapix = QPixmap::fromImage(goomba);
    for(unsigned int i = 0; i < enemies.size();i++){
        QGraphicsPixmapItem * enemypix = new QGraphicsPixmapItem(goombapix);
        enemiesPixs.push_back(enemypix);
        Enemy * enemy = enemies.at(i).get();
        enemypix->setPos(enemy->getXPos()*displaySize,enemy->getYPos()*displaySize);
        enemypix->setOffset(-goomba.width()/2,-goomba.height()/2);
        scene->addItem(enemypix);
    }

    QImage mushroom = QImage(":/mushroom.png");
    mushroom = mushroom.scaled(int(displaySize*24),int(displaySize*24));
    QPixmap mushroompix = QPixmap::fromImage(mushroom);
    for(unsigned int i = 0; i < healthpacks.size();i++){
        QGraphicsPixmapItem * healthpackpix = new QGraphicsPixmapItem(mushroompix);
        hpPixs.push_back(healthpackpix);
        Tile * hp = healthpacks.at(i).get();
        healthpackpix->setPos(hp->getXPos()*displaySize,hp->getYPos()*displaySize);
        healthpackpix->setOffset(-mushroom.width()/2,-mushroom.height()/2);
        scene->addItem(healthpackpix);
    }


}

void Gview::on_startGame_clicked()
{
    double val = ui->distanceweightval->text().toDouble();
    emit changeweight(1, val);
    cout << val <<endl;
    val = ui->stepweightval->text().toDouble();
    emit changeweight(2, val);
    cout << val <<endl;
    val = ui->energyweightval->text().toDouble();
    emit changeweight(3, val);
    cout << val <<endl;
    emit gameStart();
}

void Gview::explodeEnemy(float health,Enemy * enemy){
    emit updateHealthbar(health - enemy->getValue());
    QImage goomba = QImage(":/goombaretro.png");
    goomba = goomba.scaled(int(displaySize*24),int(displaySize*12));
    QPixmap goombapix = QPixmap::fromImage(goomba);
    for(int i = 0;i<enemiesPixs.size();i++){
        QGraphicsPixmapItem * local = enemiesPixs.at(i);
        if(mariopix->collidesWithItem(local)){
            local->setOffset(-goomba.width()/2,-goomba.height()/2);
            QTransform transform;
            transform.translate(0, goomba.height()/2);
            local->setTransform(transform);
            local->setPixmap(goombapix);
        }
    }

}

void Gview::triggerHealthpack(float health,Tile * hp){
    emit updateHealthbar(health);
    QImage mushroom = QImage(":/mushroom.png");
    mushroom = mushroom.scaled(int(displaySize*24),int(displaySize*12));
    QPixmap mushroompix = QPixmap::fromImage(mushroom);
    for(int i = 0;i<hpPixs.size();i++){
        QGraphicsPixmapItem * local = hpPixs.at(i);
        if(mariopix->collidesWithItem(local)){
            local->setOffset(-mushroom.width()/2,-mushroom.height()/2);
            QTransform transform;
            transform.translate(0, mushroom.height()/2);
            local->setTransform(transform);
            local->setPixmap(mushroompix);
        }
    }
}

void Gview::drawCurrentBest(vector<tile_t*> path){
    //qDeleteAll(path);
    QBrush brush(Qt::SolidPattern);
    QPen pen(Qt::SolidLine);
    pen.setWidth(3);
    brush.setColor(QColor(255,0,0));
    pen.setColor(QColor(255,0,0));
    for(unsigned long i = 0; i<path.size();i++){
        Tile * t = path[i]->t;
        QGraphicsRectItem * rect = scene->addRect(t->getXPos()*displaySize,t->getYPos()*displaySize,displaySize,displaySize,pen,brush);
        bestPath.push_back(rect);
    }
}

void Gview::changeHealthbar(float health){
    cout << "health: " << health<< endl;
    ui->healthbar->setValue(health);
}

void Gview::on_startGenetic_clicked()
{
    emit geneticTrigger();
}


