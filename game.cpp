
#include "snake.cpp"
#include "BoardReader.cpp"
#include "collectableObj.cpp"
#include "particle.cpp"


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

sf::Text ScoreText;      // for displaying score
sf::Text HighScoreText;      // for displaying best score

sf::Texture snakeTexture;
sf::Sprite snakeSP;
sf::Texture headTexture;
sf::Sprite head;
sf::Texture appleTexture;
sf::Sprite appleSP;

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
void initGame(sf::SoundBuffer& appleEatingSound, sf::Sound& appleEating) {
    snakeTexture.loadFromFile(TEXTURES_PATH+"snake.png");
    snakeSP.setTexture(snakeTexture);
    headTexture.loadFromFile(TEXTURES_PATH+"head.png");
    head.setTexture(headTexture);
    appleTexture.loadFromFile(TEXTURES_PATH+"apple.png");
    appleSP.setTexture(appleTexture);
    appleEatingSound.loadFromFile(SOUNDS_PATH+"applebite.wav");
    appleEating.setBuffer(appleEatingSound);
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
        for(int i = 0; i < size; i++)
            snake.fade();
    }
    
}

PartycleSystem makeParticles(int particleDense = 100)
{
    srand (time(NULL));
    Particle* firstParticle = 
     new Particle(
        rand() % 200 / 100.0, //speedX
        rand() %  200 / 100.0   //speedY
        , 2.5 + rand() % 100 / 100.0,      //ttl
        1.5 + rand() % 300 / 1000.0,      //size
        1 ,                     //color
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
        1 ,                     //color
        rand() % 50/100.0 - 0.25,   //velocityX
        rand() % 50/100.0 - 0.25);   //velocityY


        currParticle = currParticle ->next; 
        currParticle ->next = nullptr;
    }

    PartycleSystem collectedApplePS = PartycleSystem(firstParticle, 0, 0, 0);
    return collectedApplePS;
}


void updateScore(int addToScore){
    score += addToScore;
    ScoreText.setString("Score: " + std::to_string(score));
}

// mby rework it, to check collision with array of
//collectableObj - it will allow us to add fe poison apple, bombs etc
// and other staff that snake can collide with

void snakeHeadCollision(Snake *snake, collectableObj* objects[], 
    sf::Sound& appleEating, PartycleSystem &appleEatingPS, int collObjAmount = 0)
{
    
    
    for(int i = 0; i <  collObjAmount; i++)
    {
        
        if(snake->getHead()->x == objects[i]->getPosX() &&  snake->getHead()->y == objects[i]->getPosY() )
        {
            std::cout<<objects[i]->getScoreBonus()<<std::endl;
            appleEatingPS = makeParticles();
            appleEatingPS.setPosition(objects[i] ->getPosX() * cell_size_pix, 
            objects[i] ->getPosY() * cell_size_pix);

            appleEating.play();
            resizeSnake(*snake, objects[i]->getSizeBonus());
            updateScore(objects[i]->getScoreBonus());
            objects[i]->goToFreeRandomPosistion(boardReader.wallHead, snake->getHead());


        }
    }
}

void drawField(sf::RenderWindow& window, Snake& snake, 

                collectableObj& collObj, PartycleSystem &collectedApplePS)// sf::Text &ScoreText) 

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
    else
        tempAppleSP.setColor(sf::Color(255, 0, 0));

    tempAppleSP.setPosition(collObj.getPosX() * cell_size_pix, collObj.getPosY()* cell_size_pix);
    window.draw(tempAppleSP);

    if(poisonedAppleOn)
    {
        tempAppleSP = appleSP;
        tempAppleSP.setColor(sf::Color(0,215,0));

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
            if(!collObj.getPrevIsGolden()){
                shape.setFillColor(sf::Color(255, 0, 0));
            }else
                shape.setFillColor(sf::Color(255,215,0));
            shape.setPosition(
            (snake.getHead()->x * cell_size_pix + particleToDraw->positionX + cell_size_pix/2), 
            (snake.getHead()->y * cell_size_pix + particleToDraw->positionY + cell_size_pix/2));
            
            window.draw(shape);
        }
        particleToDraw = particleToDraw->next;
    }

    //*/

    //Drawing board (with walls)

    wall* currWall = boardReader.wallHead;

    while(currWall!= nullptr)
    {
        //temp, to show how reading from boardReader works
        sf::CircleShape shape(10);

         shape.setPosition(
            currWall->getX() * cell_size_pix, 
            currWall->getY() * cell_size_pix);

        shape.setFillColor(sf::Color(255, 255, 255));

        window.draw(shape);

        currWall = currWall->getNext();
    }

}

