#include <string>
#include "snake.h"

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
	bool goToFreeRandomPosistion(bodyPart* head = nullptr);
	
};