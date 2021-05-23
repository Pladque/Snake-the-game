#pragma once
int snakeSpeed = 7;     //lower is faster
const int GRID_SIZE_X = 30;
const int GRID_SIZE_Y = 30;
int goldenAppleProbability = 10; //lower == more golden apples

int goldenAppleSizeBonus = 2;
int goldenAppleScoreBonus = 5;

int redAppleSizeBonus = 1;
int redAppleScoreBonus = 1;

//Main game settings, changing them will take effects in whole game
//EnterMenu() functions modify/retrieves them in order to affect the game
bool enemySnakePresent = true;
bool poisonedAppleOn = true;
bool gameMusicOn = true;
int difficulty = 1;			//0 - easy, 1 - normal, 2 - hard, 3 - titan
int boardNumber = 0;		//0 - clear board, 1 - corners, 2 - corners_in_distance, 3 - few_ways_out_of_a_board
int amountOfHealthes = 3; //This variable changes its value, depending on the difficulty
//eocf

const int scoreBarHeight = 64;
