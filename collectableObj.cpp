#include "BoardReader.cpp"
#include "collectableObj.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


collectableObj::collectableObj(std::string newName, int newPosX, int newPosY, int newScoreBonus, int newSizeBonus, bool isPoisoned = false)
{
	this->name= newName;
	this->posX = newPosX;
	this->posY = newPosY;
	this->scoreBonus = newScoreBonus;
	this->sizeBonus = newSizeBonus;
    this->isGolden = false;
    this->isPoisoned = isPoisoned;
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

void collectableObj::setAsGolden()
{
     this->isGolden = true;
            this->scoreBonus = goldenAppleScoreBonus;
            this->sizeBonus = goldenAppleSizeBonus;
}

int field[GRID_SIZE_Y + scoreBarHeight/32][GRID_SIZE_X];

bool collectableObj::goToFreeRandomPosistion(wall* firstWall, bodyPart* head, collectableObj** objects){
    srand (time(NULL));
    if(!isPoisoned){
        if(rand()%goldenAppleProbability == 0)
        {
            this->isGolden = true;
            this->scoreBonus = goldenAppleScoreBonus;
            this->sizeBonus = goldenAppleSizeBonus;
        }
        else
        {
            this->isGolden = false;
            this->scoreBonus = redAppleScoreBonus;
            this->sizeBonus = redAppleSizeBonus;
        }
    }
    
    
    
    for(int i = scoreBarHeight / 32; i < GRID_SIZE_Y + scoreBarHeight/32; i++){
        for(int j = 0; j < GRID_SIZE_X; j++) {
            field[i][j] = 0;
        }
    }
    bodyPart* temp = head;
    int counter = 0;
    while(temp) {
        counter++;
        if(temp->isVisible)
            field[temp->y][temp->x] = 1;
        temp = temp->next;
    }
    if(objects != nullptr){
        if(poisonedAppleOn) {
            if(isPoisoned) {
                field[objects[0]->getPosY()][objects[0]->getPosX()] = 1;
                counter++;
            }else {
                field[objects[1]->getPosY()][objects[1]->getPosX()] = 1;
                counter++;
            }
        }
    }
    wall* temp2 = firstWall;
    while(temp2) {
        counter++;
        field[temp2->getY()][temp2->getX()] = 1;
        temp2 = temp2->getNext();
    }
    
    if(GRID_SIZE_X*GRID_SIZE_Y - counter < 5)
        return false;//if wasnt able to change position
    
    int randomIndex = rand() % (GRID_SIZE_X*GRID_SIZE_Y - counter);
    counter = 0;
    
    for(int i = scoreBarHeight / 32; i < GRID_SIZE_Y + scoreBarHeight/32; i++) {
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

bool collectableObj::getIsPoisoned()
{
    return this->isPoisoned;
}


void collectableObj::makePosion()
{
    this->isPoisoned = true;
}


