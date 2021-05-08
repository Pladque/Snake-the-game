#include "snake.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>

const int cell_size_pix = 32;
const int window_width = GRID_SIZE_X * cell_size_pix;
const int window_height = GRID_SIZE_Y * cell_size_pix;

int snake_x = GRID_SIZE_X / 2;
int snake_y = GRID_SIZE_Y / 2;

void clearField(Snake& snake, int size){
    for(int i = 0; i < size; i++)
        snake.grow();
}

//int getEmptyCell(){
//    int countEmpty = 0;
//    for (int i = 0; i < field_y; i++) {
//        for (int j = 0; j < field_x; j++) {
//            if(field[i][j] == NONE){
//                countEmpty++;
//            }
//        }
//    }
//    int indexEmpty = rand() % countEmpty;
//    
//    for (int i = 0; i < field_y; i++) {
//        for (int j = 0; j < field_x; j++) {
//            if(field[i][j] == NONE){
//                if(countEmpty == indexEmpty) {
//                    
//                }
//            }
//        }
//    }
//}

void addApple(){
    
}

void drawField(sf::RenderWindow& window, Snake& snake) {
    sf::Texture noneTexture;
    noneTexture.loadFromFile("none.png");
    sf::Sprite none;
    none.setTexture(noneTexture);
    
    sf::Texture snakeTexture;
    snakeTexture.loadFromFile("snake.png");
    sf::Sprite snakeSP;
    snakeSP.setTexture(snakeTexture);
    
    sf::Texture headTexture;
    headTexture.loadFromFile("head.png");
    sf::Sprite head;
    head.setTexture(headTexture);
    
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
int main(int, char const**)
{
    // Init game
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Snake Game", sf::Style::Titlebar | sf::Style::Close);
    //Game loop
    srand(time(NULL));
    Snake snake(snake_x, snake_y);
    snake.changeDirection(Direction::left);
    clearField(snake, 5);
    
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

        snake.move();
        
        window.clear(sf::Color::White);
        
        drawField(window, snake);
        
        window.display();
        sf::sleep(sf::milliseconds(100));
    }
    
    return EXIT_SUCCESS;
}
