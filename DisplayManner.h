#ifndef DISPLAYMANNER_H
#define DISPLAYMANNER_H

using namespace std;

class DisplayManner
{
public:
    DisplayManner(){}
    virtual ~DisplayManner() = default;
    virtual void displayEnemies() = 0;

};

#endif // DISPLAYMANNER_H