void setupScoreDisplayer( sf::Font &font)
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

void windowPollEvent(sf::RenderWindow &window,
sf::Event &ev, Direction &newDir, Snake &snake, bool &gamePaused)
{
    while(window.pollEvent(ev)) {
                if(ev.type == sf::Event::Closed) {
                    window.close();
                }else if(ev.key.code == sf::Keyboard::Escape) {
                    gamePaused = true;
                }else if(ev.type == sf::Event::KeyPressed) {
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
                        default:
                            break;
                    }
                }
    }
}



int run(std::string boardName = "")
{
    bool gamePaused = false;
    int frameCounter = 0;
    // Init game
    sf::SoundBuffer appleEatingSound;
    sf::Sound appleEating;
    appleEating.setVolume(40);
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Snake Game", sf::Style::Titlebar | sf::Style::Close);
    initGame(appleEatingSound, appleEating);

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

    //Loading highScore
    std::fstream highScoreFile;
    std::string hishScoreFileFullPath = SAVES_PATH + HIGH_SCORE_FILE_NAME;
    highScoreFile.open(hishScoreFileFullPath.c_str(), std::ios::in | std::ios::binary);

    highScore = 0;
    
    highScoreFile.read((char *) &highScore, sizeof(highScore));

    highScoreFile.close();
    //////////////////////////////////
    
    setupScoreDisplayer(font);

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
        poisonedApple = collectableObj("poisonedApple", 0, 0, -5, -1);
        poisonedApple.goToFreeRandomPosistion(boardReader.wallHead, snake.getHead());
        poisonedApple.makePosion();
    }

    Direction newDir;
    newDir = snake.getDirection();
    int i = 0;

    //saving all objects snake can collide with
    int collisonObjsAmount = 2;
    collectableObj* AllCollectableObjs[10];

    AllCollectableObjs[0] = &apple;
    if(poisonedAppleOn)
        AllCollectableObjs[1] = &poisonedApple;


    while(window.isOpen()){
        i++;
        sf::Event ev;
        
        windowPollEvent(window, ev, newDir, snake, gamePaused);
        
        if(!gamePaused){
            
            if(frameCounter % snakeSpeed == 0)
            {      
                snake.changeDirection(newDir);
                if(!snake.move()){
                    sf::SoundBuffer gameOverSound;
                    sf::Sound gameOver;
                    gameOverSound.loadFromFile(SOUNDS_PATH + "gameover2.wav");
                    gameOver.setBuffer(gameOverSound);
                    gameOver.setVolume(50);
                    gameOver.play();
                    sf::sleep(sf::milliseconds(2500));
                    break;
                    //window.close();
                }
                frameCounter = 0;
            }
        }

        window.clear(sf::Color(153,204,255,100));
        

        drawField(window, snake, apple, collectedApplePS);//, ScoreText);


        snakeHeadCollision(&snake, AllCollectableObjs, appleEating, collectedApplePS,collisonObjsAmount);
        collectedApplePS.blowUp();
        window.display();
        sf::sleep(sf::milliseconds(frameFreezeTime));
        frameCounter++;
    }
    
    if(score > highScore)
    {
    
        highScoreFile.open(hishScoreFileFullPath.c_str(), 
                    std::ios::out | std::ios::trunc | std::ios::binary);
        std::string scoreAsString = std::to_string(score);

        highScoreFile.write((char *) &score, sizeof(score));

        highScoreFile.close();
    }

    deleteParticle(collectedApplePS);



    
    return EXIT_SUCCESS;
}
