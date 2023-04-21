#include "input.h"
#include "main.h"

extern b8 g_GameIsRunning;
extern b8 g_ShowDebugInfo;
extern Input g_Input;
extern Ball g_Ball;

void ProcessInput(void){
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		const u8* keyStates = SDL_GetKeyboardState(NULL);
		g_Input.debugKeyIsDown = keyStates[SDL_SCANCODE_TAB];
		g_Input.closeKeyIsDown = keyStates[SDL_SCANCODE_ESCAPE];

		static v2 mousePosWhenMousePressed = {0};

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
		if(event.button.button == SDL_BUTTON_LEFT && !g_Ball.isMoving){
			if(event.type == SDL_MOUSEBUTTONDOWN){
				mouseIsCurrentlyPressed = MTHLIB_TRUE;
				int mouseX = 0;
				int mouseY = 0;
				SDL_GetMouseState(&mouseX, &mouseY);
				mousePosWhenMousePressed = (v2){mouseX, mouseY};
			}
			if(event.type == SDL_MOUSEBUTTONUP){
				mouseIsCurrentlyPressed = MTHLIB_FALSE;
				int mouseX = 0;
				int mouseY = 0;
				SDL_GetMouseState(&mouseX, &mouseY);
				v2 mousePosWhenMouseReleased = (v2){(float)mouseX, (float)mouseY};
				v2 subtraction = SubtractV2(mousePosWhenMousePressed, mousePosWhenMouseReleased);
				if(NormV2(subtraction) >= MAX_VEL_NORM){
					g_Ball.vel = ScaleV2(UnitV2(subtraction), MAX_VEL_NORM);
				}else{
					g_Ball.vel = subtraction;
				}
				g_Ball.isMoving = MTHLIB_TRUE;
			}
		}
		g_Input.debugKeyWasDown = g_Input.debugKeyIsDown;
		g_Input.closeKeyWasDown = g_Input.closeKeyIsDown;
	}
}