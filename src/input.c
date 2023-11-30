#include <math.h> //for acosf (haven't implemented acosf in rmathlib.h)

#include "input.h"
#include "parseLevel.h"
#include "main.h"

extern b8 g_GameIsRunning;
extern b8 g_ShowDebugInfo;
extern GAME_STATE g_GameState;
extern Input g_Input;
extern Level* level;
extern Arrow g_Arrow;
extern PowerBar g_PowerBar;
extern i32 g_StrokeCounter;

extern const char* g_LevelPaths[];
extern const char* g_LevelSongPaths[];
extern i32 g_CurrentLevel;

static rm_v2f mousePosWhenMousePressed = {0};
static b8 mouseIsPressed = RMATH_FALSE;

void ProcessInputStartMenu(void){
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		const u8* keyStates = SDL_GetKeyboardState(NULL);
		g_Input.debugKeyIsDown = keyStates[SDL_SCANCODE_TAB];
		g_Input.closeKeyIsDown = keyStates[SDL_SCANCODE_ESCAPE];

		if(event.type == SDL_QUIT){
			g_GameIsRunning = RMATH_FALSE;
		}
		if(event.type == SDL_KEYDOWN){
			if(g_Input.closeKeyIsDown  && !g_Input.closeKeyWasDown){
				g_GameIsRunning = RMATH_FALSE;
			}
			if(g_Input.debugKeyIsDown && !g_Input.debugKeyWasDown){
				g_ShowDebugInfo = !g_ShowDebugInfo;
			}
		}
		if(event.button.button == SDL_BUTTON_LEFT){
			if(event.type == SDL_MOUSEBUTTONDOWN){
				//verify that the mouse position is at the top of a button
				rm_v2f mousePosition = GetMousePosition();

				//Hard coding for now (could create some sort of Button struct and so on...)
				// (v2){0.5 * WINDOW_WIDTH - (225 / 2), 0.5 * WINDOW_HEIGHT - 0.5 * LOGO_HEIGHT + 160}, (v2){225, 25}
				// (v2){0.5 * WINDOW_WIDTH - (75 / 2), 0.5 * WINDOW_HEIGHT - 0.5 * LOGO_HEIGHT + 230}, (v2){75, 25}
				rm_AABB2D playButtonAABB = (rm_AABB2D){.min = {0.5 * WINDOW_WIDTH - (225 / 2), 
														 0.5 * WINDOW_HEIGHT - 0.5 * LOGO_HEIGHT + 160}, 
	        										   .max = {0.5 * WINDOW_WIDTH - (225 / 2) + 255, 
														 0.5 * WINDOW_HEIGHT - 0.5 * LOGO_HEIGHT + 160 + 25}};
				rm_AABB2D quitButtonAABB = (rm_AABB2D){.min = {0.5 * WINDOW_WIDTH - (75 / 2),
														 0.5 * WINDOW_HEIGHT - 0.5 * LOGO_HEIGHT + 230}, 
												 	   .max = {0.5 * WINDOW_WIDTH - (75 / 2) + 75,
														 0.5 * WINDOW_HEIGHT - 0.5 * LOGO_HEIGHT + 230 + 25}};
				if(rm_collision_point_AABB2D(mousePosition, playButtonAABB)){
					g_GameState = GS_LEVEL;
					level->firstInitialized = RMATH_TRUE;
				}
				if(rm_collision_point_AABB2D(mousePosition, quitButtonAABB)){
					g_GameIsRunning = RMATH_FALSE;
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
			g_GameIsRunning = RMATH_FALSE;
		}
		if(event.type == SDL_KEYDOWN){
			if(g_Input.closeKeyIsDown  && !g_Input.closeKeyWasDown){
				g_GameIsRunning = RMATH_FALSE;
			}
			if(g_Input.debugKeyIsDown && !g_Input.debugKeyWasDown){
				g_ShowDebugInfo = !g_ShowDebugInfo;
			}
			if(event.key.keysym.sym == SDLK_RETURN){
				g_GameState = GS_LEVEL;
				g_CurrentLevel = 0;
				level = CreateLevel(g_LevelPaths[g_CurrentLevel], g_LevelSongPaths[g_CurrentLevel]);
				level->firstInitialized = RMATH_TRUE;
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
			g_GameIsRunning = RMATH_FALSE;
		}
		if(event.type == SDL_KEYDOWN){
			if(g_Input.closeKeyIsDown  && !g_Input.closeKeyWasDown){
				g_GameIsRunning = RMATH_FALSE;
			}
			if(g_Input.debugKeyIsDown && !g_Input.debugKeyWasDown){
				g_ShowDebugInfo = !g_ShowDebugInfo;
			}
		}
		if(event.button.button == SDL_BUTTON_LEFT && !level->ball.isMoving){
			if(event.type == SDL_MOUSEBUTTONDOWN){
				mouseIsPressed = RMATH_TRUE;
				mousePosWhenMousePressed = GetMousePosition();
			}
			if(event.type == SDL_MOUSEBUTTONUP && mouseIsPressed){
				mouseIsPressed = RMATH_FALSE;
				rm_v2f mousePosWhenMouseReleased = GetMousePosition();
				rm_v2f subtraction = rm_sub_v2f(mousePosWhenMousePressed, mousePosWhenMouseReleased);
				if(rm_mag_v2f(subtraction) >= MAX_VEL_NORM){
					level->ball.vel = rm_scale_v2f(rm_unit_v2f(subtraction), MAX_VEL_NORM);
				}else{
					level->ball.vel = subtraction;
				}
				level->ball.isMoving = RMATH_TRUE;
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
	rm_v2f mousePosition = GetMousePosition();

	//Calculating arrow direction
	rm_v2f ballCenter = (rm_v2f){level->ball.pos.x + 0.5*BALL_SIZE, level->ball.pos.y + 0.5*BALL_SIZE};
	rm_v2f dir = rm_sub_v2f(ballCenter, mousePosition);
	
	//Unit X axis vector
	const rm_v2f horizontalVector = (rm_v2f){1.0f, 0.0f};
	
	//Angle operations
	f32 dirNorm = rm_mag_v2f(dir);
	if (dirNorm != 0.0f){
		f32 angleInRadians = 0.0f;
		f32 angleInDegrees = 0.0f;
		if(level->ball.pos.y < mousePosition.y){
			//Mouse is under the ball
			angleInRadians = acosf(rm_dot_v2f(rm_scale_v2f(dir, -1), horizontalVector) / dirNorm);
			angleInDegrees += 270;
		}else{
			angleInRadians = RMATH_PI - acosf(rm_dot_v2f(rm_scale_v2f(dir, -1), horizontalVector) / dirNorm);
			angleInDegrees += 90;
		}
		angleInDegrees += angleInRadians * (180/RMATH_PI);
		g_Arrow.angle = angleInDegrees;
	}
}

void UpdatePowerBar(void){
	if(mouseIsPressed){
		rm_v2f currentMousePosition = GetMousePosition();
		rm_v2f subtraction = rm_sub_v2f(mousePosWhenMousePressed, currentMousePosition);
		f32 subSize = rm_mag_v2f(subtraction);
		f32 clampedSize = rm_clamp32(0, MAX_VEL_NORM, subSize);
		g_PowerBar.currentPower = clampedSize;
	}else{
		g_PowerBar.currentPower = 0.0f;
	}
}

rm_v2f GetMousePosition(void){
	int mouseX = 0;
	int mouseY = 0;
	SDL_GetMouseState(&mouseX, &mouseY);
	return (rm_v2f){mouseX, mouseY};
}