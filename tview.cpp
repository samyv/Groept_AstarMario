#include "tview.h"
#include "math.h"

Tview::Tview(vector<unique_ptr<Tile>> greyTiles, vector<unique_ptr<Enemy>> enemies, unique_ptr<Protagonist> protagonist, int columns, int rows)
{
    int xPosition = protagonist->getXPos();
    int yPosition = protagonist->getYPos();
    int xStart = xPosition - terminalMapSize;
    int yStart = yPosition - terminalMapSize;
    int xEnd = xPosition + terminalMapSize;
    int yEnd = yPosition + terminalMapSize;
    int vectorSize = int(pow(2*terminalMapSize, 2));
    //characters has all special characters set for displayed tiles
    vector<string> characters(vectorSize, "");
    int protagonistIndex = (2*terminalMapSize*(yPosition-yStart) + xPosition-xStart);
    //set protagonist character
    characters.at(protagonistIndex) = "P";

    //set characters for both enemies as grey tiles (which can not be crossed by the protagonist)
    for(unsigned long i =0; i <enemies.size();i++){
        int x = enemies.at(i)->getXPos();
        int y = enemies.at(i)->getYPos();
        if(xEnd >= x >= xStart && yEnd >= y >= yStart){
            int index = (2*terminalMapSize*(y-yStart) + x-xStart);
            characters.at(index) = "E";
        }
    }
    for(unsigned long i =0; i <greyTiles.size();i++){
        int x = greyTiles.at(i)->getXPos();
        int y = greyTiles.at(i)->getYPos();
        if(xEnd >= x >= xStart && yEnd >= y >= yStart){
            int index = (2*terminalMapSize*(y-yStart) + x-xStart);
            characters.at(index) = "O";
        }
        int yEnd = yPosition + terminalMapSize;

        //making sure map follows protagonist and when reaching limits of the worldmap that you can see the protagonist move
        if(xStart < 1){
            xStart = 0;
        } else if (xEnd > columns){
            xStart = columns - 2*terminalMapSize;
            xEnd = columns;
        }

        if(yStart < 1){
            yStart = 0;
        } else if (yEnd > rows){
            yStart = rows - 2*terminalMapSize;
            yEnd = rows;
        }

    }

    //drawing the terminal view per column, per row
    for(int i =0; i < 2*terminalMapSize;i++){
        for(int j =0; j < terminalMapSize;j++){
            cout << "+---+";
        }
        cout <<""<< endl;
        for(int j =0; j < terminalMapSize;j++){
            int charIndex = i*2*terminalMapSize + j;
            cout << "| "<< characters.at(charIndex)<<" |";
        }
        cout <<""<< endl;
    }
}
