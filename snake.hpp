#pragma once
#include "consts.cpp"
#pragma once
#include "settings.cpp"
#pragma once
#include "BoardReader.cpp"



class bodyPart
{
public:
    unsigned short x;
    unsigned short y;
    bool isVisible;
    bodyPart* next;
    bodyPart();
    bodyPart(unsigned short  newX, unsigned short  newY, bodyPart* newNext, bool isVisible);
    bodyPart(const bodyPart& toCopy);
    void copyPos(bodyPart positionSource);

};

class Snake{
private:
    int length;
    bodyPart* head;
    Direction direction;
    

public:
    bool move(BoardReader* wallsCotainer);    //false if collided with own body
    Snake();
    Snake(unsigned short startX, unsigned  short startY);
    void grow(bool isVisible);
    void fade();
    bool returnBack();
    Direction getDirection();
    void changeDirection(Direction newDir);
    bodyPart* getHead();
    ~Snake();
};
