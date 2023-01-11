#include "render.h"
#include "main.h"

extern Window g_Window;
extern SDL_Texture* g_BackgroundTile;
extern SDL_Texture* g_BrickTile;
extern Hole g_Hole;
extern Ball g_Ball;
extern b8 g_TileMap[WINDOW_HEIGHT/64][WINDOW_WIDTH/64];

void RenderBackgroundAndWalls(void){
	//Background and brick tiles have the same width and height, 
	//		so we only need two attributes instead of four
	int width = 0;
	int height = 0;
	SDL_QueryTexture(g_BackgroundTile, NULL, NULL, &width, &height);
	for(int y = 0; y < WINDOW_HEIGHT / height; y++){
		for(int x = 0; x < WINDOW_WIDTH / width; x++){
			//Drawing corresponding tile (either background tile or brick tile)
			SDL_Rect destRect = {x * width, y * height, width, height};
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

void RenderGraphics(void){
	SDL_RenderClear(g_Window.renderer);
	RenderBackgroundAndWalls();
	RenderHole();
	RenderBall();
	SDL_RenderPresent(g_Window.renderer);
}