#include <SDL.h>
#include "route.h"
#include <iostream>
#include "Variables.h"

//algorithm A*

Box boardBox[31][28];
//updates the last coordinate of the ghost i to the actual coordinate and its element
void updateToActualCoordinate(int i) {
	lastCoordinate[i] = { (ghostRect[i].y + ghostRect[i].h / 2) / sizeSquare , (ghostRect[i].x + ghostRect[i].w / 2 - boardBegin) / sizeSquare };
	elementLastCoordinate[i] = matriz[lastCoordinate[i].x][lastCoordinate[i].y];
}

//updates the movement of a ghost i using the last elements of the route
void updateGhostMovement(int i) {
	SDL_Point p1 = ghostRoute[i][indexGhostRoute[i] - 1];
	SDL_Point p2 = ghostRoute[i][indexGhostRoute[i] - 2];
	ghostMovement[i] = getDirection(p1, p2);
}

//generates a route of the ghost i to pacman
void newRouteToPacman(int i) {
	int xGhost = (ghostRect[i].x - boardBegin + ghostRect[i].w / 2) / sizeSquare;
	int yGhost = (ghostRect[i].y + ghostRect[i].h / 2) / sizeSquare;
	int xPacman = (pacmanRect.x - boardBegin + pacmanRect.w / 2) / sizeSquare;;
	int yPacman = (pacmanRect.y + pacmanRect.h / 2) / sizeSquare;
	AStar(yGhost, xGhost, yPacman, xPacman, ghostRoute[i], indexGhostRoute[i]);
	updateGhostMovement(i);
}

//generates a route of the ghost i a free random coordinate
void newRandomRoute(int i) {
	int xGhost = (ghostRect[i].x - boardBegin + ghostRect[i].w / 2) / sizeSquare;
	int yGhost = (ghostRect[i].y + ghostRect[i].h / 2) / sizeSquare;
	int x;
	int y;
	do {
		x = rand() % (totalX - 2);
		y = rand() % (totalY - 2);
	} while (matriz[y][x] > 10 || (x == xGhost && y == yGhost));
	AStar(yGhost, xGhost, y, x, ghostRoute[i], indexGhostRoute[i]);
	updateGhostMovement(i);
}

//get the direction between 2 consecutive points on the board, p1 is the actual point and p2 is the final point
int getDirection(SDL_Point p1, SDL_Point p2) {
	int xf = p1.x - p2.x;
	int yf = p1.y - p2.y;
	if (xf != 0) {
		if (xf > 0)
			return 1;
		else
			return 3;
	}
	else {
		if (yf > 0)
			return 0;
		else
			return 2;
	}
}

void addToEndBox(Box array[], int& index, Box element) {
	array[index] = element;
	index++;
}

void addToEndRoute(SDL_Point array[], int& index, SDL_Point element) {
	array[index] = element;
	index++;
}

// deletes a Box element of a Box array, decrement the index and reorganize the elements
void deleteOfArray(Box array[], int& index, Box element) {
	for (int i = 0; i < index; i++) {
		if (array[i].x == element.x && array[i].y == element.y) {
			for (int j = i + 1; j < index; j++) {
				array[j - 1] = array[j];
			}
			array[index - 1].x = -1;
			index--;
			break;
		}
	}
}

bool arrayContains(Box array[], int index, Box element) {
	for (int i = 0; i < index; i++) {
		if (array[i].x == element.x && array[i].y == element.y) {
			return true;
		}
	}
	return false;
}

int heuristic(Box a, Box b) {
	int x = abs(a.x - b.x);
	int y = abs(a.y - b.y);
	int z = x + y;
	return z;
}

void updateDataArray(Box array[], int index, Box element) {
	for (int i = 0; i < index; i++) {
		if (array[i].x == element.x && array[i].y == element.y) {
			array[i] = element;
		}
	}
}

void updateNeighbor(int xI, int yI, Box finalBox, Box actual, Box openSet[], int& indexOpenSet, Box closedSet[], int& indexClosedSet) {
	Box neighbor = boardBox[xI][yI];//aqui hay un error
	if (matriz[xI][yI] < 11) {
		bool first = arrayContains(openSet, indexOpenSet, neighbor);
		bool second = arrayContains(closedSet, indexClosedSet, neighbor);
		if (!first && !second) {
			boardBox[xI][yI].g = actual.g + 1;
			boardBox[xI][yI].h = heuristic(boardBox[xI][yI], finalBox);
			boardBox[xI][yI].f = boardBox[xI][yI].g + boardBox[xI][yI].h;
			boardBox[xI][yI].father = { actual.x, actual.y };
			addToEndBox(openSet, indexOpenSet, boardBox[xI][yI]);
		}
		else if (first) {
			if (actual.g + 1 < neighbor.g) {
				boardBox[xI][yI].g = actual.g + 1;
				boardBox[xI][yI].h = heuristic(boardBox[xI][yI], finalBox);
				boardBox[xI][yI].f = boardBox[xI][yI].g + boardBox[xI][yI].h;
				boardBox[xI][yI].father = { actual.x, actual.y };
				updateDataArray(openSet, indexOpenSet, boardBox[xI][yI]);
			}
		}
	}
}

