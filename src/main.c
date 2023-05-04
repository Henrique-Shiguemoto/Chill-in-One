#include "main.h"
#include "input.h"
#include "render.h"
#include "sim.h"
#include "parseLevel.h"
#include "audio.h"

Window g_Window = {0};
SDL_Texture* g_BackgroundTile = NULL;
SDL_Texture* g_BrickTile = NULL;
SDL_Texture* g_LogoTexture = NULL;
TTF_Font* g_Font = NULL;

GAME_STATE g_GameState = 0;

Level* level = NULL;
i32 g_CurrentLevel = 0;
const char* g_LevelPaths[] = {"src/levels/lvl1.txt",
							"src/levels/lvl2.txt",
							"src/levels/lvl3.txt"};
const char* g_LevelSongPaths[] = {"assets/sounds/music/Song2-lowVolume.wav",
								"assets/sounds/music/Song4-lowVolume.wav",
								"assets/sounds/music/Song5-lowVolume.wav"};

Arrow g_Arrow = {.offsetFromBall = {0, 0}, .width = 64, .height = 64};
PowerBar g_PowerBar = {.currentPower = 0.0f};
i32 g_StrokeCounter = 0;

Input g_Input = {0};

b8 g_GameIsRunning = MTHLIB_FALSE;
b8 g_ShowDebugInfo = MTHLIB_FALSE;

Audio g_CollisionSFX = {0};
Audio g_HoleSFX = {0};

int main(void){
	if(!InitializeSystems()) goto quit;
	if(!CreateWindow()) goto quit;

	level = CreateLevel(g_LevelPaths[g_CurrentLevel], g_LevelSongPaths[g_CurrentLevel]);
	level->firstInitialized = MTHLIB_FALSE;

	if(!LoadAssets()) goto quit;

	g_GameState = GS_STARTMENU;
	g_GameIsRunning = MTHLIB_TRUE;
	while(g_GameIsRunning){
		u32 frameStart = SDL_GetTicks();
		ProcessInput();
		SimulateWorld();
		RenderGame();
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
	g_Window.window = SDL_CreateWindow(GAME_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	if(g_Window.window == NULL){
		//Error creating window
		fprintf(stderr, "%s\n", SDL_GetError());
		return MTHLIB_FALSE;
	}

	//Setting the window icon
	SDL_Surface* iconSurface = IMG_Load("assets/images/ball.png");
	SDL_SetWindowIcon(g_Window.window, iconSurface);

	//Creating a renderer (with GPU acceleration, we're only using VRAM pixel data, i.e. textures)
	g_Window.renderer = SDL_CreateRenderer(g_Window.window, -1, SDL_RENDERER_ACCELERATED);
	if(g_Window.renderer == NULL){
		//Error creating renderer
		fprintf(stderr, "%s\n", SDL_GetError());
		return MTHLIB_FALSE;
	}
	return MTHLIB_TRUE;
}

b8 InitializeSystems(void){
	//SDL Initialization
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0){
		//Error initializing SDL
		fprintf(stderr, "%s\n", SDL_GetError());
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
		//Error loading text
		fprintf(stderr, IMG_GetError());
		return MTHLIB_FALSE;
	}
	level->hole.texture = IMG_LoadTexture(g_Window.renderer, "assets/images/hole.png");
	if(level->hole.texture == NULL){
		//Error loading texture
		fprintf(stderr, IMG_GetError());
		return MTHLIB_FALSE;
	}
	level->ball.texture = IMG_LoadTexture(g_Window.renderer, "assets/images/ball.png");
	if(level->ball.texture == NULL){
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
	
	// For some reason the pitch sounds different if I use CreateAudio here...
	if(SDL_LoadWAV("assets/sounds/sfx/Hit_Hurt3.wav", &g_CollisionSFX.spec, &g_CollisionSFX.buffer, &g_CollisionSFX.length) == NULL){
		//Error loading wav
		fprintf(stderr, "%s\n", SDL_GetError());
		return MTHLIB_FALSE;
	}
	g_CollisionSFX.deviceID = SDL_OpenAudioDevice(NULL, 0, &level->song.spec, NULL, 0);
	if(g_CollisionSFX.deviceID == 0){
		fprintf(stderr, "%s\n", SDL_GetError());
		return MTHLIB_FALSE;
	}

	if(SDL_LoadWAV("assets/sounds/sfx/Pickup_Coin2.wav", &g_HoleSFX.spec, &g_HoleSFX.buffer, &g_HoleSFX.length) == NULL){
		//Error loading wav
		fprintf(stderr, "%s\n", SDL_GetError());
		return MTHLIB_FALSE;
	}
	g_HoleSFX.deviceID = SDL_OpenAudioDevice(NULL, 0, &level->song.spec, NULL, 0);
	if(g_HoleSFX.deviceID == 0){
		fprintf(stderr, "%s\n", SDL_GetError());
		return MTHLIB_FALSE;
	}
	
	g_LogoTexture = IMG_LoadTexture(g_Window.renderer, "assets/images/logo.png");
	if(g_LogoTexture == NULL){
		//Error loading texture
		fprintf(stderr, IMG_GetError());
		return MTHLIB_FALSE;
	}

	return MTHLIB_TRUE;
}

void QuitGame(void){
	SDL_DestroyTexture(level->hole.texture);
	SDL_DestroyTexture(level->ball.texture);
	SDL_DestroyTexture(g_BrickTile);
	SDL_DestroyTexture(g_BackgroundTile);
	SDL_DestroyTexture(g_Arrow.texture);
	SDL_DestroyTexture(g_PowerBar.backgroundTexture);
	SDL_DestroyTexture(g_PowerBar.foregroundTexture);
	
	SDL_DestroyRenderer(g_Window.renderer);
	SDL_DestroyWindow(g_Window.window);
	
	TTF_CloseFont(g_Font);

	FreeAudio(&level->song);
	FreeAudio(&g_CollisionSFX);

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}