#include "ui_mainwindow.h"
#include "gview.h"
#include "game.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <iostream>

int main(int argc, char *argv[])
  {
  QApplication a(argc, argv);
  Game game;
  Gview gview;
  gview.show();
cout << "here" << endl;



  return a.exec();
  }
