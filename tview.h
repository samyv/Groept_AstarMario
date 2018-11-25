#ifndef TVIEW_H
#define TVIEW_H
#include <world_global.h>
#include <world.h>
#include <iostream>
#include <vector>

using namespace std;

class Tview
{
public:
    Tview(vector<unique_ptr<Tile>> tiles, vector<unique_ptr<Enemy>> enemies, unique_ptr<Protagonist> protagonist, int columns, int rows);
    int terminalMapSize = 25;
};

#endif // TVIEW_H
