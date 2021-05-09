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

bool collectableObj::goToFreeRandomPosistion(bodyPart* head)
{
	if(head == nullptr)	//changing position to random, and dont care where is snake
	{
	  srand (time(NULL));

	  this-> posX= rand() % GRID_SIZE_X;
	  this-> posY= rand() % GRID_SIZE_Y;
	}
	else	//changins position to only not ocupated by snake
	{
		//todo: find free coords
		
		return true;	//if was able to change position
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


