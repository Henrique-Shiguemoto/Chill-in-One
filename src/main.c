#include <stdio.h>
#include <stdbool.h>

#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "SDL_image.h"
#include "mthlib.h"

#define WINDOW_WIDTH	640
#define WINDOW_HEIGHT	640

SDL_Window* g_Window = {0};
SDL_Renderer* g_Renderer = {0};
SDL_Surface* g_BackgroundTileSurface = {0};
SDL_Texture* g_BackgroundTileTexture = {0};
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
	SDL_RenderClear(g_Renderer);
	//Render background tiles
	int width = g_BackgroundTileSurface->w;
	int height = g_BackgroundTileSurface->h;
	for(int y = 0; y < WINDOW_HEIGHT / height; y++){
		for(int x = 0; x < WINDOW_WIDTH / width; x++){
			SDL_Rect destRect = {x * width, y * height, width, height};
			SDL_RenderCopy(g_Renderer, g_BackgroundTileTexture, NULL, &destRect);
		}
	}
	SDL_RenderPresent(g_Renderer);
}

int main(void){
	//SDL Initialization
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		//Error initializing SDL
		fprintf(stderr, SDL_GetError());
		return EXIT_FAILURE;
	}

	//SDL_image Initialization
	if((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG ){
		//Error initializing SDL_image
		fprintf(stderr, IMG_GetError());
		SDL_Quit();
		return EXIT_FAILURE;
	}

	//Creating a window
	g_Window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	if(g_Window == NULL){
		//Error creating window
		fprintf(stderr, SDL_GetError());
		return EXIT_FAILURE;
	}

	//Creating a renderer (with GPU acceleration, we're only using VRAM pixel data, i.e. textures)
	g_Renderer = SDL_CreateRenderer(g_Window, -1, SDL_RENDERER_ACCELERATED);
	if(g_Renderer == NULL){
		//Error creating renderer
		fprintf(stderr, SDL_GetError());
		return EXIT_FAILURE;	
	}

	//Loading images
	g_BackgroundTileSurface = IMG_Load("assets/images/backgroundTile.png");
	g_BackgroundTileTexture = SDL_CreateTextureFromSurface(g_Renderer, g_BackgroundTileSurface);

	//Now that we had no problems initializing SDL and
	//		creating a window, we can run the game
	g_GameIsRunning = MTHLIB_TRUE;

	//Game Loop
	while(g_GameIsRunning){
		ProcessInput();
		SimulateWorld();
		RenderGraphics();
	}

	//Freeing memory
	SDL_FreeSurface(g_BackgroundTileSurface);
	SDL_DestroyTexture(g_BackgroundTileTexture);
	SDL_DestroyRenderer(g_Renderer);
	SDL_DestroyWindow(g_Window);

	//Quiting subsystems
	IMG_Quit();
	SDL_Quit();
	return EXIT_SUCCESS;
}