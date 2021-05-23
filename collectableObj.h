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
	
public:
	collectableObj(std::string newName, int posX, int posY, int newScoreBonus, int newSizeBonus, bool isPoisoned);
	std::string getName();
	int getSizeBonus();
	int getScoreBonus();
	bool goToFreeRandomPosistion(wall* firstWall, bodyPart* head = nullptr, collectableObj** objects = nullptr);

	int getPosX();
	int getPosY();

	bool getIsGolden();
	bool getIsPoisoned();
	void makePosion();

	void setAsGolden();
	
};
