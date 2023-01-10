#include <stdio.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>

int main(void){
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		//Error initializing SDL
		fprintf(stderr, SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Quit();
	return EXIT_SUCCESS;
}