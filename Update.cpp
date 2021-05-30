#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "route.h"
#include "Variables.h"

void generateModeTimes();
void initMap();
void initDirections();

void changeFruit() {
	extern int level;
	extern int indexFruit;
	switch (level) {
	case 0:
		indexFruit = 0; // cherry
		break;
	case 1:
		indexFruit = 1; // strawberry
		break;
	case 2:
	case 3:
		indexFruit = 2;// orange
		break;
	case 4:
	case 5:
		indexFruit = 3; // apple
		break;
	case 6:
	case 7:
		indexFruit = 4; // grape
		break;
	case 8:
	case 9:
		indexFruit = 5; // galaxian
		break;
	case 10:
	case 11:
		indexFruit = 6; // bell
		break;
	default:
		indexFruit = 7; // key
		break;
	}
}

void generateAlleyMovement(int i) {
	indexGhostRoute[i] = 0;
	routeInAlley(i, ghostRoute[i], indexGhostRoute[i]);
	updateGhostMovement(i);
}

void changePuntuation(std::string newText) {
	TTF_Font* font = TTF_OpenFont("Oswald-BoldItalic.ttf", SCREEN_SIZE / 9);
	SDL_Surface* textSurface;
	SDL_Color white = { 255,255,255 };
	const char* t = newText.c_str();
	textSurface = TTF_RenderText_Solid(font, t, white);
	puntuationTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FreeSurface(textSurface);
	TTF_CloseFont(font);
}

//generates a new random route, with no return movements
void generateGhostRandomMovement(int i) {
	indexGhostRoute[i] = 0;
	lockLastCoordinate(i);
	newRandomRoute(i);
	unlockLastCoordinate(i);
	updateToActualCoordinate(i);
}

//return true if a ghost i are in the alley
bool ghostInAlley(int i) {
	int xGhost = (ghostRect[i].x - boardBegin + ghostRect[i].w / 2) / sizeSquare;
	int yGhost = (ghostRect[i].y + ghostRect[i].h / 2) / sizeSquare;
	if (yGhost == 14 && (xGhost < 6 || xGhost > 21)) {
		return true;
	}
	return false;
}

void changeAlleyMovement(int i) {
	switch (ghostMovement[i]) {
	case 0:
		if ((ghostRect[i].x - boardBegin) % sizeSquare < ghostVelocity[i]) {
			generateAlleyMovement(i);
		}
		break;
	case 2:
		if ((ghostRect[i].x + ghostRect[i].w - boardBegin) % sizeSquare < ghostVelocity[i]) {
			generateAlleyMovement(i);
		}
		break;
	}
}

void changeGhostRandomMovement(int i) {
	if (ghostInAlley(i)) {
		changeAlleyMovement(i);
	}
	else {
		switch (ghostMovement[i]) {
		case 0:
			if ((ghostRect[i].x - boardBegin) % sizeSquare < ghostVelocity[i]) {
				generateGhostRandomMovement(i);
			}
			break;
		case 1:
			if (ghostRect[i].y % sizeSquare < ghostVelocity[i]) {
				generateGhostRandomMovement(i);
			}
			break;
		case 2:
			if ((ghostRect[i].x + ghostRect[i].w - boardBegin) % sizeSquare < ghostVelocity[i]) {
				generateGhostRandomMovement(i);
			}
			break;
		case 3:
			if ((ghostRect[i].y + ghostRect[i].h) % sizeSquare < ghostVelocity[i]) {
				generateGhostRandomMovement(i);
			}
			break;
		}
	}
}

void checkGhostInCenter(int i) {
	if (SDL_HasIntersection(&ghostRect[i], &centerRect)) {
		ghostAction[i] = 1;
		ghostState[i] = 0;
	}
}

void changeGhostCageMovement(int i) {
	switch (ghostMovement[i]) {
	case 0:
		if ((ghostRect[i].x - boardBegin) % sizeSquare < returningVelocity) {
			generateCageRoute(i);
		}
		break;
	case 1:
		if (ghostRect[i].y % sizeSquare < returningVelocity) {
			generateCageRoute(i);
		}
		break;
	case 2:
		if ((ghostRect[i].x + ghostRect[i].w - boardBegin) % sizeSquare < returningVelocity) {
			generateCageRoute(i);
		}
		break;
	case 3:
		if ((ghostRect[i].y + ghostRect[i].h) % sizeSquare < returningVelocity) {
			generateCageRoute(i);
		}
		break;
	}
}

