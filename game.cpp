
#include "snake.cpp"
#include "BoardReader.cpp"
#include "collectableObj.cpp"
#include "particle.cpp"
#include "TexturesAndSprites.cpp"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
//DEBUG:
#include <iostream>
#include <string>


int snake_x = GRID_SIZE_X / 2;
int snake_y = GRID_SIZE_Y / 2;

//temp as global, put it into main later mby
int score = 0;
int highScore = 0;

const int window_width = GRID_SIZE_X * cell_size_pix;
const int window_height = GRID_SIZE_Y * cell_size_pix;

BoardReader boardReader;

void deleteParticle(PartycleSystem &toDelete){
    Particle* temp = toDelete.getFirstParticle();
    Particle* help;
    while(temp) {
        help = temp->next;
        delete temp;
        temp = help;
    }
}

///INITS:

void initGame(sf::SoundBuffer& appleEatingSound, sf::Sound& appleEating) {
    snakeTexture.loadFromFile(TEXTURES_PATH+"snake.png");
    snakeSP.setTexture(snakeTexture);
    headTexture.loadFromFile(TEXTURES_PATH+"head.png");
    head.setTexture(headTexture);
    appleTexture.loadFromFile(TEXTURES_PATH+"apple.png");
    appleSP.setTexture(appleTexture);
    wallTexture.loadFromFile(TEXTURES_PATH+"wall.png");
    wallSP.setTexture(wallTexture);
    playTexture.loadFromFile(TEXTURES_PATH+"play.png");
    playSP.setTexture(playTexture);
    pauseTexture.loadFromFile(TEXTURES_PATH+"pause.png");
    pauseSP.setTexture(pauseTexture);

    /// SOUNDS ///
    appleEatingSound.loadFromFile(SOUNDS_PATH+"applebite.wav");
    appleEating.setBuffer(appleEatingSound);

    gameOverSound.loadFromFile(SOUNDS_PATH + "gameover2.wav");
    gameOver.setBuffer(gameOverSound);
    gameOver.setVolume(50);

    appleEating.setVolume(40);
}

void initMusic()
{
    if (gameMusicOn == true)
     {
		 if (!backgroundMusic.openFromFile(SOUNDS_PATH + "gameMusic.ogg"))
		 {
			std::cout<<"Music File error! Couldn't load: " 
                        + SOUNDS_PATH + "gameMusic.ogg"<<std::endl;
		 }
		 backgroundMusic.setVolume(30.f);
		 backgroundMusic.setLoop(true);
	 }
}

void setupScoreDisplayers( sf::Font &font)
{
    ScoreText.setFont(font); // font is a sf::Font
    ScoreText.setString("Score: " + std::to_string(score));
    ScoreText.setCharacterSize(48); // in pixels, not points!
    ScoreText.setFillColor(sf::Color::Black);
    ScoreText.setStyle(sf::Text::Bold | sf::Text::Underlined);

    //HighScoreText
    HighScoreText.setFont(font); // font is a sf::Font
    HighScoreText.setString("Best Score: " + std::to_string(highScore));
    HighScoreText.setCharacterSize(48); // in pixels, not points!
    HighScoreText.setFillColor(sf::Color::Black);
    HighScoreText.setStyle(sf::Text::Bold | sf::Text::Underlined);

}

void loadHighScore()
{
    std::fstream highScoreFile;
    std::string hishScoreFileFullPath = SAVES_PATH + HIGH_SCORE_FILE_NAME;
    highScoreFile.open(hishScoreFileFullPath.c_str(), std::ios::in | std::ios::binary);
    highScore = 0;
    highScoreFile.read((char *) &highScore, sizeof(highScore));
    highScoreFile.close();
}

void saveHighScore()
{
    if(score > highScore)
    {
        std::fstream highScoreFile;
        std::string hishScoreFileFullPath = SAVES_PATH + HIGH_SCORE_FILE_NAME;
        highScoreFile.open(hishScoreFileFullPath.c_str(), 
                    std::ios::out | std::ios::trunc | std::ios::binary);
        std::string scoreAsString = std::to_string(score);

        highScoreFile.write((char *) &score, sizeof(score));

        highScoreFile.close();
    }
}

void resizeSnake(Snake& snake, int size)
{
    if(size > 0)
    {
        for(int i = 0; i < size; i++)
            snake.grow();
    }
    else
    {
        for(int i = 0; i < -size; i++)
            snake.fade();
    }
    
}

