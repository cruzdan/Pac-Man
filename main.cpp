#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <math.h>
#include <stdlib.h>
#include "route.h"

bool init();
void loadImage();
void initVariables();
void close();
void detectKey();
void generateMenu();
void update();
void show();

int SCREEN_SIZE = 720;
int MS = 10;

SDL_Window* window;
SDL_Renderer* renderer;
bool gameOver = false;
int sizeSquare = SCREEN_SIZE / 36;
int totalX = 28;//total of squares on the board in x
int totalY = 31;//total of squares on the board in y
int matriz[31][28]; //board of the game
int movement; //pacman movement 0: left, 1: up, 2: right, 3: down
int nextMovement; //pacman next movement 0: left, 1: up, 2: right, 3: down
int pacmanSpeed = 1;
int boardBegin = SCREEN_SIZE / 9;

//image
SDL_Rect pacmanLifeRect;
SDL_Rect littleBallRect;
SDL_Rect pacmanRect;
SDL_Rect ghostRect[4];
SDL_Texture* totalTexture;
SDL_Texture* mapTexture;
SDL_Rect imageGhostRect[15];
SDL_Rect imagePacmanRect[2];
SDL_Rect imageBallRect;
int sizeImageSquare;
int widthMapSquare;
int heightMapSquare;

//menu 0
SDL_Texture* gameTexture;
SDL_Rect gameRect;
SDL_Texture* scoreTexture;
SDL_Rect scoreRect;
SDL_Texture* livesTexture;
SDL_Rect livesRect;
SDL_Texture* puntuationTexture;
SDL_Rect puntuationRect;
int lives = 3;

//movement of ghosts
int ghostAction[7]; // it indicates the action of the ghost i, 0: inside, 1: centering to leave, 2: leaving, 3:outside disbanded, 4: outside persecution, 5: returning to the cage, 6: running away
/*0: Inky, 1: Pinky, 2: Clyde, 3: Blinky*/
int ghostInside = 3;
int timerLeaveGhost = 0; // timer to get out a ghost of the cage
SDL_Rect centerRect;//rect of the center of the cage of ghost
SDL_Rect lastRect;//last rect that the ghost will need to stay to get free of the cage
int insideGhostVelocity;//velocity of the ghost in the cage
int ghostVelocity[4];//velocity of the ghost out of the cage
int returningVelocity;// velocity of the ghosts to return to the cage
int ghostMovement[4];//actual movement of the ghost i. it could have this values: 0-left, 1-up, 2-right, 3-down

int ghostState[4];//it indicates the state of the ghosts: 0: normal, 1:vulnerable, 2:intermittent, 3:dead
int timerBlue[4]; // it indicates the time of a ghost to be vulnerable or intermittent
int blueMicroseconds = 5000; // 5 seconds for a ghost to be blue
int vulnerableMicroseconds = 6000; // 6 seconds for a ghost to be vulnerable

//menu
int puntuation = 0;

//fruits
SDL_Rect fruitsImageRect[8];
bool firstFruit = false; //indicates if the first fruit is appear on the board
bool secondFruit = false; //indicates if the second fruit is appear on the board
bool eatedFruit = false; //indicates if pacman eats a fruit and the fruit score is on the board
int indexFruit; // fruit of the actual level
int timerFirstFruit = 0; //timer to count 7 seconds to the first fruit stay on the board
int timerSecondFruit = 0;//timer to count 7 seconds to the second fruit stay on the board
int scoreFruits[8];
SDL_Rect scoreFruitImageRect[8];
int timerEatedFruit = 0; // timer to count 2 seconds to the score fruit text stay on the board

//colisions rect
SDL_Rect colisionsImageRect[27];

//sprite pacman
int timerPacman = 0;
bool pacmanChange = true;

//sprite Ghosts
int timerGhost[4];
bool ghostChange[4];

//end level
int totalBalls = 244;

//text Start
SDL_Texture* textStart;
SDL_Rect startRect;
bool started = true;
int timerStart = 0;

//pacman dies
bool pacmanDies = false;//indicates if pacman is doing the animation of die
int timerDies = 0;

//eat ghost
SDL_Rect eatPuntuationRect; // square of puntuation that appears when pacman eats a ghost
int timerEat = 0; // timer to show the text of puntuation of eating a ghost
bool eated = false; // indicates when a ghost is eated
int indexEatedGhost;

//route
SDL_Point ghostRoute[4][80];//routes of the 4 ghosts, the last element is the actual coordinate of the ghosts
int indexGhostRoute[4];//it is the index of the next element in the route, as well as the total of elements in the route

bool pause = false;
int newLife = 0;//indicates the lifes winned
int level;

//modes
int modeTimes[7]; // times in microseconds of disbanded and persecution, when one ends, the other one begins
int indexMode = 0;// indicates the mode of modeTimes and the mode of ghost: indexMode % 2 -> 0: disbanded, 1: persecution
int timerMode = 0;// count the time of the actual mode and when reach the time of modeTimes[indexMode], the index increments
SDL_Point disbandedRoute[4][4]; //route of the ghost in disbanded mode
int indexDisbanded[4]; // actual index of disbanded route of ghost i
SDL_Point lastCoordinate[4]; // last traveled coordinate of ghost i that can not be repited
int elementLastCoordinate[4]; // element of the last coordinate of the ghost i

int copy; //indicates the persecution mode of the blue ghost (Inky)

int main(int argc, char* args[]) {
	if (!init())
		return -1;
	loadImage();
	initVariables();
	generateMenu();
	while (!gameOver) {
		detectKey();
		if(!started && !pause)
			update();
		show();
		SDL_Delay(MS);
	}
	close();
	return 0;
}