//red ghost: the one who run or chase (actual box of pacman)
void changeMovementOikake(int i) {
	switch (ghostMovement[i]) {
	case 0:
		if ((ghostRect[i].x - boardBegin) % sizeSquare < ghostVelocity[i]) {
			generatePacmanRoute(i);
		}
		break;
	case 1:
		if (ghostRect[i].y % sizeSquare < ghostVelocity[i]) {
			generatePacmanRoute(i);
		}
		break;
	case 2:
		if ((ghostRect[i].x + ghostRect[i].w - boardBegin) % sizeSquare < ghostVelocity[i]) {
			generatePacmanRoute(i);
		}
		break;
	case 3:
		if ((ghostRect[i].y + ghostRect[i].h) % sizeSquare < ghostVelocity[i]) {
			generatePacmanRoute(i);
		}
		break;
	}
}

//orange ghost: the one who feigns ignorance (actual box of pacman)
void changeMovementOtoboke(int i) {
	switch (ghostMovement[i]) {
	case 0:
		if ((ghostRect[i].x - boardBegin) % sizeSquare < ghostVelocity[i]) {
			generateOtobokeRoute(i);
		}
		break;
	case 1:
		if (ghostRect[i].y % sizeSquare < ghostVelocity[i]) {
			generateOtobokeRoute(i);
		}
		break;
	case 2:
		if ((ghostRect[i].x + ghostRect[i].w - boardBegin) % sizeSquare < ghostVelocity[i]) {
			generateOtobokeRoute(i);
		}
		break;
	case 3:
		if ((ghostRect[i].y + ghostRect[i].h) % sizeSquare < ghostVelocity[i]) {
			generateOtobokeRoute(i);
		}
		break;
	}
}

//pink ghost: the one who ambush (4 boxes in front of pacman)
void changeMovementMachibuse(int i) {
	switch (ghostMovement[i]) {
	case 0:
		if ((ghostRect[i].x - boardBegin) % sizeSquare < ghostVelocity[i]) {
			generateFrontPacmanRoute(i);
		}
		break;
	case 1:
		if (ghostRect[i].y % sizeSquare < ghostVelocity[i]) {
			generateFrontPacmanRoute(i);
		}
		break;
	case 2:
		if ((ghostRect[i].x + ghostRect[i].w - boardBegin) % sizeSquare < ghostVelocity[i]) {
			generateFrontPacmanRoute(i);
		}
		break;
	case 3:
		if ((ghostRect[i].y + ghostRect[i].h) % sizeSquare < ghostVelocity[i]) {
			generateFrontPacmanRoute(i);
		}
		break;
	}
}

//blue ghost: the one of inconstant character(copy a behaviour of other ghost)
void changeMovementKimagure(int i) {
	switch (ghostMovement[i]) {
	case 0:
		if ((ghostRect[i].x - boardBegin) % sizeSquare < ghostVelocity[i]) {
			generateKimagureRoute(i);
		}
		break;
	case 1:
		if (ghostRect[i].y % sizeSquare < ghostVelocity[i]) {
			generateKimagureRoute(i);
		}
		break;
	case 2:
		if ((ghostRect[i].x + ghostRect[i].w - boardBegin) % sizeSquare < ghostVelocity[i]) {
			generateKimagureRoute(i);
		}
		break;
	case 3:
		if ((ghostRect[i].y + ghostRect[i].h) % sizeSquare < ghostVelocity[i]) {
			generateKimagureRoute(i);
		}
		break;
	}
}

void selectPersecutionMovement(int i) {
	if (ghostInAlley(i)) {
		changeAlleyMovement(i);
	}
	else {
		switch (i) {
		case 0:
			changeMovementKimagure(i);
			break;
		case 1:
			changeMovementMachibuse(i);
			break;
		case 2:
			changeMovementOtoboke(i);
			break;
		case 3:
			changeMovementOikake(i);
			break;
		}
	}
}

