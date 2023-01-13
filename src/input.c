#include "input.h"
#include "main.h"

extern b8 g_GameIsRunning;
extern b8 g_ShowDebugInfo;
extern Input g_Input;

void ProcessInput(void){
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		const u8* keyStates = SDL_GetKeyboardState(NULL);
		g_Input.debugKeyIsDown = keyStates[SDL_SCANCODE_TAB];
		g_Input.closeKeyIsDown = keyStates[SDL_SCANCODE_ESCAPE];
		
		if(event.type == SDL_QUIT){
			g_GameIsRunning = MTHLIB_FALSE;
		}
		if(event.type == SDL_KEYDOWN){
			if(g_Input.closeKeyIsDown  && !g_Input.closeKeyWasDown){
				g_GameIsRunning = MTHLIB_FALSE;
			}
			if(g_Input.debugKeyIsDown && !g_Input.debugKeyWasDown){
				g_ShowDebugInfo = !g_ShowDebugInfo;
			}
		}
		
		UpdateWasKeys();
	}
	
}

void UpdateWasKeys(void){
	g_Input.debugKeyWasDown = g_Input.debugKeyIsDown;
	g_Input.closeKeyWasDown = g_Input.closeKeyIsDown;
}