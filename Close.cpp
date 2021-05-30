#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
void closeTextures() {
	extern SDL_Texture* totalTexture;
	extern SDL_Texture* mapTexture;
	extern SDL_Texture* gameTexture;
	extern SDL_Texture* scoreTexture;
	extern SDL_Texture* livesTexture;
	extern SDL_Texture* puntuationTexture;
	extern SDL_Texture* textStart;

	SDL_DestroyTexture(gameTexture);
	SDL_DestroyTexture(scoreTexture);
	SDL_DestroyTexture(puntuationTexture);
	SDL_DestroyTexture(livesTexture);
	SDL_DestroyTexture(totalTexture);
	SDL_DestroyTexture(mapTexture);
	SDL_DestroyTexture(textStart);
}

void close() {
	extern SDL_Window* window;
	extern SDL_Renderer* renderer;
	closeTextures();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}