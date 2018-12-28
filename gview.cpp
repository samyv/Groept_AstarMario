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


    //setup the scene for grahpicalview
    setupScene();

    //create a Qimage for the background
    drawBackground();

    drawMarioInit();

    QImage bowser = QImage(":/bowser.png");
    bowser = bowser.scaled(int(displaySize*24*2),int(displaySize*24*2));
    QPixmap bowserPixmap = QPixmap::fromImage(bowser);
    bowserPix = new QGraphicsPixmapItem(bowserPixmap);
    bowserPix->hide();
    scene->addItem(bowserPix);
    //draw rects for each tile (not used for now)
    //    drawWorld();

    //draw a* pathfindi ng
    //    makeModel();
    //ui->graphicsView->setSceneRect(this->frameGeometry());
    //ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    //ui->graphicsView->setHorizontalScrollBar(nullptr);
    //ui->graphicsView->setVerticalScrollBar(nullptr);
    //ui->graphicsView->sizeAdjustPolicy(1);
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
    mariopix = new ProtagonistUser(protapix);
    mariopix->setFlag(QGraphicsItem::ItemIsFocusable);
    mariopix->setFocus();
    mariopix->setOffset(-mario.width()/2,-mario.height());
    mariopix->setOffset(-mario.width()/2,-mario.height());
    scene->addItem(mariopix);
}

void Gview::setupScene(){
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    ui->healthbar->setValue(100);
    ui->energybar->setValue(100);
}

void Gview::updateProtagonist(int x, int y){
    mariopix->setPos(x*displaySize,y*displaySize);
    int scale = (x - prevX >0)?1:-1;
    if(scale != 0){
        mariopix->setTransform(QTransform::fromScale(scale,1));
    }

    prevX = x;
    ui->graphicsView->centerOn(x*displaySize,y*displaySize);


    //rect fading
    QBrush brush(Qt::SolidPattern);
    QPen pen(Qt::SolidLine);
    pen.setWidth(5);
    QColor red = QColor(255,0,0);
    brush.setColor(red);

    pen.setColor(red);
    QGraphicsRectItem * rect = scene->addRect(x*displaySize,y*displaySize,displaySize,displaySize,pen,brush);
    rects.push_back(rect);
    mariopix->setZValue(1);
    int i = 0;
    for(auto &rect: rects){

        QColor q = rect->brush().color();
        QColor  * new_q= new QColor(q.red(),q.green(),q.blue());
        if(q.alpha() >= 1){
            //            cout << q.alpha() << endl;
            new_q->setAlpha(q.alpha()-0.1);
            QBrush new_B(Qt::SolidPattern);
            QPen new_P(Qt::SolidLine);
            pen.setWidth(5);
            new_B.setColor(*new_q);
            new_P.setColor(*new_q);
            //            cout << "after " << q.alpha() << endl;
            rect->setPen(new_P);
            rect->setBrush(new_B);
            if(i == 1){
                //                cout << rect->brush().color().alpha << endl;
            }
            i++;
        } else {
            //scene->removeItem(rect);
            rects.erase(remove(rects.begin(),rects.end(),rect),rects.end());
        }
    }
}

