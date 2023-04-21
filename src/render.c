#include "render.h"
#include "main.h"

extern Window g_Window;
extern SDL_Texture* g_BackgroundTile;
extern SDL_Texture* g_BrickTile;
extern TTF_Font* g_Font;
extern b8 g_ShowDebugInfo;
extern Hole g_Hole;
extern Ball g_Ball;
extern Arrow g_Arrow;
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
						 .w = HOLE_SIZE,
						 .h = HOLE_SIZE};
	SDL_RenderCopy(g_Window.renderer, g_Hole.texture, NULL, &destRect);
}

void RenderBall(void){
	SDL_Rect destRect = {.x = g_Ball.pos.x,
						 .y = g_Ball.pos.y,
						 .w = BALL_SIZE,
						 .h = BALL_SIZE};
	SDL_RenderCopy(g_Window.renderer, g_Ball.texture, NULL, &destRect);
}

void RenderArrow(void){
	// ***Magic numbers***
	SDL_Point pt = {32, 64 + 0.5*BALL_SIZE};
	SDL_Rect destRect = {.x = g_Ball.pos.x - 32 + 0.5*BALL_SIZE,
						 .y = g_Ball.pos.y - 64,
						 .w = 64,
						 .h = 64};
	SDL_RenderCopyEx(g_Window.renderer, g_Arrow.texture, NULL, &destRect, (f64)g_Arrow.angle, &pt, SDL_FLIP_NONE);
}

void RenderUI(void){ }

void RenderDebugInfo(void){
	if(g_ShowDebugInfo == MTHLIB_TRUE){
		char debugString[200];
		snprintf(debugString, array_count(debugString), "Ball's Velocity = (%.4f, %.4f)", g_Ball.vel.x, g_Ball.vel.y);
		RenderString(debugString, (v2){25, 25}, (v2){400, 20});
		snprintf(debugString, array_count(debugString), "Ball is Moving = %s", g_Ball.isMoving ? "TRUE" : "FALSE");
		RenderString(debugString, (v2){25, 55}, (v2){300, 20});
	}
}

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
	RenderArrow();
	RenderDebugInfo();
	SDL_RenderPresent(g_Window.renderer);
}