#pragma once
#include <SFML/Graphics.hpp>
#pragma once
#include <SFML/Audio.hpp>

sf::Text ScoreText;      // for displaying score
sf::Text HighScoreText;      // for displaying best score
sf::Text PauseText;      // for displaying pause
sf::Text ChangeDirectionText;
sf::Text TryAgainText;
sf::Text ExitText;
sf::Text GameOverText;
sf::Text YouWinText;

sf::Texture snakeTexture;
sf::Sprite snakeSP;
sf::Texture headTexture;
sf::Sprite head;
sf::Texture appleTexture;
sf::Sprite appleSP;
sf::Texture wallTexture;
sf::Sprite wallSP;
sf::Texture playTexture;
sf::Sprite playSP;
sf::Texture pauseTexture;
sf::Sprite pauseSP;
sf::Texture homeTexture;
sf::Sprite homeSP;
sf::Texture backgroundTexture;
sf::Sprite backgroundSP;
sf::Texture lifeTexture;
sf::Sprite lifeSP;

sf::RectangleShape scoreBar;


//sounds

