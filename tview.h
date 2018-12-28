#ifndef TVIEW_H
#define TVIEW_H
#include <world_global.h>
#include <world.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "gview.h"

using namespace std;

class Tview : public Gview
{
   Q_OBJECT
public:
    Tview();
    //~Tview() Q_DECL_OVERRIDE;
    int terminalMapSize = 15;
    void drawCharacters();
    vector<string> characters;
    int worldColumns;
    int worldRows;
    int protagonistIndex=0;
    int healthPercentage = 100;
    int energyPercentage = 100;
    string prevChar = "";
    int centerX = -1;
    int centerY;
    int browserX;
    int browserY;
    string prevBrowserChar = "";


    public slots :
         void updateProtagonist(int x, int y);
         void displayBowser(int x, int y);
         void updatePoisonedTiles(int x, int y, int r);
         void changeHealthbar(int healthPercentage);
         void changeEnergybar(int energyPercentage);
         void setup(vector<unique_ptr<Tile>> & tiles, vector<unique_ptr<Tile>> & healthPacks, vector<unique_ptr<Enemy>> & enemies, int columns, int rows);
         void displayEnd(bool won);
};

#endif // TVIEW_H
