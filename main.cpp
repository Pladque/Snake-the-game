
#include "snake.cpp"
#include "collectableObj.cpp"
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

void resizeSnake(Snake& snake, int size){
    for(int i = 0; i < size; i++)
        snake.grow();
}


void updateScore(int addToScore){
    score += addToScore;
    text.setString(std::to_string(score));
}

// mby rework it, to check collision with array of
//collectableObj - it will allow us to add fe poison apple, bombs etc
// and other staff that snake can collide with
void snakeHeadCollision(Snake *snake, collectableObj *obj1){
    
    if(snake->getHead()->x == obj1->getPosX() &&  snake->getHead()->y == obj1->getPosY() ){
//        std::cout<<score<<std::endl;
        // we can add golden aplles, that will f.e add 2 to size
        // and 5 to score
        resizeSnake(*snake, obj1->getSizeBonus());
        updateScore(obj1->getScoreBonus());
        obj1->goToFreeRandomPosistion(snake->getHead());
    }
}

void drawField(sf::RenderWindow& window, Snake& snake, 
                collectableObj& collObj)// sf::Text &text) 
{  
    sf::Texture snakeTexture;
    snakeTexture.loadFromFile("snake.png");
    sf::Sprite snakeSP;
    snakeSP.setTexture(snakeTexture);
    
    sf::Texture headTexture;
    headTexture.loadFromFile("head.png");
    sf::Sprite head;
    head.setTexture(headTexture);

    sf::Texture appleTexture;
    appleTexture.loadFromFile("apple.png");
    sf::Sprite appleSP;
    appleSP.setTexture(appleTexture);
    
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

}



void handleKey(Snake& snake) {
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        if(snake.getDirection() == Direction::up || snake.getDirection() == Direction::down)
            return;
        snake.changeDirection(Direction::up);
    }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        if(snake.getDirection() == Direction::down || snake.getDirection() == Direction::up)
            return;
        snake.changeDirection(Direction::down);
    }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        if(snake.getDirection() == Direction::left || snake.getDirection() == Direction::right)
            return;
        snake.changeDirection(Direction::left);
    }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        if(snake.getDirection() == Direction::right || snake.getDirection() == Direction::left)
            return;
        snake.changeDirection(Direction::right);
    }
}


void setupScoreDisplayer( sf::Font &font)
{
    text.setFont(font); // font is a sf::Font
    text.setString(std::to_string(score));
    text.setCharacterSize(48); // in pixels, not points!
    text.setFillColor(sf::Color::Black);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);

}


int main(int, char const**)
{
    // Init game
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Snake Game", sf::Style::Titlebar | sf::Style::Close);
    
    //loading font
    sf::Font font;
    if (!font.loadFromFile("cabin-sketch.bold.ttf"))
    {
        std::cout<<"Font file missing"<<std::endl;
    }
    
    setupScoreDisplayer(font);
    
    //Game loop
    srand(time(NULL));
    Snake snake = Snake(snake_x, snake_y);
    snake.changeDirection(Direction::left);
    resizeSnake(snake, 5);

    collectableObj apple = collectableObj("apple", 0, 0, 1, 1);
    apple.goToFreeRandomPosistion();

    while(window.isOpen()){
        sf::Event ev;
        
        while(window.pollEvent(ev)) {
            
            if(ev.type == sf::Event::Closed) {
                window.close();
            }else if(ev.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }
        
        handleKey(snake);

        if(!snake.move())
            std::cout<<"GAME OVER"<<std::endl;
        
        window.clear(sf::Color(153,204,255,100));
        
        drawField(window, snake, apple);//, text);

        snakeHeadCollision(&snake, &apple);
        
        window.display();
        sf::sleep(sf::milliseconds(100));
    }
    
    return EXIT_SUCCESS;
}