void moveGhost(int i) {
	int speed;
	if (ghostAction[i] == 5) {
		speed = returningVelocity;
	}
	else {
		speed = ghostVelocity[i];
	}
	switch (ghostMovement[i]) {
	case 0:
		if (ghostRect[i].x - ghostVelocity[i] - boardBegin <= sizeSquare / 5) {
			indexGhostRoute[i] = 0;
			ghostRect[i].x = boardBegin + totalX * sizeSquare - sizeSquare / 2;
			routeInAlley(i, ghostRoute[i], indexGhostRoute[i]);
		}
		else
			ghostRect[i].x -= speed;
		break;
	case 1:
		ghostRect[i].y -= speed;
		break;
	case 2:
		if (ghostRect[i].x + ghostRect[i].w >= SCREEN_SIZE - boardBegin - ghostVelocity[i]) {
			indexGhostRoute[i] = 0;
			ghostRect[i].x = boardBegin;
			routeInAlley(i, ghostRoute[i], indexGhostRoute[i]);
		}
		else
			ghostRect[i].x += speed;
		break;
	case 3:
		ghostRect[i].y += speed;
		break;
	}
}

void changeGhostDisbandedMovement(int i) {
	switch (ghostMovement[i]) {
	case 0:
		if ((ghostRect[i].x - boardBegin) % sizeSquare < ghostVelocity[i]) {
			generateDisbandedRoute(i);
		}
		break;
	case 1:
		if (ghostRect[i].y % sizeSquare < ghostVelocity[i]) {
			generateDisbandedRoute(i);
		}
		break;
	case 2:
		if ((ghostRect[i].x + ghostRect[i].w - boardBegin) % sizeSquare < ghostVelocity[i]) {
			generateDisbandedRoute(i);
		}
		break;
	case 3:
		if ((ghostRect[i].y + ghostRect[i].h) % sizeSquare < ghostVelocity[i]) {
			generateDisbandedRoute(i);
		}
		break;
	}
}

//moves the ghost i to up to get out of the cage
void leavingCage(int i) {
	if (ghostRect[i].y > lastRect.y) {
		ghostRect[i].y--;
	}
	else {
		indexGhostRoute[i] = 0;
		if (ghostState[i] == 1 || ghostState[i] == 2) {
			//run away
			ghostAction[i] = 6;
			newRandomRoute(i);
			updateToActualCoordinate(i);
		}
		else if (indexMode % 2 == 0) {
			//disbanded
			ghostAction[i] = 3;
			newDisbandedRoute(i);
			updateToActualCoordinate(i);
		}
		else {
			//persecution
			//this 3 instructions will realized in every change of action (with the correspondient action)
			ghostAction[i] = 4;
			newRouteToPacman(i);
			updateToActualCoordinate(i);
		}
	}
}

//this method takes the ghost i to te center of the cage
void centeringCage(int i) {
	if (ghostRect[i].y != centerRect.y) {
		if (ghostRect[i].y < centerRect.y) {
			ghostRect[i].y++;
		}
		else {
			ghostRect[i].y--;
		}
	}
	else if (ghostRect[i].x != centerRect.x) {
		if (ghostRect[i].x < centerRect.x) {
			ghostRect[i].x++;
		}
		else {
			ghostRect[i].x--;
		}
	}
	else {
		ghostAction[i] = 2;
	}
}

//moves up and down a ghost in the cage
void upDownGhost(int i) {
	if (insideGhostVelocity > 0) {
		//down, blocks in y of 13 to 15
		if (ghostRect[i].y < 15 * sizeSquare) {
			ghostRect[i].y++;
		}
		else {
			insideGhostVelocity = -insideGhostVelocity;
		}
	}
	else {
		//up
		if (ghostRect[i].y > 13 * sizeSquare) {
			ghostRect[i].y--;
		}
		else {
			insideGhostVelocity = -insideGhostVelocity;
		}
	}
}

//changes the action of a ghost of been in the cage to get out of the cage
void leaveGhost() {
	for (int i = 0; i < 4; i++) {
		if (ghostAction[i] == 0) {
			ghostAction[i] = 1;
			break;
		}
	}
}

void incrementPuntuation(int a) {
	int auxiliar = a + puntuation;
	int totalNumbers = 1;
	while (auxiliar > 10) {
		auxiliar /= 10;
		totalNumbers++;
	}
	puntuationRect.w = totalNumbers * pacmanLifeRect.w / 2;
	puntuation += a;
	if (puntuation / 10000 > newLife) {
		newLife++;
		lives++;
	}
	changePuntuation(std::to_string(puntuation));
}


