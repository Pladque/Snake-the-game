#include "game.cpp"

/*
 * MENU	
 */ 

#define MULT 2
#define REACT_X 25.f
#define TEXT_X 26.f
#define REACT_WIDTH 250.f
#define REACT_HEIGHT 40.f
#define CHAR_SIZE 20.f
#define SPACING -60.f
#define MOUSE_CORRECTION 0.f


//DO NOT MODIFY/DELETE!!!
bool entered_settings = 0;	//value to determine if settings should be displayed on the screen
bool stay_in_menu = 1;		//may be used to determine if cotrol should stay in main Menu,
							//otherwise to move on with logic

int EnterMenu()
{
	BoardReader br;
	br.CreateSpritesForAll(BOARDS_PATH);

	//Main function -> depending on its return, game would start or not
	std::string snakes_count = "1";
	std::string difficulty_level;
	std::string allowedPoisonedApple;
	std::string music_on_off;
	extern int snakeSpeed;
	extern bool poisonedAppleOn;
	extern bool gameMusicOn;
	
	if (enemySnakePresent == false)
	{
		snakes_count = "1";
	}
	else
	{
		snakes_count = "2";
	}
	if (poisonedAppleOn == false)
	{
		//
		allowedPoisonedApple = "off";
	}
	else
	{
		//
		allowedPoisonedApple = "on";
	}
		
	if (difficulty == 0)
	{
		difficulty_level = "easy";
	}
	else if (difficulty == 1)
	{
		difficulty_level = "normal";
	}
	else if (difficulty == 2)
	{
		difficulty_level = "hard";
	}
	else
	{
		difficulty_level = "titan";
	}
		
	if (gameMusicOn == true)
	{
		music_on_off = "on";
	}
	else
	{
		music_on_off = "off";
	}
    
	sf::Vector2i localMousePosition;	//Variable holding mouse position;
	sf::Texture menuTexture;
	if (!menuTexture.loadFromFile("./Assets/Graphic/mainSnake2.jpg"))
		return -1;
	sf::Texture boardTexture;
	sf::Music menuMusic;
	if (!menuMusic.openFromFile(SOUNDS_PATH + "menuMusic.ogg"))
        return -1;
	sf::RenderWindow menuWindow(sf::VideoMode(1280, 960), "SNAKE_2D");	//resolution may be changed, after every other thing works
																		//as expected
	sf::Sprite menuSprite(menuTexture);
	sf::Sprite boardSprite(boardTexture);
	sf::Event menuEvent;
	sf::Font menuFont;
    if (!menuFont.loadFromFile("./Assets/Fonts/JosefinSans-SemiBoldItalic.ttf"))
        return -1;
	sf::Text mainText, windStartGame, windQuitGame, windOptions, windTryAgain, windResign;	//text, displayed  
	sf::Text windDifficultyLevel, windPoisonedFruit, windMusic, windReturnFromOptions;		//in consecutive windows
	sf::Text windSelectGameMode;
    sf::Text arrowRight;
    sf::Text arrowLeft;
	
													
	sf::RectangleShape rectangleBasic(sf::Vector2f(REACT_WIDTH, REACT_HEIGHT));
    rectangleBasic.setFillColor(sf::Color(20, 100, 150));
    rectangleBasic.setPosition(REACT_X, 205.f + SPACING);
    rectangleBasic.setOutlineThickness(5.f);
	rectangleBasic.setOutlineColor(sf::Color(250, 150, 100));
	
	sf::RectangleShape rectangleBasic2(sf::Vector2f(REACT_WIDTH, REACT_HEIGHT));
    rectangleBasic2.setFillColor(sf::Color(20, 100, 150));
    rectangleBasic2.setPosition(REACT_X, 268.f + SPACING);		
    rectangleBasic2.setOutlineThickness(5.f);
	rectangleBasic2.setOutlineColor(sf::Color(250, 150, 100));
    
    sf::RectangleShape rectangleBasic3(sf::Vector2f(REACT_WIDTH, REACT_HEIGHT));
    rectangleBasic3.setFillColor(sf::Color(20, 100, 150));
    rectangleBasic3.setPosition(REACT_X, 331.f + SPACING);		
    rectangleBasic3.setOutlineThickness(5.f);
	rectangleBasic3.setOutlineColor(sf::Color(250, 150, 100));		
	
	sf::RectangleShape rectangleBasic4(sf::Vector2f(REACT_WIDTH, REACT_HEIGHT));
    rectangleBasic4.setFillColor(sf::Color(20, 100, 150));
    rectangleBasic4.setPosition(REACT_X, 394.f + SPACING);		
    rectangleBasic4.setOutlineThickness(5.f);
	rectangleBasic4.setOutlineColor(sf::Color(250, 150, 100));	
	
	sf::RectangleShape rectangleBasic5(sf::Vector2f(REACT_WIDTH, REACT_HEIGHT));
    rectangleBasic5.setFillColor(sf::Color(20, 100, 150));
    rectangleBasic5.setPosition(REACT_X, 457.f + SPACING);		
    rectangleBasic5.setOutlineThickness(5.f);
	rectangleBasic5.setOutlineColor(sf::Color(250, 150, 100));		
	
	sf::RectangleShape rectangleBoard(sf::Vector2f(REACT_WIDTH, REACT_WIDTH));
    rectangleBoard.setFillColor(sf::Color(255, 255, 255));
    rectangleBoard.setPosition(REACT_X, 520.f + SPACING);		
    //rectangleBasic5.setOutlineThickness(5.f);
	//rectangleBasic5.setOutlineColor(sf::Color(250, 150, 100));					
													
	mainText.setFont(menuFont);
	mainText.setString("SNAKE 2D");
	mainText.setCharacterSize(38);
	mainText.setFillColor(sf::Color::Red);
	mainText.setPosition(570.f, 5.f);
	mainText.setStyle(sf::Text::Bold);
	
	windStartGame.setFont(menuFont);
	windStartGame.setString("New game: press 'P'");
	windStartGame.setCharacterSize(CHAR_SIZE);
	windStartGame.setFillColor(sf::Color::Black);
	windStartGame.setPosition(TEXT_X, 209.f + SPACING);
	
	windOptions.setFont(menuFont);
	windOptions.setString("Options: press 'S'");
	windOptions.setCharacterSize(CHAR_SIZE);
	windOptions.setFillColor(sf::Color::Black);
	windOptions.setPosition(TEXT_X, 272.f + SPACING);
	
	windQuitGame.setFont(menuFont);
	windQuitGame.setString("Quit game: press 'Q'");
	windQuitGame.setCharacterSize(CHAR_SIZE);
	windQuitGame.setFillColor(sf::Color::Black);
	windQuitGame.setPosition(TEXT_X, 335.f + SPACING);
	
	windPoisonedFruit.setFont(menuFont);
	windPoisonedFruit.setString("Poisoned fruits: " + allowedPoisonedApple + " ('N')");
	windPoisonedFruit.setCharacterSize(CHAR_SIZE);
	windPoisonedFruit.setFillColor(sf::Color::Black);
	windPoisonedFruit.setPosition(TEXT_X, 209.f + SPACING);
	
	windDifficultyLevel.setFont(menuFont);
	windDifficultyLevel.setString("Difficulty level: " + difficulty_level + " ('L')");
	windDifficultyLevel.setCharacterSize(CHAR_SIZE);
	windDifficultyLevel.setFillColor(sf::Color::Black);
	windDifficultyLevel.setPosition(TEXT_X, 272.f + SPACING);
	
	windMusic.setFont(menuFont);
	windMusic.setString("Music: " + music_on_off + " ('M')");
	windMusic.setCharacterSize(CHAR_SIZE);
	windMusic.setFillColor(sf::Color::Black);
	windMusic.setPosition(TEXT_X, 335.f + SPACING);
	
	windReturnFromOptions.setFont(menuFont);
	windReturnFromOptions.setString("Return to menu ('Esc')");
	windReturnFromOptions.setCharacterSize(CHAR_SIZE);
	windReturnFromOptions.setFillColor(sf::Color::Black);
	windReturnFromOptions.setPosition(TEXT_X, 398.f + SPACING);
	
	windSelectGameMode.setFont(menuFont);
	windSelectGameMode.setString("Game mode: " + snakes_count + " player ('V')");
	windSelectGameMode.setCharacterSize(CHAR_SIZE);
	windSelectGameMode.setFillColor(sf::Color::Black);
	windSelectGameMode.setPosition(TEXT_X, 461.f + SPACING);
    
    arrowRight.setFont(menuFont);
    arrowRight.setString(">");
    arrowRight.setCharacterSize(40.f);
    arrowRight.setFillColor(sf::Color::Black);
    arrowRight.setPosition(REACT_X + REACT_WIDTH - arrowRight.getLocalBounds().width - 5.f, 520.f + SPACING + REACT_WIDTH / 2 - 31.f);
    
    arrowLeft.setFont(menuFont);
    arrowLeft.setString("<");
    arrowLeft.setCharacterSize(40.f);
    arrowLeft.setFillColor(sf::Color::Black);
    arrowLeft.setPosition(REACT_X + 5.f, 520.f + SPACING + REACT_WIDTH / 2 - 31.f);
	
	
	menuMusic.setVolume(50.f);
	menuMusic.setLoop(true);
	if (gameMusicOn == true)
		menuMusic.play();
    while (menuWindow.pollEvent(menuEvent));
	//starting main Menu loop
	while (menuWindow.isOpen())
	{
		
        while (menuWindow.pollEvent(menuEvent))
        {
			//menuMusic.play();
            // Close window: exit
            rectangleBasic.setFillColor(sf::Color(20, 100, 150));
            localMousePosition = sf::Mouse::getPosition(menuWindow);
            if(localMousePosition.x >= REACT_X && localMousePosition.x <= REACT_X + REACT_WIDTH
               && localMousePosition.y >= 205.f + SPACING && localMousePosition.y <= 205.f + SPACING + REACT_HEIGHT) {
                rectangleBasic.setFillColor(sf::Color(20, 90, 110));
            }
            
            rectangleBasic2.setFillColor(sf::Color(20, 100, 150));
            if(localMousePosition.x >= REACT_X && localMousePosition.x <= REACT_X + REACT_WIDTH
               && localMousePosition.y >= 268.f + SPACING && localMousePosition.y <= 268.f + SPACING + REACT_HEIGHT) {
                rectangleBasic2.setFillColor(sf::Color(20, 90, 110));
            }
            
            rectangleBasic3.setFillColor(sf::Color(20, 100, 150));
            if(localMousePosition.x >= REACT_X && localMousePosition.x <= REACT_X + REACT_WIDTH
               && localMousePosition.y >= 331.f + SPACING && localMousePosition.y <= 331.f + SPACING + REACT_HEIGHT) {
                rectangleBasic3.setFillColor(sf::Color(20, 90, 110));
            }
            
            rectangleBasic4.setFillColor(sf::Color(20, 100, 150));
            if(localMousePosition.x >= REACT_X && localMousePosition.x <= REACT_X + REACT_WIDTH
               && localMousePosition.y >= 394.f + SPACING && localMousePosition.y <= 394.f + SPACING + REACT_HEIGHT) {
                rectangleBasic4.setFillColor(sf::Color(20, 90, 110));
            }
            
            rectangleBasic5.setFillColor(sf::Color(20, 100, 150));
            if(localMousePosition.x >= REACT_X && localMousePosition.x <= REACT_X + REACT_WIDTH
               && localMousePosition.y >= 457.f + SPACING && localMousePosition.y <= 457.f + SPACING + REACT_HEIGHT) {
                rectangleBasic5.setFillColor(sf::Color(20, 90, 110));
            }
            
            arrowRight.setFillColor(sf::Color::Black);
            if(localMousePosition.x >= REACT_X + REACT_WIDTH - arrowRight.getLocalBounds().width - 5.f
               && localMousePosition.x <= REACT_X + REACT_WIDTH - 5.f
               && localMousePosition.y >= 520.f + SPACING + REACT_WIDTH / 2 - 14.f
               && localMousePosition.y <= 520.f + SPACING + REACT_WIDTH / 2 + arrowRight.getLocalBounds().height / 2) {
                arrowRight.setFillColor(sf::Color(24, 0, 255));
            }
            
            arrowLeft.setFillColor(sf::Color::Black);
            if(localMousePosition.x >= REACT_X + 5.f
               && localMousePosition.x <= REACT_X + 5.f + arrowLeft.getLocalBounds().width
               && localMousePosition.y >= 520.f + SPACING + REACT_WIDTH / 2 - 14.f
               && localMousePosition.y <= 520.f + SPACING + REACT_WIDTH / 2 + arrowLeft.getLocalBounds().height / 2) {
                arrowLeft.setFillColor(sf::Color(24, 0, 255));
            }
            
            
			if (menuEvent.type == sf::Event::Closed)
			{
				menuWindow.close();
				return -1;
			}
			else if (menuEvent.type == sf::Event::KeyPressed)
			{
				if ((menuEvent.key.code == sf::Keyboard::Q || menuEvent.key.code == sf::Keyboard::Escape) && entered_settings == 0)
			    {
			        //the escape key was pressed
			        //Q and Esc are used to exit game
			        menuWindow.close();
			        return -1;
			    }
			    else if (menuEvent.key.code == sf::Keyboard::P && entered_settings == 0)
			    {
					menuWindow.close();
					return 0;
				}
				else if (menuEvent.key.code == sf::Keyboard::S)
				{
					entered_settings = 1;
				}
				else if (menuEvent.key.code == sf::Keyboard::M && entered_settings == 1)
				{
					if (gameMusicOn == true)
					{
						music_on_off = "off";
						gameMusicOn = false;
						windMusic.setString("Music: " + music_on_off + " ('M')");
						menuMusic.stop();
					}
					else
					{
						music_on_off = "on";
						gameMusicOn = true;
						windMusic.setString("Music: " + music_on_off + " ('M')");
						menuMusic.play();
						menuMusic.setVolume(50.f);
						
					}
				}
				else if (menuEvent.key.code == sf::Keyboard::Escape && entered_settings == 1)
				{
					entered_settings = 0;
				}
				else if (menuEvent.key.code == sf::Keyboard::L && entered_settings == 1)
				{
					difficulty++; 
					difficulty = difficulty % 4;
					if (difficulty == 0)
					{
						difficulty_level = "easy";
						snakeSpeed = 9;
						windDifficultyLevel.setString("Difficulty level: " + difficulty_level + " ('L')");
					}
					else if (difficulty == 1)
					{
						difficulty_level = "normal";
						snakeSpeed = 7;
						windDifficultyLevel.setString("Difficulty level: " + difficulty_level + " ('L')");
					}
					else if (difficulty == 2)
					{
						difficulty_level = "hard";
						snakeSpeed = 4;
						windDifficultyLevel.setString("Difficulty level: " + difficulty_level + " ('L')");
					}
					else
					{
						difficulty_level = "titan";
						snakeSpeed = 4;
						windDifficultyLevel.setString("Difficulty level: " + difficulty_level + " ('L')");
					}
					
				}
				else if (menuEvent.key.code == sf::Keyboard::N && entered_settings == 1)
				{
					if (poisonedAppleOn == false)
					{
						poisonedAppleOn = true;
						allowedPoisonedApple = "on";
						windPoisonedFruit.setString("Poisoned fruits: " + allowedPoisonedApple + " ('N')");
					}
					else
					{
						poisonedAppleOn = false;
						allowedPoisonedApple = "off";
						windPoisonedFruit.setString("Poisoned fruits: " + allowedPoisonedApple + " ('N')");
					}
				}
				else
				{
					continue;
				}
			}
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
			    // left mouse button is pressed
			    localMousePosition = sf::Mouse::getPosition(menuWindow);
			    if (localMousePosition.x >= REACT_X && localMousePosition.x <= (REACT_X + REACT_WIDTH))
			    {
					if (localMousePosition.y >= (205.f + SPACING + MOUSE_CORRECTION) && localMousePosition.y <= (205.f + SPACING + MOUSE_CORRECTION + REACT_HEIGHT) && entered_settings == 0)
					{
						//Start the game window was clicked by mouse
						menuWindow.close();
						return 0;
					}
					else if (localMousePosition.y >= (268.f + SPACING + MOUSE_CORRECTION) && localMousePosition.y <= (268.f + SPACING + MOUSE_CORRECTION + REACT_HEIGHT) && entered_settings == 0)
					{
						//settings window was clicked by mouse
						entered_settings = 1;
					}
					else if (localMousePosition.y >= (331.f + SPACING + MOUSE_CORRECTION) && localMousePosition.y <= (331.f + SPACING + MOUSE_CORRECTION + REACT_HEIGHT) && entered_settings == 0)
					{
						//quit game was clicked by mouse
				        menuWindow.close();
				        return -1;
					}
					else if (localMousePosition.y >= (205.f + SPACING + MOUSE_CORRECTION) && localMousePosition.y <= (205.f + SPACING + MOUSE_CORRECTION + REACT_HEIGHT) && entered_settings == 1)
					{
						if (poisonedAppleOn == false)
						{
							poisonedAppleOn = true;
							allowedPoisonedApple = "on";
							windPoisonedFruit.setString("Poisoned fruits: " + allowedPoisonedApple + " ('N')");
						}
						else
						{
							poisonedAppleOn = false;
							allowedPoisonedApple = "off";
							windPoisonedFruit.setString("Poisoned fruits: " + allowedPoisonedApple + " ('N')");
						}
					}
					else if (localMousePosition.y >= (268.f + SPACING + MOUSE_CORRECTION) && localMousePosition.y <= (268.f + SPACING + MOUSE_CORRECTION + REACT_HEIGHT) && entered_settings == 1)
					{
						//difficulty window was cliked by mouse
						difficulty++; 
						difficulty = difficulty % 4;
						if (difficulty == 0)
						{
							difficulty_level = "easy";
							snakeSpeed = 9;
							windDifficultyLevel.setString("Difficulty level: " + difficulty_level + " ('L')");
						}
						else if (difficulty == 1)
						{
							difficulty_level = "normal";
							snakeSpeed = 7;
							windDifficultyLevel.setString("Difficulty level: " + difficulty_level + " ('L')");
						}
						else if (difficulty == 2)
						{
							difficulty_level = "hard";
							snakeSpeed = 4;
							windDifficultyLevel.setString("Difficulty level: " + difficulty_level + " ('L')");
						}
						else
						{
							difficulty_level = "titan";
							snakeSpeed = 4;
							windDifficultyLevel.setString("Difficulty level: " + difficulty_level + " ('L')");
						}
					}
					else if (localMousePosition.y >= (331.f + SPACING + MOUSE_CORRECTION) && localMousePosition.y <= (331.f + SPACING + MOUSE_CORRECTION + REACT_HEIGHT) && entered_settings == 1)
					{
						//music window was clicked by mouse
						if (gameMusicOn == true)
						{
							music_on_off = "off";
							gameMusicOn = false;
							windMusic.setString("Music: " + music_on_off + " ('M')");
							menuMusic.stop();
						}
						else
						{
							music_on_off = "on";
							gameMusicOn = true;
							windMusic.setString("Music: " + music_on_off + " ('M')");
							menuMusic.play();
							menuMusic.setVolume(50.f);
						}
					}
					else if (localMousePosition.y >= (394.f + SPACING + MOUSE_CORRECTION) && localMousePosition.y <= (394.f + SPACING + MOUSE_CORRECTION + REACT_HEIGHT) && entered_settings == 1)
					{
						//Return to menu was clicked by mouse
						entered_settings = 0;
					}
					else if (localMousePosition.y >= (457.f + SPACING + MOUSE_CORRECTION) && localMousePosition.y <= (457.f + SPACING + MOUSE_CORRECTION + REACT_HEIGHT) && entered_settings == 1)
					{
						//Select board was clicked by mouse
					}
					else
					{
						//nothing meaningful happened; proceed with loop 
						continue;
					}
				}
			}
        }
        menuWindow.clear();
        menuWindow.draw(menuSprite);
        menuWindow.draw(mainText);
        menuWindow.draw(rectangleBasic);
	    menuWindow.draw(rectangleBasic2);
	    menuWindow.draw(rectangleBasic3);
        if (entered_settings == 0)
        {
	        menuWindow.draw(windStartGame);
			menuWindow.draw(windOptions);
	        menuWindow.draw(windQuitGame);
		}
		else
		{
			menuWindow.draw(rectangleBasic4);
			menuWindow.draw(rectangleBasic5);
			menuWindow.draw(rectangleBoard);
			menuWindow.draw(windDifficultyLevel);
			menuWindow.draw(windPoisonedFruit);
	        menuWindow.draw(windReturnFromOptions);
	        menuWindow.draw(windMusic);
	        menuWindow.draw(windSelectGameMode);
            menuWindow.draw(arrowLeft);
            menuWindow.draw(arrowRight);
		}
        menuWindow.display();
	sf::sleep(sf::milliseconds(6));
	}
	return -1;
}

/*
 * MENU
 */

int main(int, char const**)
{
	short run_return = 1;
	while (stay_in_menu == 1)
	{
		if (run_return == 1)
		{
			if (EnterMenu() == 0)
			{
				run_return = run(BOARDS_PATH + "corners.txt");
			}
			else
			{
				return 0;	
			}
		}
		else
		{
			run_return = run(BOARDS_PATH + "corners.txt");
		}
	}
	
	//if (EnterMenu() == 0)
	//{
		//run();
	//	run(BOARDS_PATH + "wallsAroundBoard.txt");
	//}
	return 0;
}
