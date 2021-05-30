#pragma once
#include <SDL.h>
extern int SCREEN_SIZE;
extern int MS;

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern bool gameOver;
extern int sizeSquare;
extern int totalX;
extern int totalY;
extern int matriz[31][28];
extern int movement;
extern int nextMovement;
extern int pacmanSpeed;
extern int boardBegin;

//image
extern SDL_Rect pacmanLifeRect;
extern SDL_Rect littleBallRect;
extern SDL_Rect pacmanRect;
extern SDL_Rect ghostRect[];
extern SDL_Texture* totalTexture;
extern SDL_Texture* mapTexture;
extern SDL_Rect imageGhostRect[];
extern SDL_Rect imagePacmanRect[];
extern SDL_Rect imageBallRect;
extern int sizeImageSquare;
extern int widthMapSquare;
extern int heightMapSquare;

//menu 0
extern SDL_Texture* gameTexture;
extern SDL_Rect gameRect;
extern SDL_Texture* scoreTexture;
extern SDL_Rect scoreRect;
extern SDL_Texture* livesTexture;
extern SDL_Rect livesRect;
extern SDL_Texture* puntuationTexture;
extern SDL_Rect puntuationRect;
extern int lives;

//movement of ghosts
extern int ghostAction[]; 
extern int ghostInside;
extern int timerLeaveGhost;
extern SDL_Rect centerRect;
extern SDL_Rect lastRect;
extern int insideGhostVelocity;
extern int ghostVelocity[];
extern int returningVelocity;
extern int ghostMovement[];

extern int ghostState[];
extern int timerBlue[];
extern int blueMicroseconds;
extern int vulnerableMicroseconds;

//menu
extern int puntuation;

//fruits
extern SDL_Rect fruitsImageRect[];
extern bool firstFruit;
extern bool secondFruit;
extern bool eatedFruit;
extern int indexFruit;
extern int timerFirstFruit;
extern int timerSecondFruit;
extern int scoreFruits[];
extern SDL_Rect scoreFruitImageRect[];
extern int timerEatedFruit;

//colisions rect
extern SDL_Rect colisionsImageRect[];

//sprite pacman
extern int timerPacman;
extern bool pacmanChange ;

//sprite Ghosts
extern int timerGhost[];
extern bool ghostChange[];

//end level
extern int totalBalls;

//text Start
extern SDL_Texture* textStart;
extern SDL_Rect startRect;
extern bool started;
extern int timerStart;

//pacman dies
extern bool pacmanDies;
extern int timerDies;

//eat ghost
extern SDL_Rect eatPuntuationRect;
extern int timerEat;
extern bool eated;
extern int indexEatedGhost;

//route
extern SDL_Point ghostRoute[4][80];
extern int indexGhostRoute[];

extern bool pause;
extern int newLife;
extern int level;

//modes
extern int modeTimes[];
extern int indexMode;
extern int timerMode;
extern SDL_Point disbandedRoute[4][4];
extern int indexDisbanded[];
extern SDL_Point lastCoordinate[];
extern int elementLastCoordinate[];

extern int copy;