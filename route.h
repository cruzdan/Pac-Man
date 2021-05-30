#pragma once
#include <SDL.h>
//struct to calculate a route with a*
struct Box {
	int x;
	int y;
	int f;
	int g;
	int h;
	SDL_Point father;
};
void AStar(int initialX, int initialY, int finalX, int finalY, SDL_Point route[], int& indexRoute);
void newDisbandedRoute(int i);
void addToEndBox(Box array[], int& index, Box element);
void addToEndRoute(SDL_Point array[], int& index, SDL_Point element);
void deleteOfArray(Box array[], int& index, Box element);
bool arrayContains(Box array[], int index, Box element);
int heuristic(Box a, Box b);
void updateDataArray(Box array[], int index, Box element);
void updateNeighbor(int xI, int yI, Box finalBox, Box actual, Box openSet[], int& indexOpenSet, Box closedSet[], int& indexClosedSet);
void generateAndUpdateNeighbors(Box actual, Box finalBox, Box openSet[], int& indexOpenSet, Box closedSet[], int& indexClosedSet);
int getDirection(SDL_Point p1, SDL_Point p2);
void newRouteToPacman(int i);
void newRandomRoute(int i);
void updateGhostMovement(int i);
void updateToActualCoordinate(int i);
void newRouteToCage(int i);
void openCage();
void closeCage();
void generateDisbandedRoute(int i);
void lockLastCoordinate(int i);
void unlockLastCoordinate(int i);
void routeInAlley(int i, SDL_Point route[], int& indexRoute);
void generateKimagureRoute(int i);
void generateFrontPacmanRoute(int i);
void searchAvailableNeighbor(int x, int y, SDL_Point route[], int& indexRoute);
void generateOtobokeRoute(int i);
void generatePacmanRoute(int i);
void generateCageRoute(int i);

