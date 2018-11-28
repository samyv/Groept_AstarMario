#include "ui_mainwindow.h"
#include "gview.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>


int main(int argc, char *argv[])
  {
  QApplication a(argc, argv);
  Gview w;
  w.show();

  return a.exec();
  }
