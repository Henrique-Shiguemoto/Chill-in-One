#include "render.h"
#include "main.h"

extern Window g_Window;
extern SDL_Texture* g_BackgroundTile;
extern SDL_Texture* g_BrickTile;
extern TTF_Font* g_Font;
extern Hole g_Hole;
extern Ball g_Ball;
extern b8 g_TileMap[WINDOW_HEIGHT/64][WINDOW_WIDTH/64];

void RenderBackgroundAndWalls(void){
	for(int y = 0; y < WINDOW_HEIGHT / 64; y++){
		for(int x = 0; x < WINDOW_WIDTH / 64; x++){
			//Drawing corresponding tile (either background tile or brick tile)
			SDL_Rect destRect = {x * 64, y * 64, 64, 64};
			SDL_Texture* texture = NULL;
			if(g_TileMap[y][x] == BACKGROUND_TILE){
				texture = g_BackgroundTile;
			}else{
				texture = g_BrickTile;
			}
			SDL_RenderCopy(g_Window.renderer, texture, NULL, &destRect);
		}
	}
}

void RenderHole(void){
	SDL_Rect destRect = {.x = g_Hole.pos.x,
						 .y = g_Hole.pos.y,
						 .w = 48,
						 .h = 48};
	SDL_RenderCopy(g_Window.renderer, g_Hole.texture, NULL, &destRect);
}

void RenderBall(void){
	SDL_Rect destRect = {.x = g_Ball.pos.x,
						 .y = g_Ball.pos.y,
						 .w = 16,
						 .h = 16};
	SDL_RenderCopy(g_Window.renderer, g_Ball.texture, NULL, &destRect);
}

void RenderUI(void){
	
}

#ifdef DEBUG
void RenderDebugInfo(void){
	char debugString[200] = "Hello";
	snprintf(debugString, array_count(debugString), "Ball's Velocity = (%f, %f)", g_Ball.vel.x, g_Ball.vel.y);
	RenderString(debugString, (v2){25, 25}, (v2){700, 40});
}
#else
void RenderDebugInfo(void){}
#endif

void RenderString(const char* text, v2 pos, v2 size){
	SDL_Surface* fontSurface = TTF_RenderText_Solid(g_Font, text, (SDL_Color){0, 0, 0, 0});
	SDL_Texture* fontTexture = SDL_CreateTextureFromSurface(g_Window.renderer, fontSurface);
	SDL_FreeSurface(fontSurface);
	SDL_Rect destRect = {(i32)pos.x, (i32)pos.y, (i32)size.x, (i32)size.y};
	SDL_RenderCopy(g_Window.renderer, fontTexture, NULL, &destRect);
	SDL_DestroyTexture(fontTexture);
}

void RenderGraphics(void){
	SDL_RenderClear(g_Window.renderer);
	RenderBackgroundAndWalls();
	RenderHole();
	RenderBall();
	// RenderUI();
	// RenderDebugInfo();
	SDL_RenderPresent(g_Window.renderer);
}