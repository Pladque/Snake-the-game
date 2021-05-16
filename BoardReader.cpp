#pragma once
#include "settings.cpp"
#pragma once
#include <string>

#include <fstream>
#include <iostream>

#include <filesystem>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;


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
        yPosCounter = scoreBarHeight / 32;
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
        curr->setNext(nullptr);
    }
    
    ~BoardReader() {
        wall* temp = wallHead;
        wall* temp2;
        
        while(temp) {
            temp2 = temp->getNext();
            delete temp;
            temp = temp2;
        }
        wallHead = nullptr;
    }

    void CreateSpritesForAll(std::string boardsTXTPath)
    {
        /*
        for (const auto & boardTxt : fs::directory_iterator(boardsTXTPath))
        {
            readBoard(boardTxt.path());
            std::fstream newBoardPPM;

            std::string boardPPMFULLName = boardTxt.path();

            std::string boardPPMName = boardTxt.path();

            for(int i = 0; i< boardPPMFULLName.length(); i++)
            {
                //saving chars that are not path 
                if(i>=boardsTXTPath.length())
                    boardPPMName += boardPPMFULLName[i];
            }

            newBoardPPM.open(BOARDS_IMGS_PATH + boardPPMName, std::ios::out);

            newBoardPPM<<"P "<<GRID_SIZE_X<<" "<<GRID_SIZE_Y<<" 255"<<'\n';

            std::string white = "0 0 0";
            std::string black = "255 255 255";

            for(int x = 0; x< GRID_SIZE_X; x++)
            {
                for(int y = 0; y< GRID_SIZE_Y; y++)
                {
                    wall* curr = this->wallHead;

                    bool found = false;
                    while(curr)
                    {
                        if(curr->getX() == x &&  curr->getY())
                        {
                            found = true;
                        }
                    }

                if(found)
                    newBoardPPM<<black<<"\t";
                else
                    newBoardPPM<<white<<"\t";

                }

                newBoardPPM<<'\n';
            }

            ////
            wall* temp = wallHead;
            wall* temp2;
            
            while(temp) {
                temp2 = temp->getNext();
                delete temp;
                temp = temp2;
            }
            wallHead = nullptr;
            ///
        }
        */
    }
};
