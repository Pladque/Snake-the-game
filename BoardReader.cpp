#pragma once
#include "settings.cpp"
#pragma once
#include <string>

#include <fstream>
#include <iostream>

#include <dirent.h>
#include <sys/types.h>
#include <sstream>
#include <vector>

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

    std::vector<std::string> list_dir(const char *path) {
    struct dirent *entry;
        DIR *dir = opendir(path);
        
        std::stringstream ss;
        
        while ((entry = readdir(dir)) != NULL) {
        ss << entry->d_name << std::endl;
        }
        closedir(dir);

        std::vector<std::string> allFIles;
        std::string temp;
        while(ss>>temp)
            allFIles.push_back(temp);


        return allFIles;
    }

    void CreateSpritesForAll(std::string boardsTXTPath)
    {
        //for (const auto & file : fs::recursive_directory_iterator(boardsTXTPath.c_str()))
            //std::cout << file.path() << std::endl;

        for (const auto & boardTxt : list_dir(boardsTXTPath.c_str()))
        {
            readBoard(BOARDS_PATH + boardTxt);
            std::ofstream newBoardPPM;

            newBoardPPM.open(BOARDS_IMGS_PATH + boardTxt + ".ppm");

            newBoardPPM<<"P3 "<<GRID_SIZE_X<<" "<<GRID_SIZE_Y<<" 255"<<'\n';
            
            std::string black = "0 0 0";
            std::string white = "255 255 255";

            for(int x = 0; x< GRID_SIZE_X; x++)
            {
                for(int y = 2; y<= GRID_SIZE_Y + 1; y++)
                {
                    wall* curr = this->wallHead;

                    bool found = false;
                    while(curr)
                    {
                        if(curr->getX() == x &&  curr->getY() == y)
                        {
                            found = true;
                            break;
                        }

                        curr = curr ->getNext();
                    }

                    if(found)
                        newBoardPPM<<black<<" ";
                    else
                        newBoardPPM<<white<<" ";

                }

                newBoardPPM<<'\n';
            }
            
            
            
            wall* temp = wallHead;
            wall* temp2;
            
            while(temp) {
                temp2 = temp->getNext();
                delete temp;
                temp = temp2;
            }
            wallHead = nullptr;
            
        }
        //*/
    }
};
