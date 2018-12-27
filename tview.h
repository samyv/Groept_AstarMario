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
    int terminalMapSize = 50;

    void updatePoisonTiles(vector<unique_ptr<Tile>> poisonTiles);
    vector<string> characters;
    int worldColumns;
    int worldRows;
    int protagonistIndex;
    int healthPercentage = 20;
    int energyPercentage = 80;
    int centerX;
    int centerY;
    /*public slots : */ void updateProtagonist(int x, int y);
    void drawCharacters();
    void updateHealth(int healthPercentage);
    void changeEnergyBar(int energyPercentage);
};

#endif // TVIEW_H
