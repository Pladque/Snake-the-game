
#ifndef SNAKE2_H
#define SNAKE2_H

#include "snake.hpp"

bodyPart::bodyPart()
{
    this->x = 0;
    this->y = 0;
    this->next = nullptr;
}

bodyPart::bodyPart(unsigned short  newX, unsigned short  newY, bodyPart* newNext, bool isVisible = true)
{
    this->x = newX;
    this->y = newY;
    this->next = newNext;
    this->isVisible = isVisible;
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

bool Snake::returnBack() {
    bodyPart* toDel = head;
    bodyPart* temp = head;
    bool isFound = false;
    while(temp) {
        if(temp->next && !temp->next->isVisible) {
            temp->next->isVisible = true;
            isFound = true;
            break;
        }
        temp = temp->next;
    }
    if(isFound){
        head = head->next;
        delete toDel;
    }
    return isFound;
}

//updated once per frame postion updater
bool Snake::move(BoardReader* wallsCotainer, bodyPart* head2)
{
    bodyPart previousPositionOfBodyPart = bodyPart(*head);

    //moving head
    if(this->direction == Direction::up)
    {
        if(head->y - 1 >= scoreBarHeight / 32) //able to move up
        {
            head->y -= 1;
        }
        else    //teleport head to other side of grid
        {
            head->y = GRID_SIZE_Y + scoreBarHeight / 32 - 1;
        }
    }
    else if(direction ==  Direction::down)
    {
        if(head->y + 1 < GRID_SIZE_Y + scoreBarHeight / 32) //able to move down
        {

            head->y += 1;
        }
        else //teleport head to other side of grid
        {
            head->y = scoreBarHeight / 32;

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
        if(currBodyPart->isVisible){
            if((currBodyPart->x == head->x  &&  currBodyPart->y == head->y)){
                if(currBodyPart->next != nullptr && currBodyPart->next->isVisible) {
                    validMove = false;
                }
            }
        }
        
        bodyPart temp = bodyPart(*currBodyPart);
        currBodyPart->copyPos(previousPositionOfBodyPart);
        previousPositionOfBodyPart = temp;

        currBodyPart = currBodyPart->next;
    }
    bodyPart* temp2 = head2;
    while(temp2) {
        if(head->x == temp2->x && head->y == temp2->y) {
            validMove = false;
        }
        temp2 = temp2->next;
    }

    if(!validMove)
        return false;

    wall* currWall = wallsCotainer->wallHead;

    while(currWall!= nullptr)
    {
        if((currWall->getX() == head->x  &&  currWall->getY() == head->y) 
        && (currWall->getNext() != nullptr))
            validMove = false;

        currWall = currWall->getNext();
    }

    return validMove;
}

void Snake::grow(bool isVisible = true)
{

    bodyPart* currBodyPart = head;
    
    while(currBodyPart->next && currBodyPart->next->isVisible)
    {
        
        currBodyPart = currBodyPart->next;
    }
    
    //not sure if it is enough to make it work
    currBodyPart->next = new bodyPart(currBodyPart->x, currBodyPart->y, currBodyPart->next, isVisible);
    
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
    bodyPart* newLastBodyPart = head;
    while(currBodyPart->next && currBodyPart->next->isVisible)
    {
        newLastBodyPart = currBodyPart;
        currBodyPart = currBodyPart->next;
    }
    
    newLastBodyPart->next = currBodyPart->next;
    
    
    if(currBodyPart == head){
        delete currBodyPart;
        head = nullptr;
    }else {
        delete currBodyPart;
    }
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

int Snake::getLen()
{
    int counter =  0;

    bodyPart* currBodyPart = head;
    
    while(currBodyPart->next && currBodyPart->next->isVisible)
    {
        
        currBodyPart = currBodyPart->next;
        counter++;
    }

    return counter;
}

 void Snake::setHead(bodyPart* newHead)
 {
     this->head = newHead;
 }

#endif