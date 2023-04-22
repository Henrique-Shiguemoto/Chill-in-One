#include <math.h>
#include "input.h"
#include "main.h"

extern b8 g_GameIsRunning;
extern b8 g_ShowDebugInfo;
extern Input g_Input;
extern Ball g_Ball;
extern Arrow g_Arrow;

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
				int mouseX = 0;
				int mouseY = 0;
				SDL_GetMouseState(&mouseX, &mouseY);
				mousePosWhenMousePressed = (v2){mouseX, mouseY};
			}
			if(event.type == SDL_MOUSEBUTTONUP){
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
	UpdateArrowAngle();
}

void UpdateArrowAngle(void){
	//Getting mouse position
	int mouseX = 0;
	int mouseY = 0;
	SDL_GetMouseState(&mouseX, &mouseY);
	v2 mousePosition = (v2){mouseX, mouseY};

	//Calculating arrow direction
	v2 ballCenter = (v2){g_Ball.pos.x + 0.5*BALL_SIZE, g_Ball.pos.y + 0.5*BALL_SIZE};
	v2 dir = SubtractV2(ballCenter, mousePosition);
	
	//Unit X axis vector
	const v2 horizontalVector = (v2){1.0f, 0.0f};
	
	//Angle operations
	f32 dirNorm = NormV2(dir);
	if (dirNorm != 0.0f){
		f32 angleInRadians = 0.0f;
		f32 angleInDegrees = 0.0f;
		if(g_Ball.pos.y < mouseY){
			//Mouse is under the ball
			angleInRadians = acosf(DotV2(ScaleV2(dir, -1), horizontalVector) / dirNorm);
			angleInDegrees += 270;
		}else{
			angleInRadians = MTHLIB_PI - acosf(DotV2(ScaleV2(dir, -1), horizontalVector) / dirNorm);
			angleInDegrees += 90;
		}
		angleInDegrees += angleInRadians * (180/MTHLIB_PI);
		g_Arrow.angle = angleInDegrees;
	}
}