//3 - black     //2 - gold      //1 - red
PartycleSystem makeParticles(int color = 1, int particleDense = 100)
{
    srand (time(NULL));
    Particle* firstParticle = 
     new Particle(
        rand() % 200 / 100.0, //speedX
        rand() %  200 / 100.0   //speedY
        , 2.5 + rand() % 100 / 100.0,      //ttl
        1.5 + rand() % 300 / 1000.0,      //size
        color ,                     //color
        rand() % 50/100.0 - 0.25,   //velocityX
        rand() % 50/100.0 - 0.25);   //velocityY


    Particle* currParticle = firstParticle;

    for(int i = 0; i < particleDense; i++)
    {
        currParticle -> next = new Particle(
        rand() % 200 / 100.0, //speedX
        rand() %  200 / 100.0   //speedY
        , 2.5 + rand() % 100 / 100.0,      //ttl
        1.5 + rand() % 300 / 1000.0,      //size
        color ,                     //color
        rand() % 50/100.0 - 0.25,   //velocityX
        rand() % 50/100.0 - 0.25);   //velocityY


        currParticle = currParticle ->next; 
        currParticle ->next = nullptr;
    }

    PartycleSystem collectedApplePS = PartycleSystem(firstParticle, 0, 0, 0);
    return collectedApplePS;
}


void updateScore(int addToScore){
    if(score + addToScore < 0) {
        score = 0;
    }else
        score += addToScore;
    ScoreText.setString("Score: " + std::to_string(score));
}

void snakeHeadCollision(Snake *snake, collectableObj* objects[], 
    sf::Sound& appleEating, PartycleSystem &appleEatingPS, int collObjAmount = 0)
{
    for(int i = 0; i <  collObjAmount; i++)
    {

        if(snake->getHead()->x == objects[i]->getPosX() &&  snake->getHead()->y == objects[i]->getPosY() )
        {
            if(objects[i]->getIsGolden())
                appleEatingPS = makeParticles(2);
            else if(objects[i]->getIsPoisoned())
                appleEatingPS = makeParticles(3);
            else
                appleEatingPS = makeParticles(1);
            appleEatingPS.setPosition(objects[i] ->getPosX() * cell_size_pix, 
            objects[i] ->getPosY() * cell_size_pix);

            appleEating.play();
            resizeSnake(*snake, objects[i]->getSizeBonus());
            updateScore(objects[i]->getScoreBonus());
            objects[i]->goToFreeRandomPosistion(boardReader.wallHead, snake->getHead());
        }
    }

}

void drawAll(sf::RenderWindow& window, Snake& snake, 
                collectableObj& collObj, PartycleSystem &collectedApplePS,
                 collectableObj &poisonedApple, bool& gamePaused)

{
    bodyPart* curr = snake.getHead();
    
    while(curr)
    {
        if(curr == snake.getHead()) {
            head.setPosition(curr->x * cell_size_pix, curr->y * cell_size_pix);
            window.draw(head);
        }else{
            snakeSP.setPosition(curr->x * cell_size_pix, curr->y * cell_size_pix);
            window.draw(snakeSP);
        }
        curr = curr->next;
    }

    // drawing apple
    sf::Sprite tempAppleSP = appleSP;
    if(collObj.getIsGolden())
        tempAppleSP.setColor(sf::Color(255,215,0));    //golden aplle color
    else if(!collObj.getIsPoisoned())
        tempAppleSP.setColor(sf::Color(255, 0, 0));

    tempAppleSP.setPosition(collObj.getPosX() * cell_size_pix, collObj.getPosY()* cell_size_pix);
    window.draw(tempAppleSP);
    
    if(poisonedAppleOn)
    {
        tempAppleSP = appleSP;
        tempAppleSP.setColor(sf::Color(0,0,0));

        tempAppleSP.setPosition(poisonedApple.getPosX() * cell_size_pix, poisonedApple.getPosY()* cell_size_pix);
        window.draw(tempAppleSP);

    }

    //drawing score
    ScoreText.setPosition(20, 20);   
    window.draw(ScoreText);

    //drawing best score
    HighScoreText.setPosition(300, 20);   
    window.draw(HighScoreText);


    //drawing particle effect
    Particle* particleToDraw = collectedApplePS.getFirstParticle();

    while(particleToDraw)
    {
        if(particleToDraw ->ttl >=0)
        {
            sf::CircleShape shape(particleToDraw->size);
            if(particleToDraw->color == 1){
                shape.setFillColor(sf::Color(255, 0, 0));
            }else if(particleToDraw->color == 2)
                shape.setFillColor(sf::Color(255,215,0));
            else
                shape.setFillColor(sf::Color(0,0,0));
            shape.setPosition(
            (snake.getHead()->x * cell_size_pix + particleToDraw->positionX + cell_size_pix/2), 
            (snake.getHead()->y * cell_size_pix + particleToDraw->positionY + cell_size_pix/2));
            
            window.draw(shape);
        }
        particleToDraw = particleToDraw->next;
    }

    //Drawing board (with walls)

    wall* currWall = boardReader.wallHead;

    while(currWall!= nullptr)
    {
         wallSP.setPosition(
            currWall->getX() * cell_size_pix, 
            currWall->getY() * cell_size_pix);

        window.draw(wallSP);

        currWall = currWall->getNext();
    }

    //pause menu
    if(!gamePaused) {
        playSP.setPosition((GRID_SIZE_X - 1) * cell_size_pix, 0);
        window.draw(playSP);
    }else{
        pauseSP.setPosition((GRID_SIZE_X - 1) * cell_size_pix, 0);
        window.draw(pauseSP);
    }

}


