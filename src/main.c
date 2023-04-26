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
PowerBar g_PowerBar = {.currentPower = 0.0f};
b8 g_GameIsRunning = MTHLIB_FALSE;
b8 g_ShowDebugInfo = MTHLIB_FALSE;
i32 g_StrokeCounter = 0;
SDL_AudioSpec g_SongSpec = {0};
u32 g_SongLength = 0;
u8* g_SongBuffer = NULL;
SDL_AudioSpec g_CollisionAudioSpec = {0};
u32 g_CollisionAudioLength = 0;
u8* g_CollisionAudioBuffer = NULL;
SDL_AudioDeviceID g_SongDevice = 0;
SDL_AudioDeviceID g_AudioDevice = 0;
b8 g_TileMap[WINDOW_HEIGHT/BRICK_SIZE][WINDOW_WIDTH/BRICK_SIZE] = { {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
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
	int status = SDL_QueueAudio(g_SongDevice, g_SongBuffer, g_SongLength);
	if(status < 0){
		fprintf(stderr, SDL_GetError());
	}
	SDL_PauseAudioDevice(g_SongDevice, 0);
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
	SDL_PauseAudioDevice(g_SongDevice, 0);
	
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
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0){
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
	g_PowerBar.backgroundTexture = IMG_LoadTexture(g_Window.renderer, "assets/images/powerMeterBackground.png");
	if(g_PowerBar.backgroundTexture == NULL){
		//Error loading texture
		fprintf(stderr, IMG_GetError());
		return MTHLIB_FALSE;
	}
	g_PowerBar.foregroundTexture = IMG_LoadTexture(g_Window.renderer, "assets/images/powerMeterForeground.png");
	if(g_PowerBar.foregroundTexture == NULL){
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
	if(SDL_LoadWAV("assets/sounds/music/Song2-lowVolume.wav", &g_SongSpec, &g_SongBuffer, &g_SongLength) == NULL){
		//Error loading wav
		fprintf(stderr, SDL_GetError());
		return MTHLIB_FALSE;
	}
	if(SDL_LoadWAV("assets/sounds/sfx/Hit_Hurt3.wav", &g_CollisionAudioSpec, &g_CollisionAudioBuffer, &g_CollisionAudioLength) == NULL){
		//Error loading wav
		fprintf(stderr, SDL_GetError());
		return MTHLIB_FALSE;
	}
	g_SongDevice = SDL_OpenAudioDevice(NULL, 0, &g_SongSpec, NULL, 0);
	if(g_SongDevice == 0){
		fprintf(stderr, SDL_GetError());
		return MTHLIB_FALSE;
	}
	g_AudioDevice = SDL_OpenAudioDevice(NULL, 0, &g_SongSpec, NULL, 0);
	if(g_AudioDevice == 0){
		fprintf(stderr, SDL_GetError());
		return MTHLIB_FALSE;
	}
	return MTHLIB_TRUE;
}

void QuitGame(void){
	SDL_DestroyTexture(g_Hole.texture);
	SDL_DestroyTexture(g_Ball.texture);
	SDL_DestroyTexture(g_BrickTile);
	SDL_DestroyTexture(g_BackgroundTile);
	SDL_DestroyTexture(g_Arrow.texture);
	SDL_DestroyTexture(g_PowerBar.backgroundTexture);
	SDL_DestroyTexture(g_PowerBar.foregroundTexture);
	
	SDL_DestroyRenderer(g_Window.renderer);
	SDL_DestroyWindow(g_Window.window);
	
	TTF_CloseFont(g_Font);

	SDL_FreeWAV(g_SongBuffer);
	SDL_FreeWAV(g_CollisionAudioBuffer);

	SDL_CloseAudioDevice(g_SongDevice);
	SDL_CloseAudioDevice(g_AudioDevice);

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}