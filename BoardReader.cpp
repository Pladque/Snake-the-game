#pragma once
#include "settings.cpp"
#pragma once
#include <string>

#include <fstream>
#include <iostream>

class wall
{
    int xPos;
    int yPos;
    wall* next;
public:

    wall(int newX, int newY)
    {
        this->xPos = newX;
        this->yPos = newY;
        next = nullptr;
    }

    int getX()
    {
        return this->xPos;
    }

    int getY()
    {
        return this->yPos;
    }

    void setNext(wall* newNext)
    {
        this->next = newNext;
    }

    wall* getNext()
    {
        return this->next;
    }
};

class BoardReader{
public:

    wall* wallHead;

    void readBoard(std::string boardName)
    {
        wallHead = new wall(-1, -1);
        std::fstream boardFile;
        boardFile.open(boardName.c_str());

        char tempCell;

        int xPosCounter = 0;
        int yPosCounter = 0;

        wall* curr = wallHead;

        //std::cout<<"here0"<<std::endl;

        while(boardFile>>tempCell)
        {
            //std::cout<<"here"<<xPosCounter<<std::endl;
            if (tempCell == 'X')
            {
                curr->setNext( new wall(xPosCounter, yPosCounter));
                curr = curr ->getNext();
                
            }

            xPosCounter++;

            if(xPosCounter == GRID_SIZE_X)
            {
                xPosCounter = 0;
                yPosCounter++;
            }
        }
    }

};