void generateAndUpdateNeighbors(Box actual, Box finalBox, Box openSet[], int& indexOpenSet, Box closedSet[], int& indexClosedSet) {
	if (actual.x > 0) {
		updateNeighbor(actual.x - 1, actual.y, finalBox, actual, openSet, indexOpenSet, closedSet, indexClosedSet);
	}
	if (actual.x < totalY - 1) {
		updateNeighbor(actual.x + 1, actual.y, finalBox, actual, openSet, indexOpenSet, closedSet, indexClosedSet);
	}
	if (actual.y > 0) {
		updateNeighbor(actual.x, actual.y - 1, finalBox, actual, openSet, indexOpenSet, closedSet, indexClosedSet);
	}
	if (actual.y < totalX - 1) {
		updateNeighbor(actual.x, actual.y + 1, finalBox, actual, openSet, indexOpenSet, closedSet, indexClosedSet);
	}
}

//on this game it is taken as matriz[initialX][initialY] to matriz[finalX][finalY]
void AStar(int initialX, int initialY,int finalX, int finalY, SDL_Point route[], int& indexRoute) {
	for (int i = 0; i < totalY; i++) {
		for (int j = 0; j < totalX; j++) {
			boardBox[i][j].f = 0;
			boardBox[i][j].g = 0;
			boardBox[i][j].h = 0;
			boardBox[i][j].father = { -1,-1 };
		}
	}
	Box openSet[200];
	Box closedSet[200];
	int indexOpenSet = 0;
	int indexClosedSet = 0;
	bool finished = false;
	addToEndBox(openSet, indexOpenSet, boardBox[initialX][initialY]);
	if (initialX - 1 == finalX && initialY == finalY) {
		addToEndRoute(route, indexRoute, { finalX, finalY });
		addToEndRoute(route, indexRoute, { initialX, initialY });
		
	}
	else if (initialX + 1 == finalX && initialY == finalY) {
		addToEndRoute(route, indexRoute, { finalX, finalY });
		addToEndRoute(route, indexRoute, { initialX, initialY });
		
	}
	else if (initialX == finalX && initialY - 1 == finalY) {
		addToEndRoute(route, indexRoute, { finalX, finalY });
		addToEndRoute(route, indexRoute, { initialX, initialY });
		
	}
	else if (initialX == finalX && initialY + 1 == finalY) {
		addToEndRoute(route, indexRoute, { finalX, finalY });
		addToEndRoute(route, indexRoute, { initialX, initialY });
		
	}
	else {
		while (indexOpenSet > 0) {
			Box actual = openSet[0];
			for (int i = 0; i < indexOpenSet; i++) {
				if (openSet[i].f < actual.f) {
					actual = openSet[i];
				}
				if (openSet[i].x == finalX && openSet[i].y == finalY) {
					finished = true;
					addToEndRoute(route, indexRoute, { finalX, finalY });
					if (finalX != actual.x || finalY != actual.y) {
						addToEndRoute(route, indexRoute, { actual.x, actual.y });
					}
					while (actual.father.x != -1) {
						actual = boardBox[actual.father.x][actual.father.y];
						addToEndRoute(route, indexRoute, { actual.x, actual.y });
					}
					break;
				}
			}
			if (finished)
				break;
			deleteOfArray(openSet, indexOpenSet, actual);
			addToEndBox(closedSet, indexClosedSet,actual);
			generateAndUpdateNeighbors(actual, boardBox[finalX][finalY], openSet, indexOpenSet, closedSet, indexClosedSet);
		}
	}
}

//generate the route to the point of disbandedRoute[indexDisbanded[i]], if this coordinate is reached, it will calculates the next route
void newDisbandedRoute(int i) {
	int xGhost = (ghostRect[i].x - boardBegin + ghostRect[i].w / 2) / sizeSquare;
	int yGhost = (ghostRect[i].y + ghostRect[i].h / 2) / sizeSquare;
	int xDisbanded = disbandedRoute[i][indexDisbanded[i]].x;
	int yDisbanded = disbandedRoute[i][indexDisbanded[i]].y;
	if (xGhost == yDisbanded && yGhost == xDisbanded) {
		if (indexDisbanded[i] < 3)
			indexDisbanded[i]++;
		else
			indexDisbanded[i] = 0;
		xDisbanded = disbandedRoute[i][indexDisbanded[i]].x;
		yDisbanded = disbandedRoute[i][indexDisbanded[i]].y;
	}
	AStar(yGhost, xGhost, xDisbanded, yDisbanded, ghostRoute[i], indexGhostRoute[i]);
	updateGhostMovement(i);
}

