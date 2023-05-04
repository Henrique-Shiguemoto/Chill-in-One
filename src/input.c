#include <math.h>
#include "input.h"
#include "main.h"

extern b8 g_GameIsRunning;
extern b8 g_ShowDebugInfo;
extern GAME_STATE g_GameState;
extern Input g_Input;
extern Level* level;
extern Arrow g_Arrow;
extern PowerBar g_PowerBar;
extern i32 g_StrokeCounter;

static v2 mousePosWhenMousePressed = {0};
static b8 mouseIsPressed = MTHLIB_FALSE;

void ProcessInputStartMenu(void){
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
		if(event.button.button == SDL_BUTTON_LEFT){
			if(event.type == SDL_MOUSEBUTTONDOWN){
				//verify that the mouse position is at the top of a button
				v2 mousePosition = GetMousePosition();

				//Hard coding for now (could create some sort of Button struct and so on...)
				// (v2){0.5 * WINDOW_WIDTH - (225 / 2), 0.5 * WINDOW_HEIGHT - 0.5 * LOGO_HEIGHT + 160}, (v2){225, 25}
				// (v2){0.5 * WINDOW_WIDTH - (75 / 2), 0.5 * WINDOW_HEIGHT - 0.5 * LOGO_HEIGHT + 230}, (v2){75, 25}
				AABB2D playButtonAABB = (AABB2D){.min = {0.5 * WINDOW_WIDTH - (225 / 2), 
														 0.5 * WINDOW_HEIGHT - 0.5 * LOGO_HEIGHT + 160}, 
												 .max = {0.5 * WINDOW_WIDTH - (225 / 2) + 255, 
														 0.5 * WINDOW_HEIGHT - 0.5 * LOGO_HEIGHT + 160 + 25}};
				AABB2D quitButtonAABB = (AABB2D){.min = {0.5 * WINDOW_WIDTH - (75 / 2),
														 0.5 * WINDOW_HEIGHT - 0.5 * LOGO_HEIGHT + 230}, 
												 .max = {0.5 * WINDOW_WIDTH - (75 / 2) + 75,
														 0.5 * WINDOW_HEIGHT - 0.5 * LOGO_HEIGHT + 230 + 25}};
				if(CollisionPointAndAABB2D(mousePosition, playButtonAABB)){
					g_GameState = GS_LEVEL;
					level->firstInitialized = MTHLIB_TRUE;
				}
				if(CollisionPointAndAABB2D(mousePosition, quitButtonAABB)){
					g_GameIsRunning = MTHLIB_FALSE;
				}

			}
		}
		g_Input.debugKeyWasDown = g_Input.debugKeyIsDown;
		g_Input.closeKeyWasDown = g_Input.closeKeyIsDown;
	}
}

void ProcessInputEndMenu(void){
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
		g_Input.debugKeyWasDown = g_Input.debugKeyIsDown;
		g_Input.closeKeyWasDown = g_Input.closeKeyIsDown;
	}	
}

void ProcessInputLevel(void){
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
		if(event.button.button == SDL_BUTTON_LEFT && !level->ball.isMoving){
			if(event.type == SDL_MOUSEBUTTONDOWN){
				mouseIsPressed = MTHLIB_TRUE;
				mousePosWhenMousePressed = GetMousePosition();
			}
			if(event.type == SDL_MOUSEBUTTONUP && mouseIsPressed){
				mouseIsPressed = MTHLIB_FALSE;
				v2 mousePosWhenMouseReleased = GetMousePosition();
				v2 subtraction = SubtractV2(mousePosWhenMousePressed, mousePosWhenMouseReleased);
				if(NormV2(subtraction) >= MAX_VEL_NORM){
					level->ball.vel = ScaleV2(UnitV2(subtraction), MAX_VEL_NORM);
				}else{
					level->ball.vel = subtraction;
				}
				level->ball.isMoving = MTHLIB_TRUE;
				g_StrokeCounter++;
			}
		}
		
		g_Input.debugKeyWasDown = g_Input.debugKeyIsDown;
		g_Input.closeKeyWasDown = g_Input.closeKeyIsDown;
	}
	UpdateArrowAngle();
	UpdatePowerBar();
}

void ProcessInput(void){
	switch(g_GameState){
		case GS_STARTMENU:{
			ProcessInputStartMenu();
			break;
		}
		case GS_LEVEL:{
			ProcessInputLevel();
			break;
		}
		case GS_ENDMENU:{
			ProcessInputEndMenu();
			break;
		}
		default:{
			fprintf(stderr, "Invalid Game State\n");
			break;
		}
	}
}

void UpdateArrowAngle(void){
	//Getting mouse position
	v2 mousePosition = GetMousePosition();

	//Calculating arrow direction
	v2 ballCenter = (v2){level->ball.pos.x + 0.5*BALL_SIZE, level->ball.pos.y + 0.5*BALL_SIZE};
	v2 dir = SubtractV2(ballCenter, mousePosition);
	
	//Unit X axis vector
	const v2 horizontalVector = (v2){1.0f, 0.0f};
	
	//Angle operations
	f32 dirNorm = NormV2(dir);
	if (dirNorm != 0.0f){
		f32 angleInRadians = 0.0f;
		f32 angleInDegrees = 0.0f;
		if(level->ball.pos.y < mousePosition.y){
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

void UpdatePowerBar(void){
	if(mouseIsPressed){
		v2 currentMousePosition = GetMousePosition();
		v2 subtraction = SubtractV2(mousePosWhenMousePressed, currentMousePosition);
		f32 subSize = NormV2(subtraction);
		f32 clampedSize = Clamp32(0, MAX_VEL_NORM, subSize);
		g_PowerBar.currentPower = clampedSize;
	}else{
		g_PowerBar.currentPower = 0.0f;
	}
}

v2 GetMousePosition(void){
	int mouseX = 0;
	int mouseY = 0;
	SDL_GetMouseState(&mouseX, &mouseY);
	return (v2){mouseX, mouseY};
}