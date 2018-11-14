#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>


int main(int argc, char *argv[])
  {
  QApplication a(argc, argv);
  MainWindow w;
  w.showFullScreen();
  return a.exec();
  }