//opens the cage to that a ghost can pass inside of the cage
void openCage() {
	matriz[12][13] = 0;
	matriz[12][14] = 0;
	for (int i = 13; i < 16; i++) {
		for (int j = 11; j < 17; j++) {
			matriz[i][j] = 0;
		}
	}
}

//closes the cage to that no one can pass inside the cage
void closeCage() {
	matriz[12][13] = 33;
	matriz[12][14] = 33;
	for (int i = 13; i < 16; i++) {
		for (int j = 11; j < 17; j++) {
			matriz[i][j] = 38;
		}
	}
}

//generates a route of the ghost i to the cage
void newRouteToCage(int i) {
	openCage();
	int xGhost = (ghostRect[i].x - boardBegin + ghostRect[i].w / 2) / sizeSquare;
	int yGhost = (ghostRect[i].y + ghostRect[i].h / 2) / sizeSquare;
	AStar(yGhost, xGhost, 14, 14, ghostRoute[i], indexGhostRoute[i]);
	updateGhostMovement(i);
	closeCage();
}

//locks the last coordinate traveled by the ghost i
void lockLastCoordinate(int i) {
	if ((lastCoordinate[i].x != 6 || lastCoordinate[i].y != 14) && (lastCoordinate[i].x != 21 || lastCoordinate[i].y != 14))
		matriz[lastCoordinate[i].x][lastCoordinate[i].y] = 11;
}

//unlocks the last coordinate traveled by the ghost i
void unlockLastCoordinate(int i) {
	matriz[lastCoordinate[i].x][lastCoordinate[i].y] = elementLastCoordinate[i];
}

//generates a new disbanded route, with no return movements
void generateDisbandedRoute(int i) {
	indexGhostRoute[i] = 0;
	lockLastCoordinate(i);
	newDisbandedRoute(i);
	unlockLastCoordinate(i);
	updateToActualCoordinate(i);
}

//generate a route to the botton of the alley or to the exit of the alley
void routeInAlley(int i, SDL_Point route[], int& indexRoute) {
	int xGhost = (ghostRect[i].x - boardBegin + ghostRect[i].w / 2) / sizeSquare;
	if (xGhost < 6) {
		//left alley
		if (ghostMovement[i] == 0) {
			//enter alley
			for (int i = 0; i <= xGhost; i++) {
				addToEndRoute(route, indexRoute, { 14, i });
			}
		}
		else {
			//leaves alley
			for (int i = 6; i >= xGhost; i--) {
				addToEndRoute(route, indexRoute, { 14, i });
			}
		}
	}
	else {
		//right alley
		if (ghostMovement[i] == 2) {
			//enter alley
			for (int i = totalX - 1; i >= xGhost; i--) {
				addToEndRoute(route, indexRoute, { 14, i });
			}
		}
		else {
			//leaves alley
			for (int i = 21; i <= xGhost; i++) {
				addToEndRoute(route, indexRoute, { 14, i });
			}
		}
	}
}

//generate a route with 2 coordinates: the actual (x,y) and a free neighbor
void searchAvailableNeighbor(int x, int y, SDL_Point route[], int& indexRoute) {
	if (x > 0) {
		if (matriz[x - 1][y] < 11) {
			addToEndRoute(route, indexRoute, { x - 1, y });
			addToEndRoute(route, indexRoute, { x , y });
			return;
		}
	}
	if (x < totalY - 1) {
		if (matriz[x + 1][y] < 11) {
			addToEndRoute(route, indexRoute, { x + 1, y });
			addToEndRoute(route, indexRoute, { x , y });
			return;
		}
	}
	if (y > 0) {
		if (matriz[x][y - 1] < 11) {
			addToEndRoute(route, indexRoute, { x , y - 1 });
			addToEndRoute(route, indexRoute, { x , y });
			return;
		}
	}
	if (y < totalX - 1) {
		if (matriz[x][y + 1] < 11) {
			addToEndRoute(route, indexRoute, { x , y + 1 });
			addToEndRoute(route, indexRoute, { x , y });
			return;
		}
	}
}

//generates a new pacman route, with no return movements
void generatePacmanRoute(int i) {
	indexGhostRoute[i] = 0;
	lockLastCoordinate(i);
	newRouteToPacman(i);
	unlockLastCoordinate(i);
	updateToActualCoordinate(i);
}

