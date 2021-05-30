#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "route.h"
void changeFruit();

bool init() {
	extern int SCREEN_SIZE;
	extern SDL_Window* window;
	extern SDL_Renderer* renderer;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), NULL);
		SDL_Quit();
		std::cout << "SDL did not initialize " << std::endl;
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Image",
			"The image PNG is not working", NULL);
		return false;
	}

	if (TTF_Init() < 0) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Ttf",
			"The ttf is not working", NULL);
		return false;
	}
	if (SDL_CreateWindowAndRenderer(SCREEN_SIZE, SCREEN_SIZE, SDL_WINDOW_RESIZABLE,
		&window, &renderer) < 0) {
		std::cout << "window or renderer are not initialized " << std::endl;
		return false;
	}
	SDL_SetWindowTitle(window, "Breakout");
	return true;
}

void generateModeTimes() {
	extern int level;
	extern int modeTimes[];
	switch (level) {
	case 0:
		modeTimes[0] = 7 * 1000; // 7 seconds
		modeTimes[2] = 7 * 1000;
		modeTimes[5] = 20 * 1000;
		modeTimes[6] = 5 * 1000;
		break;
	case 1:
	case 2:
	case 3:
		modeTimes[0] = 7 * 1000;
		modeTimes[2] = 7 * 1000;
		modeTimes[5] = 17 * 60 * 1000 + 13 * 1000; // 17 minutes, 13 seconds
		modeTimes[6] = (1 / 60) * 1000;
		break;
	default:
		modeTimes[0] = 5 * 1000;
		modeTimes[2] = 5 * 1000;
		modeTimes[5] = 17 * 60 * 1000 + 17 * 1000;
		modeTimes[6] = (1 / 60) * 1000;
		break;
	}
}



void loadImage() {
	extern SDL_Texture* totalTexture;
	extern SDL_Renderer* renderer;
	extern int sizeImageSquare;
	extern SDL_Texture* mapTexture;
	extern int widthMapSquare;
	extern int heightMapSquare;
	SDL_Surface* image;

	image = IMG_Load("images/pacmanSprites.png");
	if (!image) {
		std::cout << "failed in load pacmanSprites image" << std::endl;
	}
	SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 0, 0, 0));
	totalTexture = SDL_CreateTextureFromSurface(renderer, image);

	sizeImageSquare = image->w / 14;

	image = IMG_Load("images/map.png");
	if (!image) {
		std::cout << "failed in load map image" << std::endl;
	}
	mapTexture = SDL_CreateTextureFromSurface(renderer, image);
	widthMapSquare = image->w / 28;
	heightMapSquare = image->h / 31;

	SDL_FreeSurface(image);
}

void initDirections() {
	extern int ghostMovement[];
	extern int movement;
	extern int nextMovement;
	for (int i = 0; i < 4; i++) {
		ghostMovement[i] = 1;
	}
	movement = 0;
	nextMovement = 0;
}