void deleteSecondFruit() {
	matriz[23][13] = 0;
	timerSecondFruit = 0;
	secondFruit = false;
}

void deleteFirstFruit() {
	matriz[23][14] = 0;
	timerFirstFruit = 0;
	firstFruit = false;
}

void restart() {
	started = true;

	pacmanRect.x = (int)(boardBegin + 13.5 * sizeSquare) + 1;
	pacmanRect.y = 23 * sizeSquare + 1;

	ghostRect[0].x = (int)(boardBegin + 11.5 * sizeSquare) + 1;
	ghostRect[0].y = 14 * sizeSquare + 1;

	ghostRect[1].x = (int)(boardBegin + 13.5 * sizeSquare) + 1;
	ghostRect[1].y = 14 * sizeSquare + 1;

	ghostRect[2].x = (int)(boardBegin + 15.5 * sizeSquare) + 1;
	ghostRect[2].y = 14 * sizeSquare + 1;

	ghostRect[3].x = (int)(boardBegin + 13.5 * sizeSquare) + 1;
	ghostRect[3].y = 11 * sizeSquare + 1;

	initDirections();

	//ghost in cage
	for (int i = 0; i < 3; i++) {
		ghostAction[i] = 0;
	}
	ghostAction[3] = 3;

	for (int i = 0; i < 4; i++) {
		ghostState[i] = 0;
	}
	insideGhostVelocity = abs(insideGhostVelocity);
	timerLeaveGhost = 0;
	ghostInside = 3;

	if (lives <= 0) {
		puntuation = 0;
		lives = 3;
		totalBalls = 244;
		incrementPuntuation(0);
		blueMicroseconds = 5000;
		vulnerableMicroseconds = 6000;
		newLife = 0;//when pacman dies, the player will need others 10000 points to ein a life
		level = 0;
		generateModeTimes();
		changeFruit();
		initMap();
	}
	for (int i = 0; i < 4; i++) {
		indexGhostRoute[i] = 0;
	}
	indexMode = 0;
	timerMode = 0;

	for (int i = 0; i < 4; i++) {
		indexDisbanded[i] = 0;
	}

	for (int i = 0; i < 4; i++) {
		lastCoordinate[i] = { 0,0 };
		elementLastCoordinate[i] = 11;
	}
}

void nextLevel() {
	restart();
	totalBalls = 244;
	level++;
	changeFruit();
	generateModeTimes();
	if (blueMicroseconds > 1000) {
		blueMicroseconds -= 500;
		vulnerableMicroseconds -= 500;
	}
	initMap();
}

//put the first or second fruit on the board of the level
void putFruitOnBoard() {
	if (firstFruit) {
		matriz[23][14] = indexFruit + 3;
	}
	else
		matriz[23][13] = indexFruit + 3;
}

//Changes the sprites of pacman when he dies for 2.4 seconds and then restart the game
void detectPacmanDies() {
	timerDies += MS;
	if (timerDies >= 12 * MS * 20) {
		timerDies = 0;
		pacmanDies = false;
		//original sprite of pacman
		imagePacmanRect[1].x = sizeImageSquare;
		if (firstFruit) {
			deleteFirstFruit();
		}
		else if (secondFruit) {
			deleteSecondFruit();
		}
		restart();
	}
	else if (timerDies % (20 * MS) == 0) {
		//sprites of the death of pacman
		imagePacmanRect[1].x += sizeImageSquare;
	}
}

void moveGhostEyes() {
	for (int i = 0; i < 4; i++) {
		if (i != indexEatedGhost && ghostAction[i] == 5) {
			changeGhostCageMovement(i);
			moveGhost(i);
			checkGhostInCenter(i);
		}
	}
}

//changes the eated state of a ghost and updates the puntuation
void detectTImerEat() {
	timerEat += MS;
	if (timerEat >= 1000) {
		timerEat = 0;
		eatPuntuationRect.x += sizeImageSquare;
		eated = false;
	}
}

//if 2 seconds passed, the score fruit will disappear
void detectTimerEatFruit() {
	timerEatedFruit += MS;
	if (timerEatedFruit >= 2000) {
		eatedFruit = false;
		timerEatedFruit = 0;
	}
}

