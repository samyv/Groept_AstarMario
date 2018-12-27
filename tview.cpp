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
        if(typeid(*enemies.at(i)) == typeid(Enemy)){
            characters.at(index) = "E";
        }else{
            characters.at(index) = "B"; //those are pEnemies, "B" because of the fact they can explode (bom)
        }
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
    centerX = x;
    centerY = y;
    drawCharacters();
}

void Tview::drawCharacters(){
    int xStart = centerX - terminalMapSize;
    int yStart = centerY - terminalMapSize;
    int xEnd = centerX + terminalMapSize;
    int yEnd = centerY + terminalMapSize;


    protagonistIndex = (worldColumns*centerY + centerX);
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
            } else if(charac == "B"){
                cout << "\x1b[45m" <<  "|"<< " "<< charac<< " " << "|"<< "\x1b[0m";
            } else if(charac == "!"){
                cout << "\x1b[48;5;91m" <<  "|"<< " "<< charac<< " " << "|"<< "\x1b[0m";
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
    //healthbar and energybar
    for(int j =0; j < 2*terminalMapSize;j++){
        double healthSize = 2*terminalMapSize*healthPercentage*0.01;
        if(j<= healthSize){
            if(healthPercentage<=50 && healthPercentage>30){
                cout << "\x1b[48;5;178m" << "     "<<"\x1b[0m";
            } else if(healthPercentage<=30){
                cout << "\x1b[41m" << "     "<<"\x1b[0m";
            } else {
                cout << "\x1b[42m" << "     "<<"\x1b[0m";
            }
        } else {
            cout << "\x1b[40m" << "     "<<"\x1b[0m";
        }
    }
    cout <<""<< endl;
    for(int j =0; j < 2*terminalMapSize;j++){
        double energySize = 2*terminalMapSize*energyPercentage*0.01;
        if(j<= energySize){
            cout << "\x1b[44m" << "     "<<"\x1b[0m";
        } else {
            cout << "\x1b[40m" << "     "<<"\x1b[0m";
        }
    }
    cout <<""<< endl;
}

void Tview::updatePoisonTiles(vector<unique_ptr<Tile>> poisonTiles){
    for(unsigned long i =0; i <poisonTiles.size();i++){
        int x = poisonTiles.at(i)->getXPos();
        int y = poisonTiles.at(i)->getYPos();
        int index = (worldColumns*y + x);
        characters.at(index) = "!"; //set character for poisoned tiles
    }
    drawCharacters();
}

void Tview::updateHealth(int healthPercentage){
    this->healthPercentage = healthPercentage;
    drawCharacters();
}

void Tview::changeEnergyBar(int energyPercentage){
    this->energyPercentage = energyPercentage;
    drawCharacters();
}
