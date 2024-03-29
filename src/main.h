#pragma once

#include <stdio.h>

#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "rmathlib.h"

#include "rtypes.h"

#define GAME_NAME 		"Chill-in-One"

#define WINDOW_WIDTH	768
#define WINDOW_HEIGHT	640

#define DESIRED_FPS 	60
#define DESIRED_DELTA	(DESIRED_FPS / 1000.0f)

#define BRICK_TILE		 		1
#define BACKGROUND_TILE 		0

#define BALL_SIZE 		16
#define HOLE_SIZE 		32
#define BRICK_SIZE 		64
#define LOGO_WIDTH		440
#define LOGO_HEIGHT		124
#define MAX_VEL_NORM 	300

#define LEVEL_COUNT 	6

#define array_count(array) (sizeof(array) / sizeof(array[0]))

#define DEBUG

typedef enum GAME_STATE{
	GS_STARTMENU = 0,
	GS_LEVEL 	 = 1,
	GS_ENDMENU	 = 2
} GAME_STATE;

typedef struct Window{
	SDL_Window* window;
	SDL_Renderer* renderer;
} Window;

typedef struct Ball{
	rm_v2f pos;
	rm_v2f vel;
	b8 isMoving;
	SDL_Texture* texture;
} Ball;

typedef struct Hole{
	rm_v2f pos;
	SDL_Texture* texture;
} Hole;

typedef struct Arrow{
	rm_v2f offsetFromBall;
	f32 angle;
	i32 width;
	i32 height;
	SDL_Texture* texture;
} Arrow;

typedef struct Audio{
	SDL_AudioDeviceID deviceID;
	u8* buffer;
	u32 length;
	SDL_AudioSpec spec;
} Audio;

typedef struct Level{
	Ball ball;
	Hole hole;
	u8 tilemap[WINDOW_HEIGHT/BRICK_SIZE][WINDOW_WIDTH/BRICK_SIZE];
	Audio song;
	b8 firstInitialized;
} Level;

typedef struct PowerBar{
	SDL_Texture* backgroundTexture;
	SDL_Texture* foregroundTexture;
	f32 currentPower;
} PowerBar;

b8 InitializeSystems(void);
b8 CreateWindow(void);
b8 LoadAssets(void);
void QuitGame(void);