//moves the 4 ghosts to the indicated depending of the correspondient action and increments their timers to use their sprites
void setActionGhost() {
	for (int i = 0; i < 4; i++) {
		switch (ghostAction[i]) {
		case 0:
			upDownGhost(i);
			break;
		case 1:
			centeringCage(i);
			break;
		case 2:
			leavingCage(i);
			break;
		case 3:
			//disbanded
			changeGhostDisbandedMovement(i);
			moveGhost(i);
			break;
		case 4:
			//persecution
			selectPersecutionMovement(i);
			moveGhost(i);
			break;
		case 5:
			//returning to cage
			changeGhostCageMovement(i);
			moveGhost(i);
			checkGhostInCenter(i);
			break;
		case 6:
			//running away (blue)
			changeGhostRandomMovement(i);
			moveGhost(i);
			break;
		}
		timerGhost[i] += MS;
	}
}

//it works in blue ghosts, in blueSeconds changes to intermitent and then changes to invulnerable
void updateVulnerableGhostState() {
	for (int i = 0; i < 4; i++) {
		if (ghostState[i] == 1) {
			timerBlue[i] += MS;
			if (timerBlue[i] >= blueMicroseconds) {
				ghostState[i] = 2;
			}
		}
		else if (ghostState[i] == 2) {
			timerBlue[i] += MS;
			if (timerBlue[i] >= vulnerableMicroseconds) {
				timerBlue[i] = 0;
				ghostState[i] = 0;
				//ghost mode changes to the actual mode
				if (indexMode % 2 == 0) {
					//disbanded
					indexGhostRoute[i] = 0;
					ghostAction[i] = 3;
					newDisbandedRoute(i);
					updateToActualCoordinate(i);
				}
				else {
					//persecution
					ghostAction[i] = 4;
					updateToActualCoordinate(i);
				}
			}
		}
	}
}

//leave a ghost out of the cage every 3 seconds
void leavingAGhost() {
	if (ghostInside > 0) {
		timerLeaveGhost += MS;
		if (timerLeaveGhost >= 3000) {
			leaveGhost();
			timerLeaveGhost = 0;
			ghostInside--;
		}
	}
}

//changes the index mode when the timer reach the next time of nodeTimes
void detectTimerMode() {
	if (indexMode < 7) {
		timerMode += MS;
		if (timerMode >= modeTimes[indexMode]) {
			timerMode = 0;
			indexMode++;
			if (indexMode % 2 == 1) {
				copy = (rand() % 3) + 1;
			}
			//ghost mode changes to the actual mode
			for (int i = 0; i < 4; i++) {
				if (ghostAction[i] == 3 || ghostAction[i] == 4) {
					if (indexMode % 2 == 0) {
						//disbanded
						indexGhostRoute[i] = 0;
						ghostAction[i] = 3;
						newDisbandedRoute(i);
						updateToActualCoordinate(i);
					}
					else {
						//persecution
						indexGhostRoute[i] = 0;
						ghostAction[i] = 4;
						newRouteToPacman(i);
						updateToActualCoordinate(i);
					}
				}
			}
		}
	}
}

//if pacman eats a fruit, it will gives the points and will be deleted, also the animation of score wil be activated
void checkPacmanFruitColision() {
	if (firstFruit) {
		SDL_Rect firstFruitRect = { 14 * sizeSquare + boardBegin,23 * sizeSquare,sizeSquare, sizeSquare };
		if (SDL_HasIntersection(&pacmanRect, &firstFruitRect)) {
			deleteFirstFruit();
			eatedFruit = true;
			incrementPuntuation(scoreFruits[indexFruit]);
		}
	}
	else if (secondFruit) {
		SDL_Rect secondFruitRect = { 13 * sizeSquare + boardBegin,23 * sizeSquare,sizeSquare, sizeSquare };
		if (SDL_HasIntersection(&pacmanRect, &secondFruitRect)) {
			deleteSecondFruit();
			eatedFruit = true;
			incrementPuntuation(scoreFruits[indexFruit]);
		}
	}
}

