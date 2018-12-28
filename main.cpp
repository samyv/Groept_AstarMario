#include "game.h"
#include "ui_mainwindow.h"
#include "gview.h"
#include "game.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <iostream>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    Gview gview;
    Game game(&gview);


    return a.exec();
}


