#include <SDL.h>
void detectKey() {
	SDL_Event event;
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	while (SDL_PollEvent(&event)) {
		extern bool gameOver;
		int typeEvent = event.type;
		if (typeEvent == SDL_QUIT) {
			gameOver = true;
		}
		else if (typeEvent == SDL_KEYDOWN) {
			extern int nextMovement;
			if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT]) {
				nextMovement = 0;
			}
			if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]) {
				nextMovement = 2;
			}
			if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP]) {
				nextMovement = 1;
			}
			if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN]) {
				nextMovement = 3;
			}
			if (keys[SDL_SCANCODE_P]) {
				extern bool pause;
				pause = !pause;
			}
			if (keys[SDL_SCANCODE_ESCAPE]) {
				gameOver = true;
			}
		}
	}
}