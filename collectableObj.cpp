#include "BoardReader.cpp"
#include "collectableObj.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


collectableObj::collectableObj(std::string newName, int newPosX, int newPosY, int newScoreBonus, int newSizeBonus)
{
	this->name= newName;
	this->posX = newPosX;
	this->posY = newPosY;
	this->scoreBonus = newScoreBonus;
	this->sizeBonus = newSizeBonus;
    this->isGolden = false;
    this->prevIsGolden = false;
}

std::string collectableObj::getName()
{
	return name;
}

int collectableObj::getSizeBonus()
{
	return sizeBonus;
}

int collectableObj::getScoreBonus()
{
	return scoreBonus;
}

bool collectableObj::getIsGolden()
{
    return this->isGolden;
}

bool collectableObj::getPrevIsGolden() {
    return this->prevIsGolden;
}

int field[GRID_SIZE_Y][GRID_SIZE_X];

bool collectableObj::goToFreeRandomPosistion(wall* firstWall, bodyPart* head){
    srand (time(NULL));

    if(rand()%goldenAppleProbability == 0)
    {
        this->prevIsGolden = this->isGolden;
        this->isGolden = true;
        this->scoreBonus = goldenAppleScoreBonus;
        this->sizeBonus = goldenAppleSizeBonus;
    }
    else
    {
        this->prevIsGolden = this->isGolden;
        this->isGolden = false;
        this->scoreBonus = redAppleScoreBonus;
        this->sizeBonus = redAppleSizeBonus;
    }
    
	if(head == nullptr)	//changing position to random, and dont care where is snake
	{
	  this-> posX= rand() % GRID_SIZE_X;
	  this-> posY= rand() % GRID_SIZE_Y;
	}
	else	//changins position to only not ocupated by snake
	{
        for(int i = 0; i < GRID_SIZE_Y; i++){
            for(int j = 0; j < GRID_SIZE_X; j++) {
                field[i][j] = 0;
            }
        }
        bodyPart* temp = head;
        int counter = 0;
        while(temp) {
            counter++;
            field[temp->y][temp->x] = 1;
            temp = temp->next;
        }
        
        wall* temp2 = firstWall;
        while(temp2) {
            counter++;
            field[temp2->getY()][temp2->getX()] = 1;
            temp2 = temp2->getNext();
        }
        
        if(counter == 0)
            return false;//if wasnt able to change position
        
        int randomIndex = rand() % (GRID_SIZE_X*GRID_SIZE_Y - counter);
        counter = 0;
        
        for(int i = 0; i < GRID_SIZE_Y; i++) {
            for(int j = 0; j < GRID_SIZE_X; j++) {
                if(field[i][j] == 0){
                    if(counter == randomIndex) {
                        this-> posX = j;
                        this-> posY = i;
                        return true;
                    }
                    counter++;
                }
            }
        }
        
	}
	
	return false; //wasnt able change posisiton
}

int  collectableObj::getPosX()
{
	return posX;

}

int  collectableObj::getPosY()
{
	return posY;

}


