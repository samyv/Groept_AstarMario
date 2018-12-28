#ifndef VIEW_H
#define VIEW_H
#include <iostream>
#include <world_global.h>
#include <world.h>
#include "model.h"
#include <vector>

using namespace std;
class view/*: public QObject*/
{
//Q_OBJECT
public:
    view();
/*    virtual ~view();
public slots:
    virtual void updateProtagonist(int x, int y);
    virtual void updatePoisonTiles(int x, int y, int r);
    virtual void changeEnergybar(int energy);
    virtual void changeHealthbar(int); */
};


#endif // VIEW_H
