#pragma once
#include "consts.cpp"
class bodyPart
{
public:
    unsigned short x;
    unsigned short y;
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
    bool move();    //false if collided with own body
    Snake();
    Snake(unsigned short startX, unsigned  short startY);
    void grow();
    void fade();
    Direction getDirection();
    void changeDirection(Direction newDir);
    bodyPart* getHead();
    ~Snake();
};
