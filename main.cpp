
#include "snake.cpp"
#include "collectableObj.cpp"
#include "particle.cpp"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
//DEBUG:
#include <iostream>
#include <string>

const int cell_size_pix = 32;
const int window_width = GRID_SIZE_X * cell_size_pix;
const int window_height = GRID_SIZE_Y * cell_size_pix;

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



void initGame(sf::SoundBuffer& appleEatingSound, sf::Sound& appleEating) {
    snakeTexture.loadFromFile("snake.png");
    snakeSP.setTexture(snakeTexture);
    headTexture.loadFromFile("head.png");
    head.setTexture(headTexture);
    appleTexture.loadFromFile("apple.png");
    appleSP.setTexture(appleTexture);
    appleEatingSound.loadFromFile("applebite.wav");
    appleEating.setBuffer(appleEatingSound);
}
void resizeSnake(Snake& snake, int size){
    for(int i = 0; i < size; i++)
        snake.grow();
}

PartycleSystem makeParticles()
{
    srand (time(NULL));
    Particle* firstParticle = 
    new Particle(2 + rand() % 3 / 10, 2 + rand() % 3 / 10
    , 10 + rand() % 2, 2+ rand() % 3 - 4, 1 , 0.1 + rand() % 3 /10 - 0.2,
     0.1+ rand() % 3 /10 - 0.2);
    Particle* currParticle = firstParticle;

    for(int i = 0; i < 10; i++)
    {
        currParticle -> next = new Particle(0.5 + rand() % 3 / 10, 0.5 + rand() % 3 / 10
    , 5 + rand() % 2, 5+ rand() % 3, 1 , 0.1 + rand() % 3 /10 - 0.2,
     0.1+ rand() % 3 /10 - 0.2);
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

void snakeHeadCollision(Snake *snake, collectableObj *obj1, sf::Sound& appleEating){
    
    if(snake->getHead()->x == obj1->getPosX() &&  snake->getHead()->y == obj1->getPosY() ){
//        std::cout<<score<<std::endl;
        // we can add golden aplles, that will f.e add 2 to size
        // and 5 to score
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
    appleSP.setPosition(collObj.getPosX() * cell_size_pix, collObj.getPosY()* cell_size_pix);
    window.draw(appleSP);

    //drawing score
    text.setPosition(20, 20);   
    window.draw(text);


    //drawing particle effect
    //I will fix it later xd
    //DONT DELETE plz
    /*
    Particle* particleToDraw = collectedApplePS.getFirstParticle();

    while(particleToDraw)
    {
        if(particleToDraw ->ttl >=0)
        {
            sf::CircleShape shape(particleToDraw->size);
            shape.setFillColor(sf::Color(255, 0, 0));
            shape.setPosition((collObj.getPosX() + particleToDraw->positionX) * cell_size_pix, 
            (collObj.getPosY()+ particleToDraw->positionY)* cell_size_pix);
            window.draw(shape);
        }
        particleToDraw = particleToDraw->next;
    }

    */

}

void setupScoreDisplayer( sf::Font &font)
{
    text.setFont(font); // font is a sf::Font
    text.setString("Score: " + std::to_string(score));
    text.setCharacterSize(48); // in pixels, not points!
    text.setFillColor(sf::Color::Black);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);

}

bool gamePaused = false;
int main(int, char const**)
{
    // Init game
    sf::SoundBuffer appleEatingSound;
    sf::Sound appleEating;
    appleEating.setVolume(40);
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Snake Game", sf::Style::Titlebar | sf::Style::Close);
    initGame(appleEatingSound, appleEating);
    //loading font
    sf::Font font;
    if (!font.loadFromFile("cabin-sketch.bold.ttf"))
    {
        std::cout<<"Font file missing"<<std::endl;
    }
    
    setupScoreDisplayer(font);

    //DONT DELETE plz
    //PartycleSystem collectedApplePS = makeParticles();
    
    //Game loop
    srand(time(NULL));
    Snake snake = Snake(snake_x, snake_y);
    snake.changeDirection(Direction::left);
    resizeSnake(snake, 5);

    collectableObj apple = collectableObj("apple", 0, 0, 1, 1);
    apple.goToFreeRandomPosistion();
    
    Direction newDir;
    while(window.isOpen()){
        sf::Event ev;
        newDir = snake.getDirection();
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
//        if(!gamePaused){
            snake.changeDirection(newDir);
        if(!snake.move()){
            sf::SoundBuffer gameOverSound;
            sf::Sound gameOver;
            gameOverSound.loadFromFile("gameover2.wav");
            gameOver.setBuffer(gameOverSound);
            gameOver.setVolume(50);
            gameOver.play();
            sf::sleep(sf::milliseconds(2500));
            window.close();
        }
//        }
        window.clear(sf::Color(153,204,255,100));
        
        drawField(window, snake, apple, collectedApplePS);//, text);

        collectedApplePS.blowUp();

        snakeHeadCollision(&snake, &apple, appleEating);
            
        
            
        window.display();
        sf::sleep(sf::milliseconds(100));
    }
    
    
    return EXIT_SUCCESS;
}
