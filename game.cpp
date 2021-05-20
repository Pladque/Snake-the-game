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
bool isWon = false;
bool withHealth = false;


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
    
    lifeTexture.loadFromFile(TEXTURES_PATH+"heart.png");
    
    backgroundTexture.loadFromFile(TEXTURES_PATH+"background.png");
    
    scoreBar.setSize(sf::Vector2f(GRID_SIZE_X * cell_size_pix, scoreBarHeight));
    
    
    /// SOUNDS ///
    
}


void initMusic(sf::Music& backgroundMusic, sf::SoundBuffer& appleEatingSound, sf::Sound& appleEating, sf::SoundBuffer& gameOverSound, sf::Sound& gameOver, sf::SoundBuffer& winSound, sf::Sound& win){
    
    appleEatingSound.loadFromFile("./Assets/Sounds/applebite.wav");
    gameOverSound.loadFromFile("./Assets/Sounds/gameover2.wav");
    winSound.loadFromFile("./Assets/Sounds/win.wav");
    gameOver.setBuffer(gameOverSound);
    gameOver.setVolume(50);
    appleEating.setBuffer(appleEatingSound);
    appleEating.setVolume(40);
    win.setBuffer(winSound);
    win.setVolume(50);
    if (gameMusicOn == true)
     {
		 if (!backgroundMusic.openFromFile("./Assets/Sounds/gameMusic.ogg"))
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
    
    ChangeDirectionText.setFont(fontForPause); // font is a sf::Font
    ChangeDirectionText.setString("Choose the direction");
    ChangeDirectionText.setCharacterSize(50); // in pixels, not points!
    ChangeDirectionText.setFillColor(sf::Color::Black);
    ChangeDirectionText.setStyle(sf::Text::Bold);
    
    TryAgainText.setFont(fontForPause); // font is a sf::Font
    TryAgainText.setString("TRY AGAIN");
    TryAgainText.setCharacterSize(50); // in pixels, not points!
    TryAgainText.setFillColor(sf::Color::Black);
    TryAgainText.setStyle(sf::Text::Bold);
    
    ExitText.setFont(fontForPause); // font is a sf::Font
    ExitText.setString("EXIT");
    ExitText.setCharacterSize(50); // in pixels, not points!
    ExitText.setFillColor(sf::Color::Black);
    ExitText.setStyle(sf::Text::Bold);
    
    GameOverText.setFont(fontForPause); // font is a sf::Font
    GameOverText.setString("GAME OVER!");
    GameOverText.setCharacterSize(70); // in pixels, not points!
    GameOverText.setFillColor(sf::Color::Red);
    GameOverText.setStyle(sf::Text::Bold);
    
    YouWinText.setFont(fontForPause); // font is a sf::Font
    YouWinText.setString("YOU WIN!");
    YouWinText.setCharacterSize(70); // in pixels, not points!
    YouWinText.setFillColor(sf::Color::Red);
    YouWinText.setStyle(sf::Text::Bold);

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

void resizeSnake(Snake& snake, int size, bool isVisible = true)
{
    if(size > 0)
    {
        for(int i = 0; i < size; i++)
            snake.grow(isVisible);
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
            deleteParticle(appleEatingPS);
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
            if(!objects[i]->goToFreeRandomPosistion(boardReader.wallHead, snake->getHead(), objects)) {
                isWon = true;
            }
            //changing position of poisoned apple even if snake ate good one
            if(!objects[i]->getIsPoisoned())
                if(collObjAmount>=2)
                    objects[1]->goToFreeRandomPosistion(boardReader.wallHead, snake->getHead(), objects);
        }
    }

}

void drawAll(sf::RenderWindow& window, Snake& snake, 
                collectableObj& collObj, PartycleSystem &collectedApplePS,
             collectableObj &poisonedApple, bool& gamePaused, int lives, bool isGameOver = false)
{
    scoreBar.setFillColor(sf::Color(247, 152, 98));
    scoreBar.setPosition(0.f, 0.f);
    window.draw(scoreBar);
    
    backgroundSP.setPosition(0, scoreBarHeight);
    window.draw(backgroundSP);
    bodyPart* curr = snake.getHead();
    
    if(!isGameOver) {
        while(curr)
        {
            if(curr == snake.getHead()) {
                if(curr->isVisible){
                    head.setPosition(curr->x * cell_size_pix, curr->y * cell_size_pix);
                    window.draw(head);
                }
            }else{
                if(curr->isVisible){
                    snakeSP.setPosition(curr->x * cell_size_pix, curr->y * cell_size_pix);
                    window.draw(snakeSP);
                }
            }
            curr = curr->next;
        }
    }


    // drawing apple
    sf::Sprite tempAppleSP = appleSP;
    if(collObj.getIsGolden())
        tempAppleSP.setColor(sf::Color(255,215,0));    //golden aplle color
    else if(!collObj.getIsPoisoned())
        tempAppleSP.setColor(sf::Color(255, 0, 0));
    
    if(!isWon) {
        tempAppleSP.setPosition(collObj.getPosX() * cell_size_pix, collObj.getPosY()* cell_size_pix);
    }else
        tempAppleSP.setPosition(100 * cell_size_pix, 100 * cell_size_pix);
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
    if(snake.getHead()){
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
    if(withHealth) {
        for(int i = 0; i < lives; i++) {
            lifeSP.setPosition((GRID_SIZE_X - 3 - i) * cell_size_pix - 15*(i + 1), 16);
            window.draw(lifeSP);
        }
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
    lifeSP.setTexture(lifeTexture);
}

short run(std::string boardName = "")
{
    bool gamePaused = false;
    int frameCounter = 0;
    int lives = amountOfHealthes;
    score = 0;
    // Init game
    sf::Music backgroundMusic;    //used to play music in the background
    sf::SoundBuffer appleEatingSound;
    sf::Sound appleEating;
    sf::SoundBuffer gameOverSound;
    sf::Sound gameOver;
    sf::SoundBuffer winSound;
    sf::Sound win;
    
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Snake Game", sf::Style::Titlebar);
    
    if(isFirstGame) {
        initGame();
    }
    initMusic(backgroundMusic,appleEatingSound, appleEating, gameOverSound, gameOver, winSound, win);
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
    resizeSnake(snake, 2);
    if(withHealth)
        resizeSnake(snake, 3 * amountOfHealthes, false);

    collectableObj apple = collectableObj("apple", 0, 0, 1, 1);
    apple.goToFreeRandomPosistion(boardReader.wallHead, snake.getHead());
    collectableObj* AllCollectableObjs[10];

    AllCollectableObjs[0] = &apple;
    
    collectableObj poisonedApple("none", 0, 0 ,0 ,0);
    if(poisonedAppleOn)
    {
        poisonedApple = collectableObj("poisonedApple", 0, 0, -5, -1, true);
        poisonedApple.goToFreeRandomPosistion(boardReader.wallHead, snake.getHead(), AllCollectableObjs);
        poisonedApple.makePosion();
    }
    
    Direction newDir;
    newDir = snake.getDirection();
    int i = 0;

    //saving all objects snake can collide with
    int collisonObjsAmount = 1;
    if(poisonedAppleOn)
        collisonObjsAmount = 2;
    
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
                    if(withHealth){
                        lives--;
                        if(!snake.returnBack()){
                            gameOver.play();
                            sf::sleep(sf::milliseconds(2500));
                            GameOverText.setPosition((window_width - GameOverText.getLocalBounds().width) / 2, (window_height) / 2 - GameOverText.getLocalBounds().height - 20);
                            TryAgainText.setPosition((window_width - TryAgainText.getLocalBounds().width) / 2, (window_height - TryAgainText.getLocalBounds().height)  / 2 + 20);
                            ExitText.setPosition((window_width - ExitText.getLocalBounds().width) / 2, (window_height) / 2 + ExitText.getLocalBounds().height + 10);
                            window.draw(GameOverText);
                            window.draw(ExitText);
                            window.draw(TryAgainText);
                            window.display();
                            bool wasPressed = false;
                            sf::Event ev1;
                            while(window.pollEvent(ev1));
                            sf::Vector2i localMousePosition;
                            while(window.isOpen()) {
                                while(window.pollEvent(ev1)) {
                                    localMousePosition = sf::Mouse::getPosition(window);
                                    TryAgainText.setFillColor(sf::Color::Black);
                                    if(localMousePosition.x >= (window_width - TryAgainText.getLocalBounds().width) / 2
                                       && localMousePosition.x <= (window_width - TryAgainText.getLocalBounds().width) / 2 + TryAgainText.getLocalBounds().width
                                       && localMousePosition.y >= (window_height - TryAgainText.getLocalBounds().height) / 2 + 20 && localMousePosition.y <= (window_height - TryAgainText.getLocalBounds().height)  / 2 + 20 + TryAgainText.getLocalBounds().height) {
                                        TryAgainText.setFillColor(sf::Color(105, 105, 105));
                                    }
                                    ExitText.setFillColor(sf::Color::Black);
                                    if(localMousePosition.x >= (window_width - ExitText.getLocalBounds().width) / 2
                                       && localMousePosition.x <= (window_width - ExitText.getLocalBounds().width) / 2 + ExitText.getLocalBounds().width
                                       && localMousePosition.y >= (window_height) / 2 + ExitText.getLocalBounds().height + 10 && localMousePosition.y <= (window_height) / 2 + ExitText.getLocalBounds().height + 10 + ExitText.getLocalBounds().height) {
                                        ExitText.setFillColor(sf::Color(105, 105, 105));
                                    }
                                    
                                    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                                        localMousePosition = sf::Mouse::getPosition(window);
                                        if(localMousePosition.x >= (window_width - TryAgainText.getLocalBounds().width) / 2
                                           && localMousePosition.x <= (window_width - TryAgainText.getLocalBounds().width) / 2 + TryAgainText.getLocalBounds().width
                                           && localMousePosition.y >= (window_height - TryAgainText.getLocalBounds().height)  / 2 + 20 && localMousePosition.y <= (window_height - TryAgainText.getLocalBounds().height)  / 2 + 20 + TryAgainText.getLocalBounds().height) {
                                            saveHighScore();
                                            backgroundMusic.stop();
                                            deleteParticle(collectedApplePS);
                                            boardReader.~BoardReader();
                                            window.close();
                                            return 2;
                                        }else if(localMousePosition.x >= (window_width - ExitText.getLocalBounds().width) / 2
                                                 && localMousePosition.x <= (window_width - ExitText.getLocalBounds().width) / 2 + ExitText.getLocalBounds().width
                                                 && localMousePosition.y >= (window_height) / 2 + ExitText.getLocalBounds().height + 10 && localMousePosition.y <= (window_height) / 2 + ExitText.getLocalBounds().height + 10 + ExitText.getLocalBounds().height) {
                                            saveHighScore();
                                            backgroundMusic.stop();
                                            deleteParticle(collectedApplePS);
                                            boardReader.~BoardReader();
                                            window.close();
                                            return 1;
                                        }
                                    }
                                }
                                window.clear(sf::Color(153,204,255,100));
                                drawAll(window, snake, apple, collectedApplePS, poisonedApple, gamePaused, lives, true);
                                GameOverText.setPosition((window_width - GameOverText.getLocalBounds().width) / 2, (window_height) / 2 - GameOverText.getLocalBounds().height - 20);
                                TryAgainText.setPosition((window_width - TryAgainText.getLocalBounds().width) / 2, (window_height - TryAgainText.getLocalBounds().height)  / 2 + 20);
                                ExitText.setPosition((window_width - ExitText.getLocalBounds().width) / 2, (window_height) / 2 + ExitText.getLocalBounds().height + 10);
                                window.draw(GameOverText);
                                window.draw(ExitText);
                                window.draw(TryAgainText);
                                window.display();
                            }
                        }
                        window.clear(sf::Color(153,204,255,100));

                        collectedApplePS.blowUp();

                        drawAll(window, snake, apple, collectedApplePS, poisonedApple, gamePaused, lives);//, text);
                        sf::sleep(sf::milliseconds(1000));

                        window.display();
                        if(!snake.returnBack()){
                            gameOver.play();
                            sf::sleep(sf::milliseconds(2500));
                            saveHighScore();
                            backgroundMusic.stop();
                            deleteParticle(collectedApplePS);
                            boardReader.~BoardReader();
                            return 1;
                        }
                        window.clear(sf::Color(153,204,255,100));

                        collectedApplePS.blowUp();

                        drawAll(window, snake, apple, collectedApplePS, poisonedApple, gamePaused, lives);//, text);

                        window.display();
                        sf::sleep(sf::milliseconds(1000));
                        
                        if(!snake.returnBack()){
                            gameOver.play();
                            sf::sleep(sf::milliseconds(2500));
                            saveHighScore();
                            backgroundMusic.stop();
                            deleteParticle(collectedApplePS);
                            boardReader.~BoardReader();
                            return 1;
                        }
                        window.clear(sf::Color(153,204,255,100));

                        collectedApplePS.blowUp();

                        drawAll(window, snake, apple, collectedApplePS, poisonedApple, gamePaused, lives);//, text);
                        ChangeDirectionText.setPosition((window_width - ChangeDirectionText.getLocalBounds().width) / 2, (window_height - ChangeDirectionText.getLocalBounds().height) / 2);
                        window.draw(ChangeDirectionText);
                        window.display();
                        if(snake.getHead()->y == snake.getHead()->next->y){
                            if(snake.getHead()->x > snake.getHead()->next->x) {
                                snake.changeDirection(Direction::right);
                            }else {
                                snake.changeDirection(Direction::left);
                            }
                        }else if(snake.getHead()->x == snake.getHead()->next->x){
                            if(snake.getHead()->y > snake.getHead()->next->y) {
                                snake.changeDirection(Direction::down);
                            }else {
                                snake.changeDirection(Direction::up);
                            }
                        }
                        bool wasPressed = false;
                        sf::Event ev1;
                        while(window.pollEvent(ev1));
                        while(!wasPressed) {
                            while(window.pollEvent(ev1)) {
                                if(ev1.type == sf::Event::KeyPressed) {
                                    switch (ev1.key.code) {
                                        case sf::Keyboard::W:
                                            if(snake.getDirection() == Direction::down)
                                                break;
                                            newDir = Direction::up;
                                            wasPressed = true;
                                            break;
                                        case sf::Keyboard::S:
                                            if(snake.getDirection() == Direction::up)
                                                break;;
                                            newDir = Direction::down;
                                            wasPressed = true;
                                            break;
                                        case sf::Keyboard::A:
                                            if(snake.getDirection() == Direction::right)
                                                break;
                                            newDir = Direction::left;
                                            wasPressed = true;
                                            break;
                                        case sf::Keyboard::D:
                                            if(snake.getDirection() == Direction::left)
                                                break;
                                            newDir = Direction::right;
                                            wasPressed = true;
                                            break;
                                        default:
                                            break;
                                    }
                                }
                            }
                        }
                    }else {
                        gameOver.play();
                        sf::sleep(sf::milliseconds(2500));
                        GameOverText.setPosition((window_width - GameOverText.getLocalBounds().width) / 2, (window_height) / 2 - GameOverText.getLocalBounds().height - 20);
                        TryAgainText.setPosition((window_width - TryAgainText.getLocalBounds().width) / 2, (window_height - TryAgainText.getLocalBounds().height)  / 2 + 20);
                        ExitText.setPosition((window_width - ExitText.getLocalBounds().width) / 2, (window_height) / 2 + ExitText.getLocalBounds().height + 10);
                        window.draw(GameOverText);
                        window.draw(ExitText);
                        window.draw(TryAgainText);
                        window.display();
                        bool wasPressed = false;
                        sf::Event ev1;
                        while(window.pollEvent(ev1));
                        sf::Vector2i localMousePosition;
                        while(window.isOpen()) {
                            while(window.pollEvent(ev1)) {
                                localMousePosition = sf::Mouse::getPosition(window);
                                TryAgainText.setFillColor(sf::Color::Black);
                                if(localMousePosition.x >= (window_width - TryAgainText.getLocalBounds().width) / 2
                                   && localMousePosition.x <= (window_width - TryAgainText.getLocalBounds().width) / 2 + TryAgainText.getLocalBounds().width
                                   && localMousePosition.y >= (window_height - TryAgainText.getLocalBounds().height)  / 2 + 35 && localMousePosition.y <= (window_height - TryAgainText.getLocalBounds().height)  / 2 + 35 + TryAgainText.getLocalBounds().height) {
                                    TryAgainText.setFillColor(sf::Color(105, 105, 105));
                                }
                                ExitText.setFillColor(sf::Color::Black);
                                if(localMousePosition.x >= (window_width - ExitText.getLocalBounds().width) / 2
                                   && localMousePosition.x <= (window_width - ExitText.getLocalBounds().width) / 2 + ExitText.getLocalBounds().width
                                   && localMousePosition.y >= (window_height) / 2 + ExitText.getLocalBounds().height + 25 && localMousePosition.y <= (window_height) / 2 + ExitText.getLocalBounds().height + 25 + ExitText.getLocalBounds().height) {
                                    ExitText.setFillColor(sf::Color(105, 105, 105));
                                }
                                
                                if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                                    localMousePosition = sf::Mouse::getPosition(window);
                                    if(localMousePosition.x >= (window_width - TryAgainText.getLocalBounds().width) / 2
                                       && localMousePosition.x <= (window_width - TryAgainText.getLocalBounds().width) / 2 + TryAgainText.getLocalBounds().width
                                       && localMousePosition.y >= (window_height - TryAgainText.getLocalBounds().height)  / 2 + 35 && localMousePosition.y <= (window_height - TryAgainText.getLocalBounds().height)  / 2 + 35 + TryAgainText.getLocalBounds().height) {
                                        saveHighScore();
                                        backgroundMusic.stop();
                                        deleteParticle(collectedApplePS);
                                        boardReader.~BoardReader();
                                        window.close();
                                        return 2;
                                    }else if(localMousePosition.x >= (window_width - ExitText.getLocalBounds().width) / 2
                                             && localMousePosition.x <= (window_width - ExitText.getLocalBounds().width) / 2 + ExitText.getLocalBounds().width
                                             && localMousePosition.y >= (window_height) / 2 + ExitText.getLocalBounds().height + 25 && localMousePosition.y <= (window_height) / 2 + ExitText.getLocalBounds().height + 25 + ExitText.getLocalBounds().height) {
                                        saveHighScore();
                                        backgroundMusic.stop();
                                        deleteParticle(collectedApplePS);
                                        boardReader.~BoardReader();
                                        window.close();
                                        return 1;
                                    }
                                }
                            }
                            window.clear(sf::Color(153,204,255,100));
                            drawAll(window, snake, apple, collectedApplePS, poisonedApple, gamePaused, lives, true);
                            GameOverText.setPosition((window_width - GameOverText.getLocalBounds().width) / 2, (window_height) / 2 - GameOverText.getLocalBounds().height - 20);
                            TryAgainText.setPosition((window_width - TryAgainText.getLocalBounds().width) / 2, (window_height - TryAgainText.getLocalBounds().height)  / 2 + 20);
                            ExitText.setPosition((window_width - ExitText.getLocalBounds().width) / 2, (window_height) / 2 + ExitText.getLocalBounds().height + 10);
                            window.draw(GameOverText);
                            window.draw(ExitText);
                            window.draw(TryAgainText);
                            window.display();
                        }
                        
                    }
                }
                frameCounter = 0;
            }
        }
        
        window.clear(sf::Color(153,204,255,100));

        snakeHeadCollision(&snake, AllCollectableObjs, appleEating, collectedApplePS, collisonObjsAmount);
        
        collectedApplePS.blowUp();
        
        drawAll(window, snake, apple, collectedApplePS, poisonedApple, gamePaused, lives);//, text);
        
        window.display();
        if(isWon) {
            win.play();
            sf::sleep(sf::milliseconds(1500));
            YouWinText.setPosition((window_width - YouWinText.getLocalBounds().width) / 2, (window_height) / 2 - YouWinText.getLocalBounds().height - 20);
            TryAgainText.setPosition((window_width - TryAgainText.getLocalBounds().width) / 2, (window_height - TryAgainText.getLocalBounds().height)  / 2 + 20);
            ExitText.setPosition((window_width - ExitText.getLocalBounds().width) / 2, (window_height) / 2 + ExitText.getLocalBounds().height + 10);
            window.draw(YouWinText);
            window.draw(ExitText);
            window.draw(TryAgainText);
            window.display();
            bool wasPressed = false;
            sf::Event ev1;
            while(window.pollEvent(ev1));
            sf::Vector2i localMousePosition;
            while(window.isOpen()) {
                while(window.pollEvent(ev1)) {
                    localMousePosition = sf::Mouse::getPosition(window);
                    TryAgainText.setFillColor(sf::Color::Black);
                    if(localMousePosition.x >= (window_width - TryAgainText.getLocalBounds().width) / 2
                       && localMousePosition.x <= (window_width - TryAgainText.getLocalBounds().width) / 2 + TryAgainText.getLocalBounds().width
                       && localMousePosition.y >= (window_height - TryAgainText.getLocalBounds().height)  / 2 + 35 && localMousePosition.y <= (window_height - TryAgainText.getLocalBounds().height)  / 2 + 35 + TryAgainText.getLocalBounds().height) {
                        TryAgainText.setFillColor(sf::Color(105, 105, 105));
                    }
                    ExitText.setFillColor(sf::Color::Black);
                    if(localMousePosition.x >= (window_width - ExitText.getLocalBounds().width) / 2
                       && localMousePosition.x <= (window_width - ExitText.getLocalBounds().width) / 2 + ExitText.getLocalBounds().width
                       && localMousePosition.y >= (window_height) / 2 + ExitText.getLocalBounds().height + 25 && localMousePosition.y <= (window_height) / 2 + ExitText.getLocalBounds().height + 25 + ExitText.getLocalBounds().height) {
                        ExitText.setFillColor(sf::Color(105, 105, 105));
                    }
                    
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                        localMousePosition = sf::Mouse::getPosition(window);
                        if(localMousePosition.x >= (window_width - TryAgainText.getLocalBounds().width) / 2
                           && localMousePosition.x <= (window_width - TryAgainText.getLocalBounds().width) / 2 + TryAgainText.getLocalBounds().width
                           && localMousePosition.y >= (window_height - TryAgainText.getLocalBounds().height)  / 2 + 35 && localMousePosition.y <= (window_height - TryAgainText.getLocalBounds().height)  / 2 + 35 + TryAgainText.getLocalBounds().height) {
                            saveHighScore();
                            backgroundMusic.stop();
                            deleteParticle(collectedApplePS);
                            boardReader.~BoardReader();
                            window.close();
                            return 2;
                        }else if(localMousePosition.x >= (window_width - ExitText.getLocalBounds().width) / 2
                                 && localMousePosition.x <= (window_width - ExitText.getLocalBounds().width) / 2 + ExitText.getLocalBounds().width
                                 && localMousePosition.y >= (window_height) / 2 + ExitText.getLocalBounds().height + 25 && localMousePosition.y <= (window_height) / 2 + ExitText.getLocalBounds().height + 25 + ExitText.getLocalBounds().height) {
                            saveHighScore();
                            backgroundMusic.stop();
                            deleteParticle(collectedApplePS);
                            boardReader.~BoardReader();
                            window.close();
                            return 1;
                        }
                    }
                }
                window.clear(sf::Color(153,204,255,100));
                drawAll(window, snake, apple, collectedApplePS, poisonedApple, gamePaused, lives, true);
                YouWinText.setPosition((window_width - YouWinText.getLocalBounds().width) / 2, (window_height) / 2 - YouWinText.getLocalBounds().height - 20);
                TryAgainText.setPosition((window_width - TryAgainText.getLocalBounds().width) / 2, (window_height - TryAgainText.getLocalBounds().height)  / 2 + 20);
                ExitText.setPosition((window_width - ExitText.getLocalBounds().width) / 2, (window_height) / 2 + ExitText.getLocalBounds().height + 10);
                window.draw(YouWinText);
                window.draw(ExitText);
                window.draw(TryAgainText);
                window.display();
            }
        }
        if(snake.getHead()->next == nullptr || !snake.getHead()->next->isVisible){
            gameOver.play();
            sf::sleep(sf::milliseconds(2000));
            GameOverText.setPosition((window_width - GameOverText.getLocalBounds().width) / 2, (window_height) / 2 - GameOverText.getLocalBounds().height - 20);
            TryAgainText.setPosition((window_width - TryAgainText.getLocalBounds().width) / 2, (window_height - TryAgainText.getLocalBounds().height)  / 2 + 20);
            ExitText.setPosition((window_width - ExitText.getLocalBounds().width) / 2, (window_height) / 2 + ExitText.getLocalBounds().height + 10);
            window.draw(GameOverText);
            window.draw(ExitText);
            window.draw(TryAgainText);
            window.display();
            bool wasPressed = false;
            sf::Event ev1;
            while(window.pollEvent(ev1));
            sf::Vector2i localMousePosition;
            while(window.isOpen()) {
                while(window.pollEvent(ev1)) {
                    localMousePosition = sf::Mouse::getPosition(window);
                    TryAgainText.setFillColor(sf::Color::Black);
                    if(localMousePosition.x >= (window_width - TryAgainText.getLocalBounds().width) / 2
                       && localMousePosition.x <= (window_width - TryAgainText.getLocalBounds().width) / 2 + TryAgainText.getLocalBounds().width
                       && localMousePosition.y >= (window_height - TryAgainText.getLocalBounds().height)  / 2 + 35 && localMousePosition.y <= (window_height - TryAgainText.getLocalBounds().height)  / 2 + 35 + TryAgainText.getLocalBounds().height) {
                        TryAgainText.setFillColor(sf::Color(105, 105, 105));
                    }
                    ExitText.setFillColor(sf::Color::Black);
                    if(localMousePosition.x >= (window_width - ExitText.getLocalBounds().width) / 2
                       && localMousePosition.x <= (window_width - ExitText.getLocalBounds().width) / 2 + ExitText.getLocalBounds().width
                       && localMousePosition.y >= (window_height) / 2 + ExitText.getLocalBounds().height + 25 && localMousePosition.y <= (window_height) / 2 + ExitText.getLocalBounds().height + 25 + ExitText.getLocalBounds().height) {
                        ExitText.setFillColor(sf::Color(105, 105, 105));
                    }
                    
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                        localMousePosition = sf::Mouse::getPosition(window);
                        if(localMousePosition.x >= (window_width - TryAgainText.getLocalBounds().width) / 2
                           && localMousePosition.x <= (window_width - TryAgainText.getLocalBounds().width) / 2 + TryAgainText.getLocalBounds().width
                           && localMousePosition.y >= (window_height - TryAgainText.getLocalBounds().height)  / 2 + 35 && localMousePosition.y <= (window_height - TryAgainText.getLocalBounds().height)  / 2 + 35 + TryAgainText.getLocalBounds().height) {
                            saveHighScore();
                            backgroundMusic.stop();
                            deleteParticle(collectedApplePS);
                            boardReader.~BoardReader();
                            window.close();
                            return 2;
                        }else if(localMousePosition.x >= (window_width - ExitText.getLocalBounds().width) / 2
                                 && localMousePosition.x <= (window_width - ExitText.getLocalBounds().width) / 2 + ExitText.getLocalBounds().width
                                 && localMousePosition.y >= (window_height) / 2 + ExitText.getLocalBounds().height + 25 && localMousePosition.y <= (window_height) / 2 + ExitText.getLocalBounds().height + 25 + ExitText.getLocalBounds().height) {
                            saveHighScore();
                            backgroundMusic.stop();
                            deleteParticle(collectedApplePS);
                            boardReader.~BoardReader();
                            window.close();
                            return 1;
                        }
                    }
                }
                window.clear(sf::Color(153,204,255,100));
                drawAll(window, snake, apple, collectedApplePS, poisonedApple, gamePaused, lives, true);
                GameOverText.setPosition((window_width - GameOverText.getLocalBounds().width) / 2, (window_height) / 2 - GameOverText.getLocalBounds().height - 20);
                TryAgainText.setPosition((window_width - TryAgainText.getLocalBounds().width) / 2, (window_height - TryAgainText.getLocalBounds().height)  / 2 + 20);
                ExitText.setPosition((window_width - ExitText.getLocalBounds().width) / 2, (window_height) / 2 + ExitText.getLocalBounds().height + 10);
                window.draw(GameOverText);
                window.draw(ExitText);
                window.draw(TryAgainText);
                window.display();
            }
        }
        sf::sleep(sf::milliseconds(frameFreezeTime));
        frameCounter++;
    }
    boardReader.~BoardReader();
    saveHighScore();
    deleteParticle(collectedApplePS);
    backgroundMusic.stop();
    
    
    
    return 1;       //lost, run again
}
