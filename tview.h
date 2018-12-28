#ifndef TVIEW_H
#define TVIEW_H
#include <world_global.h>
#include <world.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <view.h>

using namespace std;

class Tview : public QObject/*: public view*/
{
   Q_OBJECT
public:
    Tview();
    //~Tview() Q_DECL_OVERRIDE;
    int terminalMapSize = 10;
    void drawCharacters();
    vector<string> characters;
    int worldColumns;
    int worldRows;
    int protagonistIndex;
    int healthPercentage = 20;
    int energyPercentage = 80;
    string prevChar = "";
    int centerX;
    int centerY;


    public slots :
         void updateProtagonist(int x, int y);
         void updatePoisonTiles(int x, int y, int r);
         void changeHealthbar(int healthPercentage);
         void changeEnergybar(int energyPercentage);
         void setup(vector<unique_ptr<Tile>> & tiles, vector<unique_ptr<Tile>> & healthPacks, vector<unique_ptr<Enemy>> & enemies, int columns, int rows);
};

#endif // TVIEW_H