void initMapColisions() {
	extern int matriz[31][28];
	extern int totalX;
	extern int totalY;

	//11
	matriz[0][0] = 11;
	matriz[19][0] = 11;

	//12
	for (int i = 1; i < 13; i++) {
		matriz[0][i] = 12;
		matriz[0][totalX - i - 1] = 12;
	}
	for (int i = 1; i < 5; i++) {
		matriz[19][i] = 12;
		matriz[19][totalX - i - 1] = 12;
	}
	for (int i = 0; i < 5; i++) {
		matriz[13][i] = 12;
		matriz[13][totalX - i - 1] = 12;
	}
	for (int i = 11; i < 17; i++) {
		matriz[16][i] = 12;
	}

	//13
	matriz[0][13] = 13;

	//14
	matriz[0][14] = 14;

	//15
	matriz[0][27] = 15;
	matriz[19][27] = 15;

	//16
	for (int i = 1; i < 9; i++) {
		matriz[i][0] = 16;
	}
	for (int i = 10; i < 13; i++) {
		matriz[i][5] = 16;
	}
	for (int i = 16; i < 19; i++) {
		matriz[i][5] = 16;
	}
	for (int i = 20; i < 24; i++) {
		matriz[i][0] = 16;
	}
	for (int i = 26; i < 30; i++) {
		matriz[i][0] = 16;
	}
	for (int i = 13; i < 16; i++) {
		matriz[i][17] = 16;
	}

	//17
	for (int i = 1; i < 4; i++) {
		matriz[i][13] = 17;
	}
	matriz[3][2] = 17;
	matriz[3][7] = 17;
	matriz[3][16] = 17;
	matriz[3][22] = 17;
	for (int i = 7; i < 13; i++) {
		matriz[i][7] = 17;
		matriz[i][20] = 17;
	}
	matriz[8][13] = 17;
	matriz[9][13] = 17;
	for (int i = 16; i < 19; i++) {
		matriz[i][7] = 17;
		matriz[i][19] = 17;
	}
	matriz[20][13] = 17;
	matriz[21][13] = 17;
	for (int i = 22; i < 25; i++) {
		matriz[i][22] = 17;
	}
	matriz[23][4] = 17;
	matriz[24][4] = 17;
	matriz[25][7] = 17;
	matriz[26][7] = 17;
	matriz[25][19] = 17;
	matriz[26][19] = 17;
	matriz[26][13] = 17;
	matriz[27][13] = 17;

	//18
	matriz[2][2] = 18;
	matriz[2][7] = 18;
	matriz[2][16] = 18;
	matriz[2][22] = 18;
	matriz[6][2] = 18;
	matriz[6][7] = 18;
	matriz[6][10] = 18;
	matriz[6][19] = 18;
	matriz[6][22] = 18;
	matriz[7][14] = 18;
	matriz[9][16] = 18;
	matriz[9][22] = 18;
	matriz[10][8] = 18;
	matriz[15][7] = 18;
	matriz[15][19] = 18;
	matriz[15][22] = 18;
	matriz[18][10] = 18;
	matriz[19][14] = 18;
	matriz[21][2] = 18;
	matriz[21][7] = 18;
	matriz[21][16] = 18;
	matriz[21][22] = 18;
	matriz[22][23] = 18;
	matriz[24][7] = 18;
	matriz[24][10] = 18;
	matriz[24][19] = 18;
	matriz[24][25] = 18;
	matriz[25][14] = 18;
	matriz[27][2] = 18;
	matriz[27][16] = 18;

	//19
	for (int i = 3; i < 5; i++) {
		matriz[2][i] = 19;
		matriz[2][totalX - i - 1] = 19;
		matriz[6][i] = 19;
		matriz[6][totalX - i - 1] = 19;
	}
	for (int i = 8; i < 11; i++) {
		matriz[2][i] = 19;
		matriz[2][totalX - i - 1] = 19;
	}
	for (int i = 11; i < 17; i++) {
		matriz[6][i] = 19;
		matriz[18][i] = 19;
		matriz[24][i] = 19;
	}
	for (int i = 9; i < 11; i++) {
		matriz[9][i] = 19;
		matriz[9][totalX - i - 1] = 19;
	}
	for (int i = 3; i < 5; i++) {
		matriz[21][i] = 19;
		matriz[21][totalX - i - 1] = 19;
	}
	for (int i = 8; i < 11; i++) {
		matriz[21][i] = 19;
		matriz[21][totalX - i - 1] = 19;
	}
	matriz[24][1] = 19;
	matriz[24][26] = 19;

	for (int i = 3; i < 7; i++) {
		matriz[27][i] = 19;
		matriz[27][totalX - i - 1] = 19;
	}
	for (int i = 9; i < 11; i++) {
		matriz[27][i] = 19;
		matriz[27][totalX - i - 1] = 19;
	}

	//20
	matriz[2][5] = 20;
	matriz[2][11] = 20;
	matriz[2][20] = 20;
	matriz[2][25] = 20;
	matriz[6][5] = 20;
	matriz[6][8] = 20;
	matriz[6][17] = 20;
	matriz[6][20] = 20;
	matriz[6][25] = 20;
	matriz[7][13] = 20;
	matriz[9][5] = 20;
	matriz[9][11] = 20;
	matriz[10][19] = 20;
	matriz[15][5] = 20;
	matriz[15][8] = 20;
	matriz[15][20] = 20;
	matriz[18][17] = 20;
	matriz[19][13] = 20;
	matriz[21][5] = 20;
	matriz[21][11] = 20;
	matriz[21][20] = 20;
	matriz[21][25] = 20;
	matriz[22][4] = 20;
	matriz[24][2] = 20;
	matriz[24][8] = 20;
	matriz[24][17] = 20;
	matriz[24][20] = 20;
	matriz[25][13] = 20;
	matriz[27][11] = 20;
	matriz[27][25] = 20;

	//21
	matriz[4][2] = 21;
	matriz[4][7] = 21;
	matriz[4][13] = 21;
	matriz[4][16] = 21;
	matriz[4][22] = 21;
	matriz[7][2] = 21;
	matriz[7][10] = 21;
	matriz[7][22] = 21;
	matriz[9][8] = 21;
	matriz[10][13] = 21;
	matriz[10][16] = 21;
	matriz[13][7] = 21;
	matriz[13][22] = 21;
	matriz[13][19] = 21;
	matriz[19][7] = 21;
	matriz[19][10] = 21;
	matriz[19][19] = 21;
	matriz[19][22] = 21;
	matriz[22][2] = 21;
	matriz[22][7] = 21;
	matriz[22][13] = 21;
	matriz[22][16] = 21;
	matriz[25][4] = 21;
	matriz[25][10] = 21;
	matriz[25][22] = 21;
	matriz[25][25] = 21;
	matriz[27][8] = 21;
	matriz[27][20] = 21;
	matriz[28][2] = 21;
	matriz[28][13] = 21;
	matriz[28][16] = 21;

	//22
	matriz[4][5] = 22;
	matriz[4][11] = 22;
	matriz[4][14] = 22;
	matriz[4][20] = 22;
	matriz[4][25] = 22;
	matriz[7][5] = 22;
	matriz[7][17] = 22;
	matriz[7][25] = 22;
	matriz[9][19] = 22;
	matriz[10][11] = 22;
	matriz[10][14] = 22;
	matriz[13][5] = 22;
	matriz[13][8] = 22;
	matriz[13][20] = 22;
	matriz[19][5] = 22;
	matriz[19][8] = 22;
	matriz[19][17] = 22;
	matriz[19][20] = 22;
	matriz[22][11] = 22;
	matriz[22][14] = 22;
	matriz[22][20] = 22;
	matriz[22][25] = 22;
	matriz[25][2] = 22;
	matriz[25][5] = 22;
	matriz[25][17] = 22;
	matriz[25][23] = 22;
	matriz[27][7] = 22;
	matriz[27][19] = 22;
	matriz[28][11] = 22;
	matriz[28][14] = 22;
	matriz[28][25] = 22;

	//23
	matriz[9][0] = 23;
	matriz[30][0] = 23;

	//24
	matriz[9][27] = 24;
	matriz[30][27] = 24;

	//25
	matriz[12][10] = 25;

	//26
	matriz[12][17] = 26;

	//27
	matriz[16][10] = 27;

	//28
	matriz[16][17] = 28;

	//29
	matriz[24][0] = 29;

	//30
	matriz[25][0] = 30;

	//31
	matriz[24][27] = 31;

	//32
	matriz[25][27] = 32;

	//33
	matriz[12][13] = 33;
	matriz[12][14] = 33;

	//34
	for (int i = 1; i < 5; i++) {
		matriz[9][i] = 34;
		matriz[9][totalX - i - 1] = 34;
	}
	for (int i = 0; i < 5; i++) {
		matriz[15][i] = 34;
		matriz[15][totalX - i - 1] = 34;
	}
	matriz[12][11] = 34;
	matriz[12][12] = 34;
	matriz[12][15] = 34;
	matriz[12][16] = 34;
	for (int i = 1; i < 27; i++) {
		matriz[30][i] = 34;
	}

	//35
	for (int i = 1; i < 9; i++) {
		matriz[i][27] = 35;
	}
	for (int i = 20; i < 24; i++) {
		matriz[i][27] = 35;
	}
	for (int i = 26; i < 30; i++) {
		matriz[i][27] = 35;
	}
	for (int i = 10; i < 13; i++) {
		matriz[i][22] = 35;
	}
	for (int i = 16; i < 19; i++) {
		matriz[i][22] = 35;
	}
	for (int i = 13; i < 16; i++) {
		matriz[i][10] = 35;
	}

	//36
	for (int i = 1; i < 4; i++) {
		matriz[i][14] = 36;
	}
	matriz[3][5] = 36;
	matriz[3][11] = 36;
	matriz[3][20] = 36;
	matriz[3][25] = 36;
	matriz[7][8] = 36;
	matriz[8][8] = 36;
	matriz[11][8] = 36;
	matriz[12][8] = 36;
	matriz[7][19] = 36;
	matriz[8][19] = 36;
	matriz[11][19] = 36;
	matriz[12][19] = 36;
	matriz[8][14] = 36;
	matriz[9][14] = 36;
	for (int i = 16; i < 19; i++) {
		matriz[i][8] = 36;
		matriz[i][20] = 36;
	}
	matriz[20][14] = 36;
	matriz[21][14] = 36;
	for (int i = 22; i < 25; i++) {
		matriz[i][5] = 36;
	}
	matriz[23][23] = 36;
	matriz[24][23] = 36;
	matriz[25][8] = 36;
	matriz[26][8] = 36;
	matriz[25][20] = 36;
	matriz[26][20] = 36;
	matriz[26][14] = 36;
	matriz[27][14] = 36;

	//37
	for (int i = 3; i < 5; i++) {
		matriz[4][i] = 37;
		matriz[4][totalX - i - 1] = 37;
		matriz[7][i] = 37;
		matriz[7][totalX - i - 1] = 37;
	}
	for (int i = 8; i < 11; i++) {
		matriz[4][i] = 37;
		matriz[4][totalX - i - 1] = 37;
	}
	for (int i = 11; i < 13; i++) {
		matriz[7][i] = 37;
		matriz[7][totalX - i - 1] = 37;
		matriz[19][i] = 37;
		matriz[19][totalX - i - 1] = 37;
		matriz[25][i] = 37;
		matriz[25][totalX - i - 1] = 37;
	}
	for (int i = 9; i < 11; i++) {
		matriz[10][i] = 37;
		matriz[10][totalX - i - 1] = 37;
	}
	for (int i = 8; i < 11; i++) {
		matriz[22][i] = 37;
		matriz[22][totalX - i - 1] = 37;
	}
	matriz[22][3] = 37;
	matriz[22][24] = 37;
	matriz[25][1] = 37;
	matriz[25][26] = 37;
	for (int i = 3; i < 11; i++) {
		matriz[28][i] = 37;
		matriz[28][totalX - i - 1] = 37;
	}

	//colision without image 38
	for (int i = 3; i < 5; i++) {
		matriz[3][i] = 38;
		matriz[3][totalX - i - 1] = 38;
	}
	for (int i = 8; i < 11; i++) {
		matriz[3][i] = 38;
		matriz[3][totalX - i - 1] = 38;
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 10; j < 13; j++) {
			matriz[j][i] = 38;
			matriz[j][totalX - i - 1] = 38;
		}
		for (int j = 16; j < 19; j++) {
			matriz[j][i] = 38;
			matriz[j][totalX - i - 1] = 38;
		}
	}
	for (int i = 13; i < 16; i++) {
		for (int j = 11; j < 17; j++) {
			matriz[i][j] = 38;
		}
	}
}