//if 7 seconds passed and the fruit has no taked, the fruit will disappear
void detectTimerFruits() {
	if (firstFruit) {
		timerFirstFruit += MS;
		if (timerFirstFruit >= 7000) {
			deleteFirstFruit();
		}
	}
	else if (secondFruit) {
		timerSecondFruit += MS;
		if (timerSecondFruit >= 7000) {
			deleteSecondFruit();
		}
	}
}

void checkPacmanGhostColision() {
	int xPacman = (pacmanRect.x - boardBegin + pacmanRect.w / 2) / sizeSquare;;
	int yPacman = (pacmanRect.y + pacmanRect.h / 2) / sizeSquare;
	for (int i = 0; i < 4; i++) {
		int xGhost = (ghostRect[i].x - boardBegin + ghostRect[i].w / 2) / sizeSquare;
		int yGhost = (ghostRect[i].y + ghostRect[i].h / 2) / sizeSquare;
		if ((xPacman == xGhost && yPacman == yGhost) && !eated) {
			switch (ghostState[i]) {
			case 0:
				//ghost kill pacman
				lives--;
				pacmanDies = true;
				eatedFruit = false;
				timerEatedFruit = 0;
				imagePacmanRect[1].x = 2 * sizeImageSquare;
				break;
			case 1:
			case 2:
				//pacman eats ghost
				ghostState[i] = 3;
				ghostAction[i] = 5;
				indexEatedGhost = i;
				indexGhostRoute[i] = 0;
				newRouteToCage(i);
				updateToActualCoordinate(i);
				incrementPuntuation((int)(200 * pow(2, eatPuntuationRect.x / sizeImageSquare)));
				eated = true;
				break;
			}
		}
	}
}

//deletes a ball if pacman has a colision with it
void deleteBall() {
	int pacmanX = (pacmanRect.x - boardBegin + pacmanRect.w / 2) / sizeSquare;
	int pacmanY = (pacmanRect.y + pacmanRect.h / 2) / sizeSquare;
	//deleting a little ball
	if (matriz[pacmanY][pacmanX] == 1) {
		matriz[pacmanY][pacmanX] = 0;
		incrementPuntuation(10);
		totalBalls--;
		for (int i = 0; i < 4; i++) {
			elementLastCoordinate[i] = matriz[lastCoordinate[i].x][lastCoordinate[i].y];
		}
		//pacman eats 70 balls
		if (totalBalls == 174) {
			firstFruit = true;
			putFruitOnBoard();
		}
		//pacman eats 190 balls
		else if (totalBalls == 54) {
			secondFruit = true;
			putFruitOnBoard();
		}
		if (totalBalls <= 0) {
			nextLevel();
		}
	}
	//deleting a big ball
	else if (matriz[pacmanY][pacmanX] == 2) {
		matriz[pacmanY][pacmanX] = 0;
		eatPuntuationRect.x = 0;
		for (int i = 0; i < 4; i++) {
			if (ghostState[i] != 3) {
				//changing the color of ghosts
				ghostState[i] = 1;
				timerBlue[i] = 0;
			}
			if (ghostAction[i] == 3 || ghostAction[i] == 4) {
				//changing to runaway mode
				indexGhostRoute[i] = 0;
				ghostAction[i] = 6;
				newRandomRoute(i);
				updateToActualCoordinate(i);
			}
		}
		incrementPuntuation(50);
		totalBalls--;
		for (int i = 0; i < 4; i++) {
			elementLastCoordinate[i] = matriz[lastCoordinate[i].x][lastCoordinate[i].y];
		}
		if (totalBalls <= 0) {
			nextLevel();
		}
	}
}

