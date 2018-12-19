#ifndef TVIEW_H
#define TVIEW_H
#include <world_global.h>
#include <world.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

class Tview : public QObject
{
   Q_OBJECT
public:
    Tview(vector<unique_ptr<Tile>> tiles, vector<unique_ptr<Tile>> healthPacks, vector<unique_ptr<Enemy>> enemies, int columns, int rows);
    int terminalMapSize = 15;

    void updateEnemy(unique_ptr<Enemy> enemy);
    vector<string> characters;
    int worldColumns;
    int worldRows;
    int protagonistIndex;
    public slots : void updateProtagonist(int x, int y);
};

#endif // TVIEW_H
