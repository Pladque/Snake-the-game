# Snake-the-game

#TODO

- [ ] Background sprite for game

- [ ] Walls

- [ ] Settings in menu

- [ ] pause menu

- [ ] high score (aka save system) 

- [ ] mode with opponent (mby like snake.io gameplay??)

- [ ] 3 types of apples (poisoned, golden, normal)

- [ ] menu with mouse input  

- [ ] Game window size depends on screen resolution

- [ ] Fix particle system 

## Update v1.1 ##

//write update here
Changes since 1.0:

*snake.c/hpp - main snake class. Snake is represented as the one-sided list, with one distinguished element, called the snake head. Other segments of program could interfere with a head, using methods of this class. Whole body contain set of single elements, belonging to the snake list, which are made to "follow" the head whenever it moves. Methods in this class allow the snake to grow, change directions and fade.

*settings.cpp - this file contains basic game configuration, such as snake speed, grid size, score bonuses, etc. Settings should be able to be altered from options in game main menu.

*collectableObj - fruit class, reserving space for fruit on the map, ensures to avoid generating fruit on the snake body/head. Methods of this class are mainly getters to class fields, needed by other segments of the code.

*consts.cpp - file with constants inlcuded in game. Contains paths to textures/sounds, snake directions mapping, etc.

*particle.cpp - experimental file, containing some "extra" effects in the game, while eating fruits etc. 

*game.cpp - game main body. Game window is generated here, with snake and fruits/other objects being rendered on the grid, player score is also displayed in the top of the window. It includes other configuration files, and libraries required for graphics/sound effects. Whole interaction with user, ie. moving snake with keystrokes etc, takes place here. Methods of previously included configuration classes are invoked here, to interact with adequates objects.

*main.cpp - main file, providing main menu and basic interaction with user, game.cpp is included here. Options/settings will be accesible from this part of the code. If user decides to start the game, game.cpp will be started.



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






