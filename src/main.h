#pragma once

#include <stdio.h>

#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "mthlib.h"

#define WINDOW_WIDTH	768
#define WINDOW_HEIGHT	640

#define DESIRED_FPS 	60
#define DESIRED_DELTA	(DESIRED_FPS / 1000.0f)

#define BRICK_TILE		 		1
#define BACKGROUND_TILE 		0

#define BALL_SIZE 		16
#define HOLE_SIZE 		48
#define BRICK_SIZE 		64

#define array_count(array) (sizeof(array) / sizeof(array[0]))

#define DEBUG

typedef struct Window{
	SDL_Window* window;
	SDL_Renderer* renderer;
} Window;

typedef struct Ball{
	v2 pos;
	v2 vel;
	SDL_Texture* texture;
} Ball;

typedef struct Hole{
	v2 pos;
	SDL_Texture* texture;
} Hole;

b8 InitializeSystems(void);
b8 CreateWindow(void);
b8 LoadAssets(void);
void QuitGame(void);