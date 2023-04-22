#include "main.h"
#include "input.h"
#include "render.h"
#include "sim.h"

Window g_Window = {0};
SDL_Texture* g_BackgroundTile = NULL;
SDL_Texture* g_BrickTile = NULL;
TTF_Font* g_Font = NULL;
Hole g_Hole = {.pos = {(1 * BRICK_SIZE) + (0.5 * HOLE_SIZE), (1 * BRICK_SIZE) + (0.5 * HOLE_SIZE)}};
Ball g_Ball = {.pos = {10 * BRICK_SIZE + 1.5 * BALL_SIZE, 8 * BRICK_SIZE + 1.5 * BALL_SIZE}, .vel = {0, 0}, .isMoving = MTHLIB_FALSE};
Arrow g_Arrow = {.ballParent = &g_Ball, .offsetFromBall = {0, 0}, .width = 64, .height = 64};
Input g_Input = {0};
b8 g_GameIsRunning = MTHLIB_FALSE;
b8 g_ShowDebugInfo = MTHLIB_FALSE;
b8 g_TileMap[WINDOW_HEIGHT/64][WINDOW_WIDTH/64] = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
												   {1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1},
												   {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
												   {1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1},
												   {1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1},
												   {1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1},
												   {1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1},
												   {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
												   {1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1},
												   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

int main(void){
	if(!InitializeSystems()) goto quit;
	if(!CreateWindow()) goto quit;
	if(!LoadAssets()) goto quit;

	//Now that we had no problems initializing SDL and
	//		creating a window, we can run the game
	g_GameIsRunning = MTHLIB_TRUE;

	//Game Loop
	while(g_GameIsRunning){
		u32 frameStart = SDL_GetTicks();
		ProcessInput();
		SimulateWorld();
		RenderGraphics();
		u32 frameEnd = SDL_GetTicks();
		if((frameEnd - frameStart) < (1000 / DESIRED_FPS)){
			SDL_Delay((1000 / DESIRED_FPS) - (frameEnd - frameStart));
		}
	}
	
quit:
	//Freeing memory and quitting subsystems
	QuitGame();
	return 0;
}

b8 CreateWindow(void){
	//Creating a window
	g_Window.window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	if(g_Window.window == NULL){
		//Error creating window
		fprintf(stderr, SDL_GetError());
		return MTHLIB_FALSE;
	}

	//Creating a renderer (with GPU acceleration, we're only using VRAM pixel data, i.e. textures)
	g_Window.renderer = SDL_CreateRenderer(g_Window.window, -1, SDL_RENDERER_ACCELERATED);
	if(g_Window.renderer == NULL){
		//Error creating renderer
		fprintf(stderr, SDL_GetError());
		return MTHLIB_FALSE;
	}
	return MTHLIB_TRUE;
}

b8 InitializeSystems(void){
	//SDL Initialization
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		//Error initializing SDL
		fprintf(stderr, SDL_GetError());
		return MTHLIB_FALSE;
	}

	//SDL_image Initialization
	if((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG ){
		//Error initializing SDL_image
		fprintf(stderr, IMG_GetError());
		return MTHLIB_FALSE;
	}

	//SDL_ttf Initialization
	if(TTF_Init() < 0){
		//Error initializing SDL_ttf
		fprintf(stderr, TTF_GetError());
		return MTHLIB_FALSE;
	}
	return MTHLIB_TRUE;
}

b8 LoadAssets(void){
	//Loading images
	g_BackgroundTile = IMG_LoadTexture(g_Window.renderer, "assets/images/backgroundTile.png");	
	if(g_BackgroundTile == NULL){
		//Error loading texture
		fprintf(stderr, IMG_GetError());
		return MTHLIB_FALSE;
	}
	g_BrickTile = IMG_LoadTexture(g_Window.renderer, "assets/images/brick.png");
	if(g_BrickTile == NULL){
		//Error loading texture
		fprintf(stderr, IMG_GetError());
		return MTHLIB_FALSE;
	}
	g_Hole.texture = IMG_LoadTexture(g_Window.renderer, "assets/images/hole.png");
	if(g_Hole.texture == NULL){
		//Error loading texture
		fprintf(stderr, IMG_GetError());
		return MTHLIB_FALSE;
	}	
	g_Ball.texture = IMG_LoadTexture(g_Window.renderer, "assets/images/ball.png");
	if(g_Ball.texture == NULL){
		//Error loading texture
		fprintf(stderr, IMG_GetError());
		return MTHLIB_FALSE;
	}
	g_Arrow.texture = IMG_LoadTexture(g_Window.renderer, "assets/images/arrow.png");
	if(g_Arrow.texture == NULL){
		//Error loading texture
		fprintf(stderr, IMG_GetError());
		return MTHLIB_FALSE;
	}
	g_Font = TTF_OpenFont("assets/fonts/8bitOperatorPlus8-Regular.ttf", 32);
	if(g_Font == NULL){
		//Error loading font
		fprintf(stderr, TTF_GetError());
		return MTHLIB_FALSE;
	}
	return MTHLIB_TRUE;
}

void QuitGame(void){
	//Freeing memory
	SDL_DestroyTexture(g_Hole.texture);
	SDL_DestroyTexture(g_Ball.texture);
	SDL_DestroyTexture(g_BrickTile);
	SDL_DestroyTexture(g_BackgroundTile);
	SDL_DestroyRenderer(g_Window.renderer);
	SDL_DestroyWindow(g_Window.window);

	//Quiting subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}