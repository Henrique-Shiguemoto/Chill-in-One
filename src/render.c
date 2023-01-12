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
			if(g_TileMap[y][x] == 0){
				texture = g_BackgroundTile;
			}else{
				texture = g_BrickTile;
			}
			SDL_RenderCopy(g_Window.renderer, texture, NULL, &destRect);
		}
	}
}

void RenderHole(void){
	SDL_Rect destRect = {.x = g_Hole.pos.x * 64 + 32 - 24,
						 .y = g_Hole.pos.y * 64 + 32 - 24,
						 .w = 48,
						 .h = 48};
	SDL_RenderCopy(g_Window.renderer, g_Hole.texture, NULL, &destRect);
}

void RenderBall(void){
	SDL_Rect destRect = {.x = g_Ball.pos.x * 64 + 32 - 8,
						 .y = g_Ball.pos.y * 64 + 32 - 8,
						 .w = 16,
						 .h = 16};
	SDL_RenderCopy(g_Window.renderer, g_Ball.texture, NULL, &destRect);
}

void RenderUI(void){
	SDL_Surface* fontSurface = TTF_RenderText_Solid(g_Font, "Test Text", (SDL_Color){0, 0, 0, 0});
	SDL_Texture* fontTexture = SDL_CreateTextureFromSurface(g_Window.renderer, fontSurface);
	SDL_FreeSurface(fontSurface);
	SDL_Rect destRect = {10, 10, 300, 50};
	SDL_RenderCopy(g_Window.renderer, fontTexture, NULL, &destRect);
	SDL_DestroyTexture(fontTexture);
}

void RenderGraphics(void){
	SDL_RenderClear(g_Window.renderer);
	RenderBackgroundAndWalls();
	RenderHole();
	RenderBall();
	//RenderUI();
	SDL_RenderPresent(g_Window.renderer);
}