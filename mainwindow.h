#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//from library
#include <world.h>
#include <world_global.h>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QtCore>
#include <QtGui>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    World * world;
    double displaySize = 1.5;
     QGraphicsScene * scene;
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
