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
    short prevx;
    short prevy;
    short prev2x;
    short prev2y;
    short prev3x;
    short prev3y;
    bodyPart* next;
    bodyPart();
    bodyPart(unsigned short  newX, unsigned short  newY, bodyPart* newNext);
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
    void grow();
    void fade();
    Direction getDirection();
    void changeDirection(Direction newDir);
    bodyPart* getHead();
    bool returnBack();
    ~Snake();
};
