#include <stdio.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "mthlib.h"

SDL_Window* g_Window = {0};
b8 g_GameIsRunning = MTHLIB_FALSE;

void ProcessInput(){
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		switch(event.type){
			case SDL_QUIT: {
				g_GameIsRunning = MTHLIB_FALSE;
				break;
			}
		}
	}
}

void SimulateWorld(){

}

void RenderGraphics(){

}

int main(void){
	//SDL Initialization
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		//Error initializing SDL
		fprintf(stderr, SDL_GetError());
		return EXIT_FAILURE;
	}

	//Creating a window
	SDL_Window* g_Window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_RESIZABLE);
	if(g_Window == NULL){
		//Error creating window
		fprintf(stderr, SDL_GetError());
		return EXIT_FAILURE;
	}

	//Now that we had no problems initializing SDL and
	//		creating a window, we can run the game
	g_GameIsRunning = MTHLIB_TRUE;

	//Game Loop
	while(g_GameIsRunning){
		ProcessInput();
		SimulateWorld();
		RenderGraphics();
	}

	SDL_DestroyWindow(g_Window);
	SDL_Quit();
	return EXIT_SUCCESS;
}