void movePacman() {
	switch (movement) {
		int auxiliar;
	case 0:
		auxiliar = pacmanRect.x - pacmanSpeed - boardBegin; //pixels on the board of pacman - speed in x
		if (matriz[pacmanRect.y / sizeSquare][auxiliar / sizeSquare] < 11) {
			pacmanRect.x -= pacmanSpeed;
			timerPacman += MS;
			if (auxiliar <= sizeSquare / 5) {
				pacmanRect.x = boardBegin + totalX * sizeSquare - sizeSquare / 2;
			}
		}
		break;
	case 1:
		auxiliar = pacmanRect.y - pacmanSpeed;//pixels on the board of pacman - speed in y
		if (matriz[auxiliar / sizeSquare][(pacmanRect.x - boardBegin) / sizeSquare] < 11) {
			pacmanRect.y -= pacmanSpeed;
			timerPacman += MS;
		}
		break;
	case 2:
		auxiliar = pacmanRect.x + pacmanRect.w + pacmanSpeed - boardBegin;//pixels on the board of pacman + speed in x
		if (matriz[pacmanRect.y / sizeSquare][auxiliar / sizeSquare] < 11) {
			pacmanRect.x += pacmanSpeed;
			timerPacman += MS;
			if (pacmanRect.x + pacmanRect.w >= SCREEN_SIZE - boardBegin - pacmanSpeed) {
				pacmanRect.x = boardBegin;
			}
		}
		break;
	case 3:
		auxiliar = pacmanRect.y + pacmanRect.h + pacmanSpeed; //pixels on the board of pacman + speed in y
		if (matriz[auxiliar / sizeSquare][(pacmanRect.x - boardBegin) / sizeSquare] < 11) {
			pacmanRect.y += pacmanSpeed;
			timerPacman += MS;
		}
		break;
	}
	if (nextMovement != movement) {
		//it detects if the next movement can be executed
		int y = pacmanRect.y / sizeSquare;//position y of pacman on the board
		int x = (pacmanRect.x - boardBegin) / sizeSquare;//position x of pacman on the board
		switch (nextMovement) {
		case 0:
			if ((pacmanRect.y % sizeSquare) <= pacmanSpeed && matriz[y][x - 1] < 11) {
				movement = nextMovement;
			}
			break;
		case 1:
			if ((pacmanRect.x % sizeSquare) <= pacmanSpeed && matriz[y - 1][x] < 11) {
				movement = nextMovement;
			}
			break;
		case 2:
			if ((pacmanRect.y % sizeSquare) <= pacmanSpeed && matriz[y][x + 1] < 11) {
				movement = nextMovement;
			}
			break;
		case 3:
			if ((pacmanRect.x % sizeSquare) <= pacmanSpeed && matriz[y + 1][x] < 11) {
				movement = nextMovement;
			}
			break;
		}
	}
}

//changes the sprites of pacman and ghosts every 100 microseconds to its direction
void changeSprites() {
	extern int timerPacman;
	extern bool pacmanChange;
	if (timerPacman >= 100) {
		timerPacman = 0;
		pacmanChange = !pacmanChange;
	}
	for (int i = 0; i < 4; i++) {
		extern int timerGhost[];
		extern bool ghostChange[];
		extern int ghostMovement[];
		extern SDL_Rect imageGhostRect[];
		extern int sizeImageSquare;
		if (timerGhost[i] >= 300) {
			timerGhost[i] = 0;
			ghostChange[i] = !ghostChange[i];
		}
		//change the sprites of ghosts
		switch (ghostMovement[i]) {
		case 0:
			imageGhostRect[i].x = 2 * sizeImageSquare;
			imageGhostRect[i + 5].x = 3 * sizeImageSquare;
			imageGhostRect[11 + i].x = 9 * sizeImageSquare;
			break;
		case 1:
			imageGhostRect[i].x = 4 * sizeImageSquare;
			imageGhostRect[i + 5].x = 5 * sizeImageSquare;
			imageGhostRect[11 + i].x = 10 * sizeImageSquare;
			break;
		case 2:
			imageGhostRect[i].x = 0;
			imageGhostRect[i + 5].x = sizeImageSquare;
			imageGhostRect[11 + i].x = 8 * sizeImageSquare;
			break;
		case 3:
			imageGhostRect[i].x = 6 * sizeImageSquare;
			imageGhostRect[i + 5].x = 7 * sizeImageSquare;
			imageGhostRect[11 + i].x = 11 * sizeImageSquare;
			break;
		}
	}
}

void update() {
	extern bool pacmanDies;
	changeSprites();
	if (!pacmanDies) {
		extern bool eated;
		if (!eated) {
			movePacman();
			deleteBall();
			checkPacmanGhostColision();
			detectTimerFruits();
			checkPacmanFruitColision();
			detectTimerMode();
			leavingAGhost();
			updateVulnerableGhostState();
			setActionGhost();
			if (eatedFruit) {
				detectTimerEatFruit();
			}
		}
		else {
			detectTImerEat();
			moveGhostEyes();
		}
	}
	else
		detectPacmanDies();
}