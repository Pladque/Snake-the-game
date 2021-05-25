#pragma once
#include<vector>
#pragma once
#include "../../collectableObj.h"
#include "../../snake.cpp"


class saveMeneger
{
public:
    bool validSave;
    int score;
    Snake snake1;
    Direction snake1Dir;
    std::vector<collectableObj> apples;
    std::string boardName;
    int difficulty;
    Snake snake2; 
    Direction snake2Dir;
    bool multiplayer;
    int amountOfHealthes;

    saveMeneger()
    {
        this->validSave = false;
    }

    void saveAll(int score,Snake snake1, Direction snake1Dir, std::vector<collectableObj> apples,
                    std::string boardName, int difficulty, Snake snake2,Direction snake2Dir, bool multiplayer,
                     int amountOfHealthes)
    {
        this->validSave = true;

        this-> score =score;
        this->snake1 = snake1;
        this-> snake1Dir=  snake1Dir;
        this-> apples=  apples;
        this-> boardName=  boardName;
        this-> difficulty=  difficulty;
        this->snake2= snake2 ;
        this-> snake2Dir=  snake2Dir;
        this->multiplayer=multiplayer;
        this->amountOfHealthes = amountOfHealthes;
    }

};
