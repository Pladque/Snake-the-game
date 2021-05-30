# Snake-the-game

#Check list

- [X] Background sprite for game    (15.05.2021)

- [X] Walls   (16.05.2021)

- [X] Settings in menu      (13.05.2021)

- [X] pause menu     (15.05.2021)

- [X] high score (aka save system) (13.05.2021)

- [X] mode with opponent (mby like snake.io gameplay??)

- [X] 3 types of apples (poisoned, golden, normal)    (14.05.2021)
  
- [X] menu with mouse input     (14.05.2021)

- [X] Game window size depends on screen resolution (14.05.2021)

- [X] Fix particle system  (13.05.2021)

- [X] GameBoards to choose  (14.05.2021)

- [X] Menu after death   (18.05.2021)

- [X] Game saving system

## Update v1.2 ##

Changes since 1.1:

*Added possobility to pause during the game and/or quit to main menu (special symbols display on the window)

*Added possibility to try again/quit to menu after snake death

*Added golden and poisoned apples with different properties (golden adds 5x score, 2x size, poisoned negative 5x score, decreases snake size)

*New game boards, which player can choose in main menu

*Expanded main menu, player can operate with mouse or keyboard

*High score system, program will save and display high score during the game, current score also being displayed on top of the window

*New textures added (boards, apples, walls)

*New health system, current health being displayed during the game, correlated with difficulty level chosen by the player, snake returns to position before collision if having additional "hearts"

*Snake speed depends on difficulty level

*Poisoned apple changes it's position after consuming either one apple, fixed some bugs with poisoned/golden apples malfunction

*Many other small fixes/upgrades

*Difficulties: adding another snake

## Update v1.1 ##

Changes since 1.0:

*snake.c/hpp - main snake class. Snake is represented as the linked list, with one distinguished element, called the snake head. Other segments of program could interfere with a head, using methods of this class. Whole body contain set of single elements, belonging to the snake list, which are made to "follow" the head whenever it moves. Methods in this class allow the snake to grow, change directions and fade.

*settings.cpp - this file contains basic game configuration, such as snake speed, grid size, score bonuses, etc. Settings should be able to be altered from options in game main menu.

*collectableObj - fruit class, reserving space for fruit on the map, ensures to avoid generating fruit on the snake body/head. Methods of this class are mainly getters to class fields, needed by other segments of the code.

*consts.cpp - file with constants inlcuded in game. Contains paths to textures/sounds, snake directions mapping, etc.

*particle.cpp - experimental file, containing some "extra" effects in the game, while eating fruits etc. 

*game.cpp - game main body. Game window is generated here, with snake and fruits/other objects being rendered on the grid, player score is also displayed in the top of the window. It includes other configuration files, and libraries required for graphics/sound effects. Whole interaction with user, ie. moving snake with keystrokes etc, takes place here. Methods of previously included configuration classes are invoked here, to interact with adequates objects.

*main.cpp - main file, providing main menu and basic interaction with user, game.cpp is included here. Options/settings will be accesible from this part of the code. If user decides to start the game, game.cpp will be started.

*Difficulties: creating particles system, buges with windows dimensions

## Update v1.0 ##

1. This application, written in C++, simulates the standard "Snake Game", in which player takes control over the creature, avoiding collapsing with any object, including the snake itself. The goal is to feed the snake with fruits/animals, making it grow bigger, and scoring points for keeping it safe.

2. Done: 
*basic Snake class, which defines snake behavior/properties
*configuration of SFML, which will be used in the future to implement some graphic features
*Snake representation rendering on the map,
*pseudo random fruits/objects creation and rendering on the map,

3. To do:
*interaction with enviroment, ie. fruits, other animals, obstacles which could stop the snake from moving,
*interaction with user, simple GUI to start/quit the game, adjust options etc,
*music (???),
*another snake, controlled by computer, which could increase difficulty, by invoking collision with player (???)

*??? mark means optional, "to be discussed" features.

4. Difficulties:
*Graphics






