#include "tview.h"
#include "math.h"
#include "game.h"

Tview::Tview(vector<unique_ptr<Tile>> tiles, vector<unique_ptr<Tile>> healthPacks, vector<unique_ptr<Enemy>> enemies, int columns, int rows)
{
    protagonistIndex = 0;
    worldColumns = columns;
    worldRows = rows;
    double vectorSize = columns*rows;
    //characters has all special characters set for displayed tiles
    //vector<string> characters(vectorSize, "");
    characters.resize(vectorSize);

    //set characters for both enemies as grey tiles (which can not be crossed by the protagonist)
    for(unsigned long i =0; i <enemies.size();i++){
        int x = enemies.at(i)->getXPos();
        int y = enemies.at(i)->getYPos();
        int index = (columns*y + x);
        characters.at(index) = "E";
    }

    for(unsigned long i =0; i <healthPacks.size();i++){
        int x = healthPacks.at(i)->getXPos();
        int y = healthPacks.at(i)->getYPos();
        int index = (columns*y + x);
        characters.at(index) = "H";
    }

    for(unsigned long i =0; i <tiles.size();i++){
        int x = tiles.at(i)->getXPos();
        int y = tiles.at(i)->getYPos();
        int index = (columns*y + x);
        if(tiles.at(i)->getValue() != INFINITY || tiles.at(i)->getValue() < 1.0f){
            if(characters.at(index).empty()){
                float value = 244 + 10 * tiles.at(i)->getValue();
                int cal = int(value);
                characters.at(index) = "\x1b[48;5;"+ to_string(cal) + "m";
            }

        } else {
            characters.at(index) = "O";
        }
    }
}

void Tview::updateProtagonist(int x, int y){
    int xStart = x - terminalMapSize;
    int yStart = y - terminalMapSize;
    int xEnd = x + terminalMapSize;
    int yEnd = y + terminalMapSize;


    protagonistIndex = (worldColumns*y + x);
    if(!characters.at(protagonistIndex).empty()){
        characters.at(protagonistIndex) = "";
    }
    //set protagonist character
    characters.at(protagonistIndex) = "P";

    //making sure map follows protagonist and when reaching limits of the worldmap that you can see the protagonist move
    if(xStart < 1){
        xStart = 0;
    } else if (xEnd > worldColumns){
        xStart = worldColumns - 2*terminalMapSize;
        xEnd = worldColumns;
    }

    if(yStart < 1){
        yStart = 0;
    } else if (yEnd > worldRows){
        yStart = worldRows - 2*terminalMapSize;
        yEnd = worldRows;
    }

    //drawing the terminal view per column, per row
    for(int i =0; i < 2*terminalMapSize;i++){
        cout << "\x1b[0m";
        for(int j =0; j < 2*terminalMapSize;j++){
            int charIndex = (yStart+i)*worldColumns + j+xStart;
            string prevCharac = "";
            if(charIndex>= worldColumns){
                prevCharac = characters.at(charIndex-worldColumns);
            }
            string charac = characters.at(charIndex);
            if(charac == "O" || prevCharac=="O"){
                cout << "\x1b[40m" << "+---+" << "\x1b[0m";
            } else if(charac.length()>2){
                cout << charac << "+---+" << "\x1b[0m";
            } else if(prevCharac.length()>2){
                cout << prevCharac << "+---+" << "\x1b[0m";
            } else {
                cout << "+---+";
            }

        }
        cout <<""<< endl;
        for(int j =0; j < 2*terminalMapSize;j++){
            int charIndex = (yStart+i)*worldColumns + j+xStart;
            string charac = characters.at(charIndex);
            if(charac == "O"){
                cout << "\x1b[40m" << "| "<< charac<<" |" << "\x1b[0m";
            } else if (charac == "P"){
                cout << "\x1b[44m" <<  "|"<< " "<< charac<< " " << "|"<< "\x1b[0m";
            } else if(charac == "E"){
                cout << "\x1b[41m" <<  "|"<< " "<< charac<< " " << "|"<< "\x1b[0m";
            } else if(charac == "H"){
                cout << "\x1b[42m" <<  "|"<< " "<< charac<< " " << "|"<< "\x1b[0m";
            } else if(charac.length()>2){
                cout << charac <<  "|" << "   " << "|" <<"\x1b[0m";
            } else {
                if(charac.empty()){
                    charac = " ";
                }
                cout << "| "<< charac<<" |";
            }
        }
        cout <<""<< endl;
    }
}

void Tview::updateEnemy(unique_ptr<Enemy> enemy){
    int enemyIndex = protagonistIndex = (worldColumns*(enemy->getYPos()) + enemy->getXPos());
    characters.at(enemyIndex) = "";
}
