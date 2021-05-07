#include "collectableObj.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

/*
class collectableObj
{
private:
	int scoreBonus;
	int sizeBonus;
	std::string name;
	int posX;
	int posY;
	
public:
	collectableObj(std::string newName, int posX, int posY, int newScoreBonus, int newSizeBonus);
	std::string getName();
	int getSizeBonus();
	int getScoreBonus();
	
}

*/

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

bool collectableObj::bool goToFreeRandomPosistion(bodyPart* head)
{
	if(head == nullptr)	//changing position to random, and dont care where is snake
	{
	  srand (time(NULL));

	  int posX= rand() % GRID_SIZE_X + 1;
	  int posY= rand() % GRID_SIZE_Y + 1;
	}
	else	//changins position to only not ocupated by snake
	{
		//todo: find free coords
		
		return true;	//if was able to change position
	}
	
	return false; //wasnt able change posisiton
}