void initMap() {
	extern int totalX;
	extern int totalY;
	extern int matriz[31][28];
	//little balls
	for (int i = 0; i < totalX; i++) {
		for (int j = 0; j < totalY; j++) {
			matriz[j][i] = 1;
		}
	}
	//big balls
	matriz[3][1] = 2;
	matriz[3][26] = 2;
	matriz[23][1] = 2;
	matriz[23][26] = 2;

	//nothing
	matriz[23][13] = 0;
	matriz[23][14] = 0;
	matriz[9][12] = 0;
	matriz[10][12] = 0;
	matriz[9][15] = 0;
	matriz[10][15] = 0;
	matriz[14][7] = 0;
	matriz[14][8] = 0;
	matriz[14][19] = 0;
	matriz[14][20] = 0;
	matriz[18][9] = 0;
	matriz[19][9] = 0;
	matriz[18][18] = 0;
	matriz[19][18] = 0;
	for (int i = 9; i < 19; i++) {
		for (int j = 11; j < 18; j++) {
			matriz[j][i] = 0;

		}
	}
	//delete balls of the alley
	for (int i = 0; i < 6; i++) {
		matriz[14][i] = 0;
		matriz[14][totalX - 1 - i] = 0;
	}
	initMapColisions();
}

void initVariables() {
	extern SDL_Rect gameRect;
	extern SDL_Rect scoreRect;
	extern SDL_Rect livesRect;
	extern SDL_Rect pacmanLifeRect;
	extern SDL_Rect puntuationRect;
	extern SDL_Rect littleBallRect;
	extern SDL_Rect pacmanRect;
	extern SDL_Rect ghostRect[];
	extern int SCREEN_SIZE;
	extern int sizeSquare;
	extern int totalY;
	extern int boardBegin;
	extern int insideGhostVelocity;
	extern int ghostVelocity[];
	extern int returningVelocity;
	extern SDL_Rect centerRect;
	extern SDL_Rect lastRect;
	extern int ghostAction[]; 
	extern int ghostState[];
	extern SDL_Rect imageGhostRect[];
	extern SDL_Rect imagePacmanRect[];
	extern SDL_Rect imageBallRect;
	extern SDL_Rect fruitsImageRect[];
	extern SDL_Rect colisionsImageRect[];
	extern int sizeImageSquare;
	extern int widthMapSquare;
	extern int heightMapSquare;
	extern int scoreFruits[];
	extern SDL_Rect scoreFruitImageRect[];
	extern SDL_Rect eatPuntuationRect;
	extern int timerGhost[];
	extern bool ghostChange[];
	extern SDL_Rect startRect;
	extern int totalX;
	extern int indexGhostRoute[];
	extern int level;
	extern int modeTimes[];
	extern SDL_Point disbandedRoute[4][4];
	extern int indexDisbanded[];
	
	
	srand((unsigned int)time(NULL));

	gameRect.w = (int)(SCREEN_SIZE * 0.15 * 3 / 4);
	gameRect.h = sizeSquare;
	gameRect.x = 0;
	gameRect.y = totalY * sizeSquare;

	scoreRect.w = gameRect.w;
	scoreRect.h = gameRect.h;
	scoreRect.x = gameRect.x;
	scoreRect.y = gameRect.y + gameRect.h;


	livesRect.w = gameRect.w;
	livesRect.h = gameRect.h;
	livesRect.x = SCREEN_SIZE / 2;
	livesRect.y = gameRect.y;

	pacmanLifeRect.w = livesRect.w / 3;
	pacmanLifeRect.h = livesRect.h;
	pacmanLifeRect.y = livesRect.y;

	puntuationRect.w = pacmanLifeRect.w / 2;
	puntuationRect.h = pacmanLifeRect.h;
	puntuationRect.x = scoreRect.x + scoreRect.w;
	puntuationRect.y = gameRect.y + gameRect.h / 2;

	littleBallRect.w = sizeSquare / 2;
	littleBallRect.h = sizeSquare / 2;

	pacmanRect.x = (int)(boardBegin + 13.5 * sizeSquare) + 1;
	pacmanRect.y = 23 * sizeSquare + 1;
	pacmanRect.w = sizeSquare - 2;
	pacmanRect.h = sizeSquare - 2;

	ghostRect[0].x = (int)(boardBegin + 11.5 * sizeSquare) + 1; // blue
	ghostRect[0].y = 14 * sizeSquare + 1;

	ghostRect[1].x = (int)(boardBegin + 13.5 * sizeSquare) + 1; // pink
	ghostRect[1].y = 14 * sizeSquare + 1;

	ghostRect[2].x = (int)(boardBegin + 15.5 * sizeSquare) + 1; // orange
	ghostRect[2].y = 14 * sizeSquare + 1;

	ghostRect[3].x = (int)(boardBegin + 13.5 * sizeSquare) + 1; // red
	ghostRect[3].y = 11 * sizeSquare + 1;

	for (int i = 0; i < 4; i++) {
		ghostRect[i].w = pacmanRect.w;
		ghostRect[i].h = pacmanRect.h;
	}

	insideGhostVelocity = SCREEN_SIZE / 720;

	ghostVelocity[0] = insideGhostVelocity;
	ghostVelocity[1] = insideGhostVelocity;
	ghostVelocity[2] = insideGhostVelocity;
	ghostVelocity[3] = insideGhostVelocity;

	returningVelocity = 2 * insideGhostVelocity;

	centerRect = ghostRect[1];
	lastRect = ghostRect[3];

	for (int i = 0; i < 3; i++) {
		ghostAction[i] = 0;
	}
	ghostAction[3] = 3;

	initMap();

	initDirections();

	for (int i = 0; i < 4; i++)
		ghostState[i] = 0;

	imageGhostRect[0] = { 2 * sizeImageSquare, 6 * sizeImageSquare,sizeImageSquare ,sizeImageSquare }; // inky
	imageGhostRect[1] = { 2 * sizeImageSquare, 5 * sizeImageSquare,sizeImageSquare ,sizeImageSquare }; // pinky
	imageGhostRect[2] = { 2 * sizeImageSquare, 7 * sizeImageSquare,sizeImageSquare ,sizeImageSquare }; // clyde
	imageGhostRect[3] = { 2 * sizeImageSquare, 4 * sizeImageSquare,sizeImageSquare ,sizeImageSquare }; // blinky
	imageGhostRect[4] = { 8 * sizeImageSquare, 4 * sizeImageSquare,sizeImageSquare ,sizeImageSquare }; // blue
	imageGhostRect[5] = { 3 * sizeImageSquare, 6 * sizeImageSquare,sizeImageSquare ,sizeImageSquare }; // inky (0)
	imageGhostRect[6] = { 3 * sizeImageSquare, 5 * sizeImageSquare,sizeImageSquare ,sizeImageSquare }; // pinky (0)
	imageGhostRect[7] = { 3 * sizeImageSquare, 7 * sizeImageSquare,sizeImageSquare ,sizeImageSquare }; // clyde (0)
	imageGhostRect[8] = { 3 * sizeImageSquare, 4 * sizeImageSquare,sizeImageSquare ,sizeImageSquare }; // blinky (0)
	imageGhostRect[9] = { 9 * sizeImageSquare, 4 * sizeImageSquare,sizeImageSquare ,sizeImageSquare }; // blue
	imageGhostRect[10] = { 11 * sizeImageSquare, 4 * sizeImageSquare,sizeImageSquare ,sizeImageSquare }; // white(0)
	imageGhostRect[11] = { 9 * sizeImageSquare, 5 * sizeImageSquare,sizeImageSquare ,sizeImageSquare }; // eyes left
	imageGhostRect[12] = { 10 * sizeImageSquare, 5 * sizeImageSquare,sizeImageSquare ,sizeImageSquare }; // eyes up
	imageGhostRect[13] = { 8 * sizeImageSquare, 5 * sizeImageSquare,sizeImageSquare ,sizeImageSquare }; // eyes right
	imageGhostRect[14] = { 11 * sizeImageSquare, 5 * sizeImageSquare,sizeImageSquare ,sizeImageSquare }; // eyes down

	imagePacmanRect[0] = { 0, 0,sizeImageSquare ,sizeImageSquare };
	imagePacmanRect[1] = { sizeImageSquare, 0,sizeImageSquare ,sizeImageSquare };

	imageBallRect = { 2 * sizeImageSquare, 0,sizeImageSquare ,sizeImageSquare };

	fruitsImageRect[0] = { 2 * sizeImageSquare, 3 * sizeImageSquare, sizeImageSquare ,sizeImageSquare }; // cherry
	fruitsImageRect[1] = { 3 * sizeImageSquare, 3 * sizeImageSquare, sizeImageSquare ,sizeImageSquare }; // strawberry
	fruitsImageRect[2] = { 4 * sizeImageSquare, 3 * sizeImageSquare, sizeImageSquare ,sizeImageSquare }; // orange
	fruitsImageRect[3] = { 5 * sizeImageSquare, 3 * sizeImageSquare, sizeImageSquare ,sizeImageSquare }; // apple
	fruitsImageRect[4] = { 6 * sizeImageSquare, 3 * sizeImageSquare, sizeImageSquare ,sizeImageSquare }; // grape
	fruitsImageRect[5] = { 7 * sizeImageSquare, 3 * sizeImageSquare, sizeImageSquare ,sizeImageSquare }; // galaxian
	fruitsImageRect[6] = { 8 * sizeImageSquare, 3 * sizeImageSquare, sizeImageSquare ,sizeImageSquare }; // bell
	fruitsImageRect[7] = { 9 * sizeImageSquare, 3 * sizeImageSquare, sizeImageSquare ,sizeImageSquare }; // key

	colisionsImageRect[0] = { 0,0,widthMapSquare,heightMapSquare }; // rounded double left superior corner
	colisionsImageRect[1] = { widthMapSquare ,0,widthMapSquare,heightMapSquare }; // horizontal superior double lines (up side)
	colisionsImageRect[2] = { 13 * widthMapSquare,0,widthMapSquare,heightMapSquare };
	colisionsImageRect[3] = { 14 * widthMapSquare,0,widthMapSquare,heightMapSquare };
	colisionsImageRect[4] = { 27 * widthMapSquare,0,widthMapSquare,heightMapSquare }; // rounded double right superior corner
	colisionsImageRect[5] = { 0,heightMapSquare,widthMapSquare,heightMapSquare }; // vertical double lines (left side)
	colisionsImageRect[6] = { 13 * widthMapSquare,heightMapSquare,widthMapSquare,heightMapSquare }; // vertical line (closer to right)
	colisionsImageRect[7] = { 2 * widthMapSquare,2 * heightMapSquare,widthMapSquare,heightMapSquare }; // rounded left superior corner
	colisionsImageRect[8] = { 3 * widthMapSquare,2 * heightMapSquare,widthMapSquare,heightMapSquare }; // horizontal line (closer to down)
	colisionsImageRect[9] = { 5 * widthMapSquare,2 * heightMapSquare,widthMapSquare,heightMapSquare }; // rounded right superior corner
	colisionsImageRect[10] = { 2 * widthMapSquare,4 * heightMapSquare,widthMapSquare,heightMapSquare }; // rounded left inferior corner
	colisionsImageRect[11] = { 5 * widthMapSquare,4 * heightMapSquare,widthMapSquare,heightMapSquare }; // rounded right inferior corner
	colisionsImageRect[12] = { 0, 9 * heightMapSquare,widthMapSquare,heightMapSquare }; // rounded double left inferior corner 
	colisionsImageRect[13] = { 27 * widthMapSquare,9 * heightMapSquare,widthMapSquare,heightMapSquare }; // rounded double right inferior corner
	colisionsImageRect[14] = { 10 * widthMapSquare,12 * heightMapSquare,widthMapSquare,heightMapSquare }; // double left superior corner
	colisionsImageRect[15] = { 17 * widthMapSquare,12 * heightMapSquare,widthMapSquare,heightMapSquare }; // double right superior corner
	colisionsImageRect[16] = { 10 * widthMapSquare,16 * heightMapSquare,widthMapSquare,heightMapSquare }; // double left inferior corner
	colisionsImageRect[17] = { 17 * widthMapSquare,16 * heightMapSquare,widthMapSquare,heightMapSquare }; // double right inferior corner
	colisionsImageRect[18] = { 0,24 * heightMapSquare,widthMapSquare,heightMapSquare };
	colisionsImageRect[19] = { 0,25 * heightMapSquare,widthMapSquare,heightMapSquare };
	colisionsImageRect[20] = { 27 * widthMapSquare,24 * heightMapSquare,widthMapSquare,heightMapSquare };
	colisionsImageRect[21] = { 27 * widthMapSquare,25 * heightMapSquare,widthMapSquare,heightMapSquare };
	colisionsImageRect[22] = { 13 * widthMapSquare,12 * heightMapSquare,widthMapSquare,heightMapSquare }; // pink rectangle
	colisionsImageRect[23] = { widthMapSquare,9 * heightMapSquare,widthMapSquare,heightMapSquare }; // horizontal superior double lines (down side)
	colisionsImageRect[24] = { 27 * widthMapSquare, heightMapSquare,widthMapSquare,heightMapSquare }; // vertical double lines (right side)
	colisionsImageRect[25] = { 14 * widthMapSquare,heightMapSquare,widthMapSquare,heightMapSquare }; // vertical line (closer to left)
	colisionsImageRect[26] = { 3 * widthMapSquare,4 * heightMapSquare,widthMapSquare,heightMapSquare }; // horizontal line (closer to up)

	scoreFruits[0] = 100;
	scoreFruits[1] = 300;
	scoreFruits[2] = 500;
	scoreFruits[3] = 700;
	scoreFruits[4] = 1000;
	scoreFruits[5] = 2000;
	scoreFruits[6] = 3000;
	scoreFruits[7] = 5000;

	scoreFruitImageRect[0] = { 0, 9 * sizeImageSquare, sizeImageSquare ,sizeImageSquare };//100
	scoreFruitImageRect[1] = { sizeImageSquare, 9 * sizeImageSquare, sizeImageSquare ,sizeImageSquare };//300
	scoreFruitImageRect[2] = { 2 * sizeImageSquare, 9 * sizeImageSquare, sizeImageSquare ,sizeImageSquare };//500
	scoreFruitImageRect[3] = { 3 * sizeImageSquare, 9 * sizeImageSquare, sizeImageSquare ,sizeImageSquare };//700
	scoreFruitImageRect[4] = { 4 * sizeImageSquare, 9 * sizeImageSquare, sizeImageSquare ,sizeImageSquare };//1000
	scoreFruitImageRect[5] = { 4 * sizeImageSquare, 10 * sizeImageSquare, sizeImageSquare ,sizeImageSquare };//2000
	scoreFruitImageRect[6] = { 4 * sizeImageSquare, 11 * sizeImageSquare, sizeImageSquare ,sizeImageSquare };//3000
	scoreFruitImageRect[7] = { 4 * sizeImageSquare, 12 * sizeImageSquare, sizeImageSquare ,sizeImageSquare };//5000

	eatPuntuationRect = { 0 , 8 * sizeImageSquare,sizeImageSquare ,sizeImageSquare };
	for (int i = 0; i < 4; i++) {
		timerGhost[i] = 0;
		ghostChange[i] = true;
	}

	startRect.w = SCREEN_SIZE / 5;
	startRect.h = (totalY * sizeSquare) / 2;
	startRect.x = SCREEN_SIZE / 2 - startRect.w / 2;
	startRect.y = startRect.h / 2;

	extern Box boardBox[31][28];

	for (int i = 0; i < totalY; i++) {
		for (int j = 0; j < totalX; j++) {
			boardBox[i][j].x = i;
			boardBox[i][j].y = j;
		}
	}

	//init a* elements
	for (int i = 0; i < 4; i++) {
		indexGhostRoute[i] = 0;
	}
	level = 0;
	//chooses the fruit of the level
	changeFruit();

	modeTimes[1] = 20000;
	modeTimes[3] = 20000;
	modeTimes[4] = 5000;
	generateModeTimes();

	disbandedRoute[0][0] = { 23,20 };
	disbandedRoute[0][1] = { 27,15 };
	disbandedRoute[0][2] = { 29,21 };
	disbandedRoute[0][3] = { 27,26 };

	disbandedRoute[1][0] = { 3,6 };
	disbandedRoute[1][1] = { 1,3 };
	disbandedRoute[1][2] = { 3,1 };
	disbandedRoute[1][3] = { 5,3 };

	disbandedRoute[2][0] = { 23,7 };
	disbandedRoute[2][1] = { 27,12 };
	disbandedRoute[2][2] = { 29,6 };
	disbandedRoute[2][3] = { 27,1 };

	disbandedRoute[3][0] = { 3,21 };
	disbandedRoute[3][1] = { 1,24 };
	disbandedRoute[3][2] = { 3,26 };
	disbandedRoute[3][3] = { 5,23 };

	for (int i = 0; i < 4; i++) {
		indexDisbanded[i] = 0;
	}

	newDisbandedRoute(3);
	updateToActualCoordinate(3);
}

void generateMenu() {
	extern int SCREEN_SIZE;
	extern SDL_Renderer* renderer;
	SDL_Texture* gameTexture;
	SDL_Texture* scoreTexture;
	SDL_Texture* livesTexture;
	SDL_Texture* puntuationTexture;

	SDL_Color white = { 255,255,255 };
	SDL_Surface* textSurface;
	TTF_Font* font = TTF_OpenFont("Oswald-BoldItalic.ttf", SCREEN_SIZE / 36);
	textSurface = TTF_RenderText_Solid(font, "GAME", white);
	gameTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	textSurface = TTF_RenderText_Solid(font, "SCORE", white);
	scoreTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	textSurface = TTF_RenderText_Solid(font, "LIVES", white);
	livesTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	font = TTF_OpenFont("Oswald-BoldItalic.ttf", SCREEN_SIZE / 9);
	textSurface = TTF_RenderText_Solid(font, "0", white);
	puntuationTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	TTF_CloseFont(font);
}