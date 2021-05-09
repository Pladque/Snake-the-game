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

int field[GRID_SIZE_Y][GRID_SIZE_X];

bool collectableObj::goToFreeRandomPosistion(bodyPart* head){
    srand (time(NULL));
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


