
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

bool isFirstGame = true;

int snake_x = GRID_SIZE_X / 2;
int snake_y = (GRID_SIZE_Y + scoreBarHeight / 32) / 2;

//temp as global, put it into main later mby
int score = 0;
int highScore = 0;


const int window_width = GRID_SIZE_X * cell_size_pix;
const int window_height = GRID_SIZE_Y * cell_size_pix + scoreBarHeight;

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


void initGame() {
    snakeTexture.loadFromFile(TEXTURES_PATH+"snake.png");
    
    headTexture.loadFromFile(TEXTURES_PATH+"head.png");
    
    appleTexture.loadFromFile(TEXTURES_PATH+"apple.png");
    
    wallTexture.loadFromFile(TEXTURES_PATH+"wall.png");
    
    playTexture.loadFromFile(TEXTURES_PATH+"play.png");
    
    pauseTexture.loadFromFile(TEXTURES_PATH+"pause.png");
    
    homeTexture.loadFromFile(TEXTURES_PATH+"home.png");
    
    backgroundTexture.loadFromFile(TEXTURES_PATH+"background.png");
    
    scoreBar.setSize(sf::Vector2f(GRID_SIZE_X * cell_size_pix, scoreBarHeight));
    
    
    /// SOUNDS ///
    
}


void initMusic(sf::Music& backgroundMusic, sf::SoundBuffer& appleEatingSound, sf::Sound& appleEating, sf::SoundBuffer& gameOverSound, sf::Sound& gameOver){
    
    appleEatingSound.loadFromFile(SOUNDS_PATH+"applebite.wav");
    gameOverSound.loadFromFile(SOUNDS_PATH + "gameover2.wav");
    gameOver.setBuffer(gameOverSound);
    gameOver.setVolume(50);
    appleEating.setBuffer(appleEatingSound);
    appleEating.setVolume(40);
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

void setupText( sf::Font &font, sf::Font &fontForPause)
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
    
    PauseText.setFont(fontForPause); // font is a sf::Font
    PauseText.setString("PAUSE");
    PauseText.setCharacterSize(120); // in pixels, not points!
    PauseText.setFillColor(sf::Color::Black);
    PauseText.setStyle(sf::Text::Bold);

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
    scoreBar.setFillColor(sf::Color(247, 152, 98));
    scoreBar.setPosition(0.f, 0.f);
    window.draw(scoreBar);
    
    backgroundSP.setPosition(0, scoreBarHeight);
    window.draw(backgroundSP);
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
        playSP.setPosition((GRID_SIZE_X - 1) * cell_size_pix, 16);
        window.draw(playSP);
    }else{
        homeSP.setPosition((GRID_SIZE_X - 2) * cell_size_pix - 10, 16);
        pauseSP.setPosition((GRID_SIZE_X - 1) * cell_size_pix, 16);
        PauseText.setPosition((window_width - PauseText.getLocalBounds().width) / 2, (window_height - PauseText.getLocalBounds().height) / 2);
        window.draw(PauseText);
        window.draw(pauseSP);
        window.draw(homeSP);
    }
    
    //drawing score
    
    ScoreText.setPosition(20, 3);
    window.draw(ScoreText);

    //drawing best score
    HighScoreText.setPosition(300, 3);
    window.draw(HighScoreText);

}


void windowPollEvent(sf::RenderWindow &window,
                     sf::Event &ev, Direction &newDir, Snake &snake, bool &gamePaused)
{
    sf::Vector2i localMousePosition;
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
        }else if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            localMousePosition = sf::Mouse::getPosition(window);
            if(localMousePosition.x >= (GRID_SIZE_X - 2) * cell_size_pix - 10.f
               && localMousePosition.x <= (GRID_SIZE_X - 2) * cell_size_pix + 22.f && localMousePosition.y >= 16.f && localMousePosition.y <= 48.f) {
                window.close();
            }else if(localMousePosition.x >= (GRID_SIZE_X - 1) * cell_size_pix - 10.f
                     && localMousePosition.x <= (GRID_SIZE_X - 1) * cell_size_pix + 22.f && localMousePosition.y >= 16.f && localMousePosition.y <= 48.f) {
                if(gamePaused)
                    gamePaused = false;
                else
                    gamePaused = true;
            }
        }
    }
}

void setTexture() {
    snakeSP.setTexture(snakeTexture);
    head.setTexture(headTexture);
    appleSP.setTexture(appleTexture);
    wallSP.setTexture(wallTexture);
    playSP.setTexture(playTexture);
    pauseSP.setTexture(pauseTexture);
    homeSP.setTexture(homeTexture);
    backgroundSP.setTexture(backgroundTexture);
}

short run(std::string boardName = "")
{
    bool gamePaused = false;
    int frameCounter = 0;
    score = 0;
    // Init game
    sf::Music backgroundMusic;    //used to play music in the background
    sf::SoundBuffer appleEatingSound;
    sf::Sound appleEating;
    sf::SoundBuffer gameOverSound;
    sf::Sound gameOver;
    
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Snake Game", sf::Style::Titlebar);
    
    if(isFirstGame) {
        initGame();
    }
    initMusic(backgroundMusic,appleEatingSound, appleEating, gameOverSound, gameOver);
    setTexture();
    
    
    isFirstGame = false;
    // Loading Board
    if(boardName != "")
    {
        boardReader.readBoard(boardName);
    }
    //loading font
    sf::Font font, fontForPause;
    if (!font.loadFromFile(FONTS_PATH+"cabin-sketch.bold.ttf"))
    {
        std::cout<<"Font file missing"<<std::endl;
    }
    if (!fontForPause.loadFromFile(FONTS_PATH+"GoblinOne-Regular.ttf"))
    {
        std::cout<<"Font file missing"<<std::endl;
    }

    loadHighScore();
    setupText(font, fontForPause);

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
                    backgroundMusic.stop();
                    deleteParticle(collectedApplePS);
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
    backgroundMusic.stop();
    deleteParticle(collectedApplePS);
	    
    return 1;       //lost, run again
}
