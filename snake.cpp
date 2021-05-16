
#include "snake.hpp"

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
    prevx = -1;
    prevy = -1;
    prev2x = -1;
    prev2y = -1;
    prev3x = -1;
    prev3y = -1;
}

bodyPart::bodyPart(const bodyPart& toCopy)
{
    this->x = toCopy.x;
    this->y = toCopy.y;
    this->prevx = toCopy.prevx;
    this->prevy = toCopy.prevy;
    this->prev2x = toCopy.prev2x;
    this->prev2y = toCopy.prev2y;
    this->prev3x = toCopy.prev3x;
    this->prev3y = toCopy.prev3y;
    this->next = toCopy.next;
}

void bodyPart::copyPos(bodyPart positionSource)
{
    this->x = positionSource.x;
    this->y = positionSource.y;
    this->prevx = positionSource.prevx;
    this->prevy = positionSource.prevy;
    this->prev2x = positionSource.prev2x;
    this->prev2y = positionSource.prev2y;
    this->prev3x = positionSource.prev3x;
    this->prev3y = positionSource.prev3y;
}

Snake::Snake(unsigned short startX, unsigned  short startY)
{
    this->head = new bodyPart(startX, startY, nullptr);

}

bool Snake::returnBack() {
    bodyPart* temp = head;
    while(temp) {
        if(temp->prevx == -1 || temp->prevy == -1){
            return false;
        }
        temp->x = temp->prevx;
        temp->y = temp->prevy;
        temp->prevy = temp->prev2y;
        temp->prevx = temp->prev2x;
        temp->prev2y = temp->prev3y;
        temp->prev2x = temp->prev3x;
        temp->prev3y = -1;
        temp->prev3x = -1;
        temp = temp->next;
    }
    return true;
}

//updated once per frame postion updater
bool Snake::move(BoardReader* wallsCotainer)
{
    bodyPart previousPositionOfBodyPart = bodyPart(*head);

    //moving head
    if(this->direction == Direction::up)
    {
        
        head->prev3y = head->prev2y;
        head->prev2y = head->prevy;
        head->prevy = head->y;
        head->prev3x = head->prev2x;
        head->prev2x = head->prevx;
        head->prevx = head->x;
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
        head->prev3y = head->prev2y;
        head->prev2y = head->prevy;
        head->prevy = head->y;
        head->prev3x = head->prev2x;
        head->prev2x = head->prevx;
        head->prevx = head->x;
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
        head->prev3y = head->prev2y;
        head->prev2y = head->prevy;
        head->prevy = head->y;
        head->prev3x = head->prev2x;
        head->prev2x = head->prevx;
        head->prevx = head->x;
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
        head->prev3y = head->prev2y;
        head->prev2y = head->prevy;
        head->prevy = head->y;
        head->prev3x = head->prev2x;
        head->prev2x = head->prevx;
        head->prevx = head->x;
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
        if((currBodyPart->x == head->x  &&  currBodyPart->y == head->y) && (currBodyPart->next != nullptr))
            validMove = false;

        bodyPart temp = bodyPart(*currBodyPart);
        currBodyPart->copyPos(previousPositionOfBodyPart);
        previousPositionOfBodyPart = temp;

        currBodyPart = currBodyPart->next;
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
    bodyPart* newLastBodyPart = head;
    while(currBodyPart->next)
    {
        newLastBodyPart = currBodyPart;
        currBodyPart = currBodyPart->next;
    }

    newLastBodyPart->next = nullptr;
    
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
