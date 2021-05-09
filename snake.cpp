#include "snake.hpp"
//#include "snake.h"

bodyPart::bodyPart()
{
    this->x = 0;
    this->y = 0;
    this->next = nullptr;
}

bodyPart::bodyPart(unsigned short  newX, unsigned short  newY, bodyPart* newNext)
{
    this->x = newX;
    this->y = newY;
    this->next = newNext;
}

bodyPart::bodyPart(const bodyPart& toCopy)
{
    this->x = toCopy.x;
    this->y = toCopy.y;
    this->next = toCopy.next;
}

void bodyPart::copyPos(bodyPart positionSource)
{
    this->x = positionSource.x;
    this->y = positionSource.y;
}

Snake::Snake(unsigned short startX, unsigned  short startY)
{
    this->head = new bodyPart(startX, startY, nullptr);

}

//updated once per frame postion updater
bool Snake::move()
{
    bodyPart previousPositionOfBodyPart = bodyPart(*head);

    //moving head
    if(this->direction == Direction::up)
    {
        if(head->y - 1 >= 0) //able to move up
        {
            head->y -= 1;
        }
        else    //teleport head to other side of grid
        {
            head->y = GRID_SIZE_Y - 1;
        }
    }
    else if(direction ==  Direction::down)
    {
        if(head->y + 1 < GRID_SIZE_Y) //able to move down
        {

            head->y += 1;
        }
        else //teleport head to other side of grid
        {
            head->y = 0;

        }
    }
    else if(direction ==  Direction::left)
    {
        if(head->x - 1 >= 0) //able to move left
        {
            head->x -= 1;
        }
        else //teleport head to other side of grid
        {
            head->x = GRID_SIZE_X - 1;
        }
    }
    else if(direction ==  Direction::right)
    {
        if(head->x + 1 < GRID_SIZE_X) //able to move right
        {
            head->x += 1;
        }
        else    //teleport head to other side of grid
        {
            head->x = 0;
        }
    }

    //moving rest of body
    bodyPart* currBodyPart = head->next;

    bool validMove = true;
    while(currBodyPart)
    {
        if(currBodyPart->x == head->x  &&  currBodyPart->y == head->y)
            validMove = false;

        bodyPart temp = bodyPart(*currBodyPart);
        currBodyPart->copyPos(previousPositionOfBodyPart);
        previousPositionOfBodyPart = temp;

        currBodyPart = currBodyPart->next;
    }

    return validMove;
}

void Snake::grow()
{

    bodyPart* currBodyPart = head;

    while(currBodyPart->next)
    {
        currBodyPart = currBodyPart->next;
    }

    //not sure if it is enough to make it work
    currBodyPart->next = new bodyPart(*currBodyPart);
}

void Snake::changeDirection(Direction newDir) {
    direction = newDir;
}

Direction Snake::getDirection() {
    return direction;
}

void Snake::fade()
{
    bodyPart* currBodyPart = head;
    bodyPart* newLastBodyPart;
    while(currBodyPart->next)
    {
        newLastBodyPart = currBodyPart;
        currBodyPart = currBodyPart->next;
    }

    newLastBodyPart->next = nullptr;

    delete currBodyPart;
}

bodyPart* Snake::getHead()
{
    return this->head;
}

Snake::~Snake() {
    bodyPart* currBodyPart = head;
    bodyPart* toDel;
    while(currBodyPart){
        toDel = currBodyPart;
        currBodyPart = currBodyPart->next;
        delete toDel;
    }
}
