#include "game.cpp"

/*
 * MENU	
 */
 
bool stay_in_menu = 1;	//may be used to determine if cotrol should stay in main Menu,
						//otherwise to move on with logic

int EnterMenu()
{
	//Main function -> depending on its return, game would start or not
	sf::Texture menuTexture;
	if (!menuTexture.loadFromFile(TEXTURES_PATH + "mainSnake2.jpg"))
		return -1;
	sf::Music menuMusic;
	sf::RenderWindow menuWindow(sf::VideoMode(600, 450), "SNAKE_2D");	//resolution may be changed, after every other thing works
																		//as expected
	sf::Sprite menuSprite(menuTexture);
	sf::Event menuEvent;
	sf::Font menuFont;
    if (!menuFont.loadFromFile("./Assets/Fonts/JosefinSans-SemiBoldItalic.ttf"))
        return -1;
	sf::Text mainText, windStartGame, windQuitGame, windTryAgain, windResign;	//text, displayed in consecutive windows
													//end of Menu init
													
	sf::RectangleShape rectangleBasic(sf::Vector2f(180.f, 25.f));
    rectangleBasic.setFillColor(sf::Color(20, 100, 150));
    rectangleBasic.setPosition(18.f, 115.f);
    rectangleBasic.setOutlineThickness(5.f);
	rectangleBasic.setOutlineColor(sf::Color(250, 150, 100));
    
    sf::RectangleShape rectangleBasic2(sf::Vector2f(180.f, 25.f));
    rectangleBasic2.setFillColor(sf::Color(20, 100, 150));
    rectangleBasic2.setPosition(18.f, 163.f);		
    rectangleBasic2.setOutlineThickness(5.f);
	rectangleBasic2.setOutlineColor(sf::Color(250, 150, 100));									
													
	mainText.setFont(menuFont);
	mainText.setString("SNAKE 2D");
	mainText.setCharacterSize(30);
	mainText.setFillColor(sf::Color::Red);
	mainText.setPosition(400.f, 9.f);
	mainText.setStyle(sf::Text::Bold);
	
	windStartGame.setFont(menuFont);
	windStartGame.setString("New game: press 'P'");
	windStartGame.setCharacterSize(18);
	windStartGame.setFillColor(sf::Color::Black);
	windStartGame.setPosition(20.f, 116.f);
	
	windQuitGame.setFont(menuFont);
	windQuitGame.setString("Quit game: press 'Q'");
	windQuitGame.setCharacterSize(18);
	windQuitGame.setFillColor(sf::Color::Black);
	windQuitGame.setPosition(20.f, 164.f);
	
	//starting main Menu loop
	while (menuWindow.isOpen())
	{
        while (menuWindow.pollEvent(menuEvent))
        {
            // Close window: exit
			if (menuEvent.type == sf::Event::Closed)
				menuWindow.close();
			else if (menuEvent.type == sf::Event::KeyPressed)
			{
				if (menuEvent.key.code == sf::Keyboard::Escape || menuEvent.key.code == sf::Keyboard::Q)
			    {
			        //the escape key was pressed
			        //Q is used to exit game
			        menuWindow.close();
			        return -1;
			    }
			    else if (menuEvent.key.code == sf::Keyboard::P)
			    {
					menuWindow.close();
					return 0;
				}
				else
				{
					continue;
				}
			}
        }
        menuWindow.clear();
        menuWindow.draw(menuSprite);
        menuWindow.draw(rectangleBasic);
        menuWindow.draw(rectangleBasic2);
        menuWindow.draw(mainText);
        menuWindow.draw(windStartGame);
        menuWindow.draw(windQuitGame);
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