void generateOtobokeRoute(int i) {
	int xPacman = (pacmanRect.x - boardBegin + pacmanRect.w / 2) / sizeSquare;;
	int yPacman = (pacmanRect.y + pacmanRect.h / 2) / sizeSquare;
	switch (i) {
	case 0:
		//inferior right corner
		if (yPacman > 15 && xPacman > 6) {
			generatePacmanRoute(i);
		}
		else {
			generateDisbandedRoute(i);
		}
		break;
	case 1:
		//superior left corner
		if (yPacman < 14 && xPacman < 15) {
			generatePacmanRoute(i);
		}
		else {
			generateDisbandedRoute(i);
		}
		break;
	case 2:
		//inferior left corner
		if (yPacman > 15 && xPacman < 21) {
			generatePacmanRoute(i);
		}
		else {
			generateDisbandedRoute(i);
		}
		break;
	case 3:
		//superior right corner
		if (yPacman < 14 && xPacman > 12) {
			generatePacmanRoute(i);
		}
		else {
			generateDisbandedRoute(i);
		}
		break;
	}
}

void generateFrontPacmanRoute(int i) {
	indexGhostRoute[i] = 0;
	lockLastCoordinate(i);
	int xPacman = (pacmanRect.x - boardBegin + pacmanRect.w / 2) / sizeSquare;;
	int yPacman = (pacmanRect.y + pacmanRect.h / 2) / sizeSquare;
	int xGhost = (ghostRect[i].x - boardBegin + ghostRect[i].w / 2) / sizeSquare;
	int yGhost = (ghostRect[i].y + ghostRect[i].h / 2) / sizeSquare;
	switch (movement) {
	case 0:
		for (int j = 4; j >= 0; j--) {
			if (xPacman > j - 1 && matriz[yPacman][xPacman - j] < 11) {
				if (yGhost == yPacman && xGhost == xPacman - j)
					searchAvailableNeighbor(yGhost, xGhost, ghostRoute[i], indexGhostRoute[i]);
				else
					AStar(yGhost, xGhost, yPacman, xPacman - j, ghostRoute[i], indexGhostRoute[i]);
				updateGhostMovement(i);
				break;
			}
		}
		break;
	case 1:
		for (int j = 4; j >= 0; j--) {
			if (yPacman > j - 1 && matriz[yPacman - j][xPacman] < 11) {
				if (yGhost == yPacman - j && xGhost == xPacman)
					searchAvailableNeighbor(yGhost, xGhost, ghostRoute[i], indexGhostRoute[i]);
				else
					AStar(yGhost, xGhost, yPacman - j, xPacman, ghostRoute[i], indexGhostRoute[i]);
				updateGhostMovement(i);
				break;
			}
		}
		break;
	case 2:
		for (int j = 4; j >= 0; j--) {
			if (xPacman < totalX - j && matriz[yPacman][xPacman + j] < 11) {
				if (yGhost == yPacman && xGhost == xPacman + j)
					searchAvailableNeighbor(yGhost, xGhost, ghostRoute[i], indexGhostRoute[i]);
				else
					AStar(yGhost, xGhost, yPacman, xPacman + j, ghostRoute[i], indexGhostRoute[i]);
				updateGhostMovement(i);
				break;
			}
		}
		break;
	case 3:
		for (int j = 4; j >= 0; j--) {
			if (yPacman < totalY - j && matriz[yPacman + j][xPacman] < 11) {
				if (yGhost == yPacman + j && xGhost == xPacman)
					searchAvailableNeighbor(yGhost, xGhost, ghostRoute[i], indexGhostRoute[i]);
				else
					AStar(yGhost, xGhost, yPacman + j, xPacman, ghostRoute[i], indexGhostRoute[i]);
				updateGhostMovement(i);
				break;
			}
		}
		break;
	}
	unlockLastCoordinate(i);
	updateToActualCoordinate(i);
}

void generateKimagureRoute(int i) {
	switch (copy) {
	case 1:
		//copy the behaviour of the pink ghost
		generateFrontPacmanRoute(i);
		break;
	case 2:
		//copy the behaviour of the orange ghost
		generateOtobokeRoute(i);
		break;
	case 3:
		//copy the behaviour of the red ghost
		generatePacmanRoute(i);
		break;
	}
}

//generates a new cage route, with no return movements
void generateCageRoute(int i) {
	indexGhostRoute[i] = 0;
	lockLastCoordinate(i);
	newRouteToCage(i);
	unlockLastCoordinate(i);
	updateToActualCoordinate(i);
}