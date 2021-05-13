
#include "snake.cpp"
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
sf::Text text;      // for displaying score

sf::Texture snakeTexture;
sf::Sprite snakeSP;
sf::Texture headTexture;
sf::Sprite head;
sf::Texture appleTexture;
sf::Sprite appleSP;

const int window_width = GRID_SIZE_X * cell_size_pix;
const int window_height = GRID_SIZE_Y * cell_size_pix;



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
void resizeSnake(Snake& snake, int size){
    for(int i = 0; i < size; i++)
        snake.grow();
}

PartycleSystem makeParticles(int particleDense = 100)
{
    srand (time(NULL));
    Particle* firstParticle = 
     new Particle(
        rand() % 3, //speedX
        rand() % 3   //speedY
        , 2 + rand() % 2,      //ttl
        2+ rand() % 3,      //size
        1 ,                     //color
        rand() % 5,   //velocityX
        rand() % 5);   //velocityY


    Particle* currParticle = firstParticle;

    for(int i = 0; i < particleDense; i++)
    {
        currParticle -> next = new Particle(
        rand() % 3 - 1, //speedX
        rand() % 3 - 1  //speedY
        , 2 + rand() % 2,      //ttl
        2+ rand() % 3,      //size
        1 ,                     //color
        rand() % 5 / 10 ,   //velocityX
        rand() % 5 / 10);   //velocityY


        currParticle = currParticle ->next; 
        currParticle ->next = nullptr;
    }

    PartycleSystem collectedApplePS = PartycleSystem(firstParticle, 0, 0, 0);
    return collectedApplePS;
}


void updateScore(int addToScore){
    score += addToScore;
    text.setString("Score: " + std::to_string(score));
}

// mby rework it, to check collision with array of
//collectableObj - it will allow us to add fe poison apple, bombs etc
// and other staff that snake can collide with

void snakeHeadCollision(Snake *snake, collectableObj *obj1, 
    sf::Sound& appleEating, PartycleSystem &appleEatingPS){
    
    if(snake->getHead()->x == obj1->getPosX() &&  snake->getHead()->y == obj1->getPosY() ){
//        std::cout<<score<<std::endl;
        // we can add golden aplles, that will f.e add 2 to size
        // and 5 to score
        appleEatingPS = makeParticles();
        appleEatingPS.setPosition(obj1 ->getPosX() * cell_size_pix, 
        obj1 ->getPosY() * cell_size_pix);

        appleEating.play();
        resizeSnake(*snake, obj1->getSizeBonus());
        updateScore(obj1->getScoreBonus());
        obj1->goToFreeRandomPosistion(snake->getHead());

    }
}

void drawField(sf::RenderWindow& window, Snake& snake, 
                collectableObj& collObj, PartycleSystem &collectedApplePS)// sf::Text &text) 
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

    //drawing score
    text.setPosition(20, 20);   
    window.draw(text);


    //drawing particle effect
    Particle* particleToDraw = collectedApplePS.getFirstParticle();

    while(particleToDraw)
    {
        if(particleToDraw ->ttl >=0)
        {
            sf::CircleShape shape(particleToDraw->size);
            shape.setFillColor(sf::Color(255, 0, 0));
            shape.setPosition(
            (collectedApplePS.getPosX() + particleToDraw->positionX), 
            (collectedApplePS.getPosY() + particleToDraw->positionY));
            
            window.draw(shape);
        }
        particleToDraw = particleToDraw->next;
    }

    //*/

}

void setupScoreDisplayer( sf::Font &font)
{
    text.setFont(font); // font is a sf::Font
    text.setString("Score: " + std::to_string(score));
    text.setCharacterSize(48); // in pixels, not points!
    text.setFillColor(sf::Color::Black);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);

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



int run()
{
    bool gamePaused = false;
    int frameCounter = 0;
    // Init game
    sf::SoundBuffer appleEatingSound;
    sf::Sound appleEating;
    appleEating.setVolume(40);
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Snake Game", sf::Style::Titlebar | sf::Style::Close);
    initGame(appleEatingSound, appleEating);

    //loading font
    sf::Font font;
    if (!font.loadFromFile(FONTS_PATH+"cabin-sketch.bold.ttf"))
    {
        std::cout<<"Font file missing"<<std::endl;
    }
    
    setupScoreDisplayer(font);

    //DONT DELETE plz
    PartycleSystem collectedApplePS = makeParticles();
    
    //Game loop
    srand(time(NULL));
    Snake snake = Snake(snake_x, snake_y);
    snake.changeDirection(Direction::left);
    resizeSnake(snake, 5);

    collectableObj apple = collectableObj("apple", 0, 0, 1, 1);
    apple.goToFreeRandomPosistion();
    
    Direction newDir;
    newDir = snake.getDirection();
    while(window.isOpen()){

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
                    window.close();
                }
                frameCounter = 0;
            }
        }

        window.clear(sf::Color(153,204,255,100));
        
        drawField(window, snake, apple, collectedApplePS);//, text);

        collectedApplePS.blowUp();

        snakeHeadCollision(&snake, &apple, appleEating, collectedApplePS);
            
        window.display();
        sf::sleep(sf::milliseconds(frameFreezeTime));
        frameCounter++;
    }
    
    
    return EXIT_SUCCESS;
}