void windowPollEvent(sf::RenderWindow &window,
sf::Event &ev, Direction &newDir, Snake &snake, bool &gamePaused)
{
    while(window.pollEvent(ev)) {
        if(ev.type == sf::Event::Closed) {
            window.close();
        }else if(ev.type == sf::Event::KeyPressed) {
            if(gamePaused){
                if(ev.key.code == sf::Keyboard::Escape) {
                    gamePaused = false;
                }
            }else{
                switch (ev.key.code) {
                    case sf::Keyboard::W:
                        if(snake.getDirection() == Direction::up || snake.getDirection() == Direction::down)
                            break;
                        newDir = Direction::up;
                        break;
                    case sf::Keyboard::S:
                        if(snake.getDirection() == Direction::down || snake.getDirection() == Direction::up)
                            break;;
                        newDir = Direction::down;
                        break;
                    case sf::Keyboard::A:
                        if(snake.getDirection() == Direction::left || snake.getDirection() == Direction::right)
                            break;
                        newDir = Direction::left;
                        break;
                    case sf::Keyboard::D:
                        if(snake.getDirection() == Direction::right || snake.getDirection() == Direction::left)
                            break;
                        newDir = Direction::right;
                        break;
                    case sf::Keyboard::Escape:
                        gamePaused = true;
                        break;
                    default:
                        break;
                }
            }
        }
    }
}

short run(std::string boardName = "")
{
    bool gamePaused = false;
    int frameCounter = 0;
   
    // Init game
    
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Snake Game", sf::Style::Titlebar | sf::Style::Close);
    initGame(appleEatingSound, appleEating);
    
    initMusic();

    // Loading Board
    if(boardName != "")
    {
        boardReader.readBoard(boardName);
    }
    //loading font
    sf::Font font;
    if (!font.loadFromFile(FONTS_PATH+"cabin-sketch.bold.ttf"))
    {
        std::cout<<"Font file missing"<<std::endl;
    }

    loadHighScore();
    setupScoreDisplayers(font);

    PartycleSystem collectedApplePS;
    
    //Game loop
    srand(time(NULL));
    Snake snake = Snake(snake_x, snake_y);
    snake.changeDirection(Direction::left);
    resizeSnake(snake, 5);

    collectableObj apple = collectableObj("apple", 0, 0, 1, 1);
    apple.goToFreeRandomPosistion(boardReader.wallHead, snake.getHead());
    
    collectableObj poisonedApple("none", 0, 0 ,0 ,0);
    if(poisonedAppleOn)
    {
        poisonedApple = collectableObj("poisonedApple", 0, 0, -5, -1, true);
        poisonedApple.goToFreeRandomPosistion(boardReader.wallHead, snake.getHead());
        poisonedApple.makePosion();
    }

    Direction newDir;
    newDir = snake.getDirection();
    int i = 0;

    //saving all objects snake can collide with
    int collisonObjsAmount = 1;
    if(poisonedAppleOn)
        collisonObjsAmount = 2;
    collectableObj* AllCollectableObjs[10];

    AllCollectableObjs[0] = &apple;
    if(poisonedAppleOn)
        AllCollectableObjs[1] = &poisonedApple;

	if (gameMusicOn == true)
	{
		backgroundMusic.play();
	}

    while(window.isOpen()){
        i++;
        sf::Event ev;
        
        windowPollEvent(window, ev, newDir, snake, gamePaused);
        
        if(!gamePaused){
            
            if(frameCounter % snakeSpeed == 0)
            {      
                snake.changeDirection(newDir);
                if(!snake.move(&boardReader)){
                    
                    gameOver.play();
                    sf::sleep(sf::milliseconds(2500));
                    saveHighScore();
                    return 1;
                }
                frameCounter = 0;
            }
        }

        window.clear(sf::Color(153,204,255,100));

        snakeHeadCollision(&snake, AllCollectableObjs, appleEating, collectedApplePS, collisonObjsAmount);
        collectedApplePS.blowUp();

        drawAll(window, snake, apple, collectedApplePS, poisonedApple, gamePaused);//, text);
        window.display();
        
        sf::sleep(sf::milliseconds(frameFreezeTime));
        frameCounter++;
    }

    saveHighScore();

    deleteParticle(collectedApplePS);
	
	backgroundMusic.stop();
    
    return 1;       //lost, run again
}
