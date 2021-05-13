#pragma once
#include <string>
#pragma once
#include "snake.hpp"


class collectableObj
{
private:
	int scoreBonus;
	int sizeBonus;
	std::string name;
	int posX;
	int posY;

	//for apples
	bool isGolden;
	bool isPoisoned;
    bool prevIsGolden;
	
public:
	collectableObj(std::string newName, int posX, int posY, int newScoreBonus, int newSizeBonus);
	std::string getName();
	int getSizeBonus();
	int getScoreBonus();
	bool goToFreeRandomPosistion(wall* firstWall, bodyPart* head = nullptr);

	int getPosX();
	int getPosY();

	bool getIsGolden();
    bool getPrevIsGolden();
	bool getIsPoisoned();
	void makePosion();
	
};
