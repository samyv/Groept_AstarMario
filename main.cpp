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


    try {
        return a.exec();
    } catch (std::bad_alloc& ba) {
        cerr << "BAD ALLOC " << ba.what() << endl;
    }
}
