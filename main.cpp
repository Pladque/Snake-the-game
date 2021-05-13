#include "game.cpp"

/*
 * MENU	
 */ 

#define MULT 2
#define REACT_X 28.f
#define TEXT_X 30.f
//#define REACT_Y

int difficulty = 1;			//0 - easy, 1 - normal, 2 - hard
bool entered_settings = 0;	//value to determine if settings should be displayed on the screen
bool stay_in_menu = 1;		//may be used to determine if cotrol should stay in main Menu,
							//otherwise to move on with logic

int EnterMenu()
{
	std::string snakes_count = "1";
	std::string difficulty_level = "normal";
	std::string music_on_off = "on";
	extern int snakeSpeed;
	
	//Main function -> depending on its return, game would start or not
	sf::Texture menuTexture;
	if (!menuTexture.loadFromFile(TEXTURES_PATH + "mainSnake2.jpg"))
		return -1;
	sf::Music menuMusic;
	if (!menuMusic.openFromFile(SOUNDS_PATH + "menuMusic.ogg"))
        return -1;
	sf::RenderWindow menuWindow(sf::VideoMode(1280, 960), "SNAKE_2D");	//resolution may be changed, after every other thing works
																		//as expected
	sf::Sprite menuSprite(menuTexture);
	sf::Event menuEvent;
	sf::Font menuFont;
    if (!menuFont.loadFromFile("./Assets/Fonts/JosefinSans-SemiBoldItalic.ttf"))
        return -1;
	sf::Text mainText, windStartGame, windQuitGame, windOptions, windTryAgain, windResign;	//text, displayed in consecutive windows
	sf::Text windDifficultyLevel, windSnakesCount, windMusic, windReturnFromOptions;
													//end of Menu init
													
	sf::RectangleShape rectangleBasic(sf::Vector2f(232.f, 25.f));
    rectangleBasic.setFillColor(sf::Color(20, 100, 150));
    rectangleBasic.setPosition(REACT_X, 115.f);
    rectangleBasic.setOutlineThickness(5.f);
	rectangleBasic.setOutlineColor(sf::Color(250, 150, 100));
	
	sf::RectangleShape rectangleBasic2(sf::Vector2f(232.f, 25.f));
    rectangleBasic2.setFillColor(sf::Color(20, 100, 150));
    rectangleBasic2.setPosition(REACT_X, 178.f);		
    rectangleBasic2.setOutlineThickness(5.f);
	rectangleBasic2.setOutlineColor(sf::Color(250, 150, 100));
    
    sf::RectangleShape rectangleBasic3(sf::Vector2f(232.f, 25.f));
    rectangleBasic3.setFillColor(sf::Color(20, 100, 150));
    rectangleBasic3.setPosition(REACT_X, 241.f);		
    rectangleBasic3.setOutlineThickness(5.f);
	rectangleBasic3.setOutlineColor(sf::Color(250, 150, 100));		
	
	sf::RectangleShape rectangleBasic4(sf::Vector2f(232.f, 25.f));
    rectangleBasic4.setFillColor(sf::Color(20, 100, 150));
    rectangleBasic4.setPosition(REACT_X, 304.f);		
    rectangleBasic4.setOutlineThickness(5.f);
	rectangleBasic4.setOutlineColor(sf::Color(250, 150, 100));								
													
	mainText.setFont(menuFont);
	mainText.setString("SNAKE 2D");
	mainText.setCharacterSize(36);
	mainText.setFillColor(sf::Color::Red);
	mainText.setPosition(570.f, 5.f);
	mainText.setStyle(sf::Text::Bold);
	
	windStartGame.setFont(menuFont);
	windStartGame.setString("New game: press 'P'");
	windStartGame.setCharacterSize(18);
	windStartGame.setFillColor(sf::Color::Black);
	windStartGame.setPosition(TEXT_X, 116.f);
	
	windOptions.setFont(menuFont);
	windOptions.setString("Options: press 'S'");
	windOptions.setCharacterSize(18);
	windOptions.setFillColor(sf::Color::Black);
	windOptions.setPosition(TEXT_X, 179.f);
	
	windQuitGame.setFont(menuFont);
	windQuitGame.setString("Quit game: press 'Q'");
	windQuitGame.setCharacterSize(18);
	windQuitGame.setFillColor(sf::Color::Black);
	windQuitGame.setPosition(TEXT_X, 242.f);
	
	windSnakesCount.setFont(menuFont);
	windSnakesCount.setString("Snakes number: " + snakes_count + " ('N')");
	windSnakesCount.setCharacterSize(18);
	windSnakesCount.setFillColor(sf::Color::Black);
	windSnakesCount.setPosition(TEXT_X, 116.f);
	
	windDifficultyLevel.setFont(menuFont);
	windDifficultyLevel.setString("Difficulty level: " + difficulty_level + " ('L')");
	windDifficultyLevel.setCharacterSize(18);
	windDifficultyLevel.setFillColor(sf::Color::Black);
	windDifficultyLevel.setPosition(TEXT_X, 179.f);
	
	windMusic.setFont(menuFont);
	windMusic.setString("Music: " + music_on_off + " ('M')");
	windMusic.setCharacterSize(18);
	windMusic.setFillColor(sf::Color::Black);
	windMusic.setPosition(TEXT_X, 242.f);
	
	windReturnFromOptions.setFont(menuFont);
	windReturnFromOptions.setString("Return to menu ('Esc')");
	windReturnFromOptions.setCharacterSize(18);
	windReturnFromOptions.setFillColor(sf::Color::Black);
	windReturnFromOptions.setPosition(TEXT_X, 305.f);
	
	
	menuMusic.play(); 
	menuMusic.setVolume(50.f);
	//starting main Menu loop
	while (menuWindow.isOpen())
	{
		
        while (menuWindow.pollEvent(menuEvent))
        {
			//menuMusic.play();
            // Close window: exit
			if (menuEvent.type == sf::Event::Closed)
				menuWindow.close();
			else if (menuEvent.type == sf::Event::KeyPressed)
			{
				if ((menuEvent.key.code == sf::Keyboard::Q || menuEvent.key.code == sf::Keyboard::Q) && entered_settings == 0)
			    {
			        //the escape key was pressed
			        //Q is used to exit game
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
					if (music_on_off == "on")
					{
						music_on_off = "off";
						windMusic.setString("Music: " + music_on_off + " ('M')");
						menuMusic.stop();
					}
					else
					{
						music_on_off = "on";
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
					difficulty = difficulty % 3;
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
					else
					{
						difficulty_level = "hard";
						snakeSpeed = 4;
						windDifficultyLevel.setString("Difficulty level: " + difficulty_level + " ('L')");
					}
				}
				else
				{
					continue;
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
			menuWindow.draw(windDifficultyLevel);
			menuWindow.draw(windSnakesCount);
	        menuWindow.draw(windReturnFromOptions);
	        menuWindow.draw(windMusic);
		}
        menuWindow.display();
	}
	return -1;
}

/*
 * MENU
 */

int main(int, char const**)
{
	if (EnterMenu() == 0)
	{
		run();
	}
	else
	{
		return 0;
	}
}
