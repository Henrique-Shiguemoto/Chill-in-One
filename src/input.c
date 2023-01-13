#include "input.h"
#include "main.h"

extern b8 g_GameIsRunning;

void ProcessInput(void){
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		switch(event.type){
			case SDL_QUIT: {
				g_GameIsRunning = MTHLIB_FALSE;
				break;
			}
			case SDL_KEYDOWN: {
				//Closing game with escape key
				if(event.key.keysym.sym == SDLK_ESCAPE){
					g_GameIsRunning = MTHLIB_FALSE;
				}
				break;
			}
			case SDL_MOUSEMOTION: {
				break;
			}
		}
	}
}