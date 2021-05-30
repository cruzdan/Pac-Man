#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "Variables.h"

void changeTextStart(std::string newText) {
	TTF_Font* font = TTF_OpenFont("Oswald-BoldItalic.ttf", (int)(SCREEN_SIZE / 2.5));
	SDL_Surface* textSurface;
	SDL_Color white = { 255,255,255 };
	const char* t = newText.c_str();
	textSurface = TTF_RenderText_Solid(font, t, white);
	textStart = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FreeSurface(textSurface);
	TTF_CloseFont(font);
}

//changes the text of the start text every 1 second
void start() {
	if (timerStart % 1000 == 0) {
		int number = timerStart / 1000 + 1;
		std::string num_str(std::to_string(number));
		changeTextStart(num_str);
	}
	timerStart += MS;
	if (timerStart >= 3000) {
		timerStart = 0;
		started = false;
	}
}

void showBoard() {
	SDL_Rect rect;
	rect.w = sizeSquare;
	rect.h = rect.w;
	for (int i = 0; i < totalX; i++) {
		for (int j = 0; j < totalY; j++) {
			switch (matriz[j][i]) {
			case 0:
			case 38:
				break;
			case 1:
				littleBallRect.x = (boardBegin + rect.w * i) + littleBallRect.w / 2;
				littleBallRect.y = (rect.h * j) + littleBallRect.h / 2;
				SDL_RenderCopy(renderer, totalTexture, &imageBallRect, &littleBallRect);
				break;
			case 2:
				rect.x = boardBegin + rect.w * i;
				rect.y = rect.h * j;
				SDL_RenderCopy(renderer, totalTexture, &imageBallRect, &rect);
				break;
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
				rect.x = boardBegin + rect.w * i;
				rect.y = rect.h * j;
				SDL_RenderCopy(renderer, totalTexture, &fruitsImageRect[indexFruit], &rect);
				break;
			default:
				rect.x = boardBegin + rect.w * i;
				rect.y = rect.h * j;
				SDL_RenderCopy(renderer, mapTexture, &colisionsImageRect[matriz[j][i] - 11], &rect);
			}
		}
	}
}

void showMenu() {
	SDL_RenderCopy(renderer, gameTexture, NULL, &gameRect);
	SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
	SDL_RenderCopy(renderer, puntuationTexture, NULL, &puntuationRect);
	SDL_RenderCopy(renderer, livesTexture, NULL, &livesRect);
	for (int i = 0; i < lives; i++) {
		SDL_Rect rect = { 4 * sizeImageSquare, sizeImageSquare, 2 * sizeImageSquare, 2 * sizeImageSquare };
		pacmanLifeRect.x = livesRect.x + livesRect.w + pacmanLifeRect.w * i;
		SDL_RenderCopy(renderer, totalTexture, &rect, &pacmanLifeRect);
	}
	SDL_SetRenderDrawColor(renderer, 13, 151, 182, 0);
	showBoard();
}

void showCharacters() {
	if (!pacmanDies) {
		SDL_Rect rect;
		if (pacmanChange)
			rect = imagePacmanRect[1];
		else
			rect = imagePacmanRect[0];
		switch (movement) {
		case 0:
			SDL_RenderCopyEx(renderer, totalTexture, &rect, &pacmanRect, 180, NULL, SDL_FLIP_NONE);
			break;
		case 1:
			SDL_RenderCopyEx(renderer, totalTexture, &rect, &pacmanRect, 270, NULL, SDL_FLIP_NONE);
			break;
		case 2:
			SDL_RenderCopy(renderer, totalTexture, &rect, &pacmanRect);
			break;
		case 3:
			SDL_RenderCopyEx(renderer, totalTexture, &rect, &pacmanRect, 90, NULL, SDL_FLIP_NONE);
			break;
		}
	}
	else {
		SDL_RenderCopy(renderer, totalTexture, &imagePacmanRect[1], &pacmanRect);
	}
	for (int i = 0; i < 4; i++) {
		switch (ghostState[i]) {
		case 0:
			if (ghostChange[i])
				SDL_RenderCopy(renderer, totalTexture, &imageGhostRect[i], &ghostRect[i]);
			else
				SDL_RenderCopy(renderer, totalTexture, &imageGhostRect[i + 5], &ghostRect[i]);
			break;
		case 1:
			if (ghostChange[i])
				SDL_RenderCopy(renderer, totalTexture, &imageGhostRect[4], &ghostRect[i]);
			else
				SDL_RenderCopy(renderer, totalTexture, &imageGhostRect[9], &ghostRect[i]);
			break;
		case 2:
			if (ghostChange[i])
				SDL_RenderCopy(renderer, totalTexture, &imageGhostRect[4], &ghostRect[i]);
			else
				SDL_RenderCopy(renderer, totalTexture, &imageGhostRect[10], &ghostRect[i]);
			break;
		case 3:
			SDL_RenderCopy(renderer, totalTexture, &imageGhostRect[11 + i], &ghostRect[i]);
			break;
		}
	}
}

void show() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	showMenu();
	showCharacters();
	if (started) {
		start();
		SDL_RenderCopy(renderer, textStart, NULL, &startRect);
	}
	if (eated) {
		SDL_RenderCopy(renderer, totalTexture, &eatPuntuationRect, &ghostRect[indexEatedGhost]);
	}
	//show the score fruit rect on the board
	if (eatedFruit) {
		SDL_Rect rect;
		//first fruit
		if (totalBalls > 55)
			rect = { 14 * sizeSquare + boardBegin, 23 * sizeSquare, sizeSquare,sizeSquare };
		else//second fruit
			rect = { 13 * sizeSquare + boardBegin, 23 * sizeSquare, sizeSquare,sizeSquare };
		SDL_RenderCopy(renderer, totalTexture, &scoreFruitImageRect[indexFruit], &rect);
	}
	SDL_RenderPresent(renderer);
}