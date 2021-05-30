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
    // moves snakes body in snake.direction
    bool move(BoardReader* wallsCotainer, bodyPart* head2);    //false if collided with own body or wall
    Snake();
    //creates snake at given coords
    Snake(unsigned short startX, unsigned  short startY);
    // grow snake by Onepiece 
    // isVisible - if you want to grow visible, or invisible part
    void grow(bool isVisible);
    // fades snake
    void fade();
    bool returnBack();
    Direction getDirection();
    void changeDirection(Direction newDir);
    bodyPart* getHead();
    ~Snake();
    int getLen();
    void setHead(bodyPart* newHead);
    Snake& operator=(Snake& snake);
};