void Gview::initDisplay(vector<unique_ptr<Tile>> & tiles,vector<unique_ptr<Enemy>> & enemies,vector<unique_ptr<Tile>> & healthpacks){
    QImage goomba = QImage(":/goombaretro.png");
    goomba = goomba.scaled(int(displaySize*24),int(displaySize*24));
    QPixmap goombapix = QPixmap::fromImage(goomba);

    QImage Pe = QImage(":/PEnemy.png");
    Pe = Pe.scaled(int(displaySize*24),int(displaySize*24));
    QPixmap PePix = QPixmap::fromImage(Pe);

    QGraphicsPixmapItem * enemypix;
    for(auto &enemy : enemies){
        if(typeid (*enemy) == typeid (Enemy)){
            enemypix = new QGraphicsPixmapItem(goombapix);
        } else if(typeid (*enemy) == typeid (PEnemy)){
            enemypix = new QGraphicsPixmapItem(PePix);
        }
        enemypix->setPos(enemy->getXPos()*displaySize,enemy->getYPos()*displaySize);
        enemypix->setOffset(-goomba.width()/2,-goomba.height()/2);
        enemiesPixs.push_back(enemypix);
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

    if(ui->viewbox->currentIndex() == 0){
        //Gview
    } else {
        //Tview
    }

    if(ui->controlBox->currentIndex() == 0){
        //ai
    } else {
        //user
    }

    displaySize = ui->displaysizeval->text().toDouble();

    emit setnumbers(ui->enemiesval->value(), ui->hpval->value());

    if(ui->salesmanbox->currentIndex() == 0){
        cout << ui->salesmanbox->currentIndex() << endl;
        emit gameStart();
    } else {
        emit geneticTrigger();
    }

    emit sendSound("qrc:/sound/sm64_mario_here_we_go.wav");
}

void Gview::explodeEnemy(int x,int y){
    QLabel *gif_anim = new QLabel();
    QMovie *movie = new QMovie(":/explosion.gif", QByteArray(), this);
    new QGraphicsProxyWidget();
    gif_anim->setAttribute( Qt::WA_NoSystemBackground);
    gif_anim->setMovie(movie);
    movie->start();
    QGraphicsProxyWidget * w =scene->addWidget(gif_anim);
    emit sendSound("qrc:/sound/smw_thunder.wav");
    w->setWidget(gif_anim);
    w->setScale(0.3f);
    w->setPos(x*displaySize - 0.3f*w->size().width()/2,x*displaySize - 0.3f*w->size().height());
    eff = new QGraphicsOpacityEffect();
    w->setGraphicsEffect(eff);
    QPropertyAnimation *a = new QPropertyAnimation(eff,"opacity");
    a->setDuration(3000);
    a->setStartValue(500);
    a->setEndValue(0);
    a->setEasingCurve(QEasingCurve::OutBack);
    a->start(QPropertyAnimation::DeleteWhenStopped);
    connect(movie, &QMovie::frameChanged, this,
            [movie]()
    {
        if(movie->currentFrameNumber() == (movie->frameCount()-1))
        {
            movie->stop();
            //Explicity emit finished signal so that label **
            //can show the image instead of a frozen gif
            //Also, double check that movie stopped before emiting
            if (movie->state() == QMovie::NotRunning)
            {
                emit movie->finished();
            }
        }
    }
    );
}

void Gview::triggerHealthpack(Tile * hp){
    for(QGraphicsPixmapItem * pix : hpPixs){
        if((hp->getXPos() == pix->x()) && (hp->getYPos() == pix->y())){
            QImage mushroom = QImage(":/mushroom.png");
            mushroom = mushroom.scaled(int(displaySize*24),int(displaySize*12));
            QPixmap mushroompix = QPixmap::fromImage(mushroom);
            pix->setOffset(-mushroom.width()/2,-mushroom.height()/2);
            QTransform transform;
            cout << "trigger healthpack"<< endl;
            transform.translate(0, mushroom.height()/2);
            pix->setTransform(transform);
            pix->setPixmap(mushroompix);
            break;
        }
    }
}

void Gview::drawCurrentBest(vector<tile_t*> path){
    for(auto &rect : bestPath){
        scene->removeItem(rect.get());
    }
    QBrush brush(Qt::SolidPattern);
    QPen pen(Qt::SolidLine);
    pen.setWidth(3);
    brush.setColor(QColor(255,0,0));
    pen.setColor(QColor(255,0,0));

    for(unsigned long i = 0; i<path.size();i++){
        Tile * t = path[i]->t;
        QGraphicsRectItem * rect;
        rect = scene->addRect(t->getXPos()*displaySize,t->getYPos()*displaySize,displaySize,displaySize,pen,brush);
        unique_ptr<QGraphicsRectItem> rect_ptr(rect);
        // bestPath.push_back(rect_ptr);
    }
}

void Gview::changeHealthbar(int health){
    ui->healthbar->setValue(health);
}

void Gview::changeEnergybar(int energy){
    ui->energybar->setValue(energy);
}

void Gview::updatePoisonedTiles(int x,int y,int r)
{
    QBrush * brush = new QBrush(QColor(0,255,0,120));
    QPen greenp(Qt::green);
    //    scene->addEllipse((x-r/2)*displaySize,(y-r/2)*displaySize,r*displaySize,r*displaySize,greenp,*brush);
    scene->addRect((x-r/2)*displaySize,(y-r/2)*displaySize,r*displaySize,r*displaySize,greenp,*brush);

}

void Gview::displayBowser(int x,int y)
{
    bowserPix->show();
    bowserPix->setPos(x*displaySize,y*displaySize);
}

void Gview::enemyDead(){
    QPixmap enemyPix;
    QImage goomba = QImage(":/goombaretro.png");
    goomba = goomba.scaled(int(displaySize*24),int(displaySize*12));

    QImage pE = QImage(":/PEnemy.png");
    pE = pE.scaled(int(displaySize*24),int(displaySize*12));
    for(int i = 0;i<enemiesPixs.size();i++){
        QGraphicsPixmapItem * local = enemiesPixs.at(i);
        if(mariopix->collidesWithItem(local)){
            enemyPix = QPixmap::fromImage(goomba);
            local->setOffset(-goomba.width()/2,-goomba.height()/2);
            QTransform transform;
            transform.translate(0, goomba.height()/2);
            local->setTransform(transform);
            local->setPixmap(enemyPix);
        }
    }
}

void Gview::penemyDead(){
    QPixmap enemyPix;
    QImage pE = QImage(":/PEnemy.png");
    pE = pE.scaled(int(displaySize*24),int(displaySize*12));
    for(auto &epix : enemiesPixs){
        if(epix->collidesWithItem(mariopix,Qt::ItemSelectionMode::IntersectsItemShape)){
            QBrush brush(Qt::SolidPattern);
            QPen pen(Qt::SolidLine);
            pen.setWidth(3);
            QColor green = QColor(0,255,0);
            green.setAlpha(100);
            brush.setColor(green);
            pen.setColor(green);
            //scene->addEllipse(epix->pos().x()-displaySize*200/2,epix->pos().y()-displaySize*200/2,displaySize*200,displaySize*200,pen,brush);
            cout << "penemy" << endl;
            emit poisonExplosion(epix->x(),epix->y());
            enemyPix = QPixmap::fromImage(pE);
            epix->setOffset(-pE.width()/2,-pE.height()/2);
            QTransform transform;
            transform.translate(0, pE.height()/2);
            epix->setTransform(transform);
            epix->setPixmap(enemyPix);
        }
    }
}

void Gview::collisonDetect()
{
    for(auto &epix : enemiesPixs){
        if(epix->collidesWithItem(mariopix,Qt::ItemSelectionMode::IntersectsItemShape)){
            int x = epix->pos().x()/displaySize;
            int y = epix->pos().y()/displaySize;
            emit enemyDeadUser(x,y);
        }
    }
    for(auto &hpix : hpPixs){
        if(hpix->collidesWithItem(mariopix,Qt::ItemSelectionMode::IntersectsItemShape)){
            int x = hpix->pos().x()/displaySize;
            int y = hpix->pos().y()/displaySize;
            emit hpUser(x,y);
        }
    }
}


void Gview::on_startGenetic_toggled(bool checked)
{
    if(checked){
        ui->startGenetic->setText("STOPG");
        emit geneticTrigger();
    } else{
        ui->startGenetic->setText("STARTG");
        emit geneticStop();
    }

}

ProtagonistUser *Gview::getMariopix() const
{
    return mariopix;
}

void Gview::drawPoisoned(qreal x,qreal y){
    QBrush brush(Qt::SolidPattern);
    QPen pen(Qt::SolidLine);
    pen.setWidth(3);
    brush.setColor(QColor(0,255,0));
    pen.setColor(QColor(0,255,0));
    scene->addRect(x*displaySize,y*displaySize,displaySize,displaySize,pen,brush);
}




void Gview::on_enemiesval_sliderMoved(int position)
{
    ui->enemiessliderval->setText(QString::number(position));
}

void Gview::on_hpval_sliderMoved(int position)
{
    ui->healthsliderval->setText(QString::number(position));
}

void Gview::on_horizontalSlider_sliderMoved(int position)
{
    ui->speedsliderval->setText(QString::number(position));
}
