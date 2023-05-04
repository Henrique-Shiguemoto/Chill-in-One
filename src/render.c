#include "render.h"
#include "main.h"

extern Window g_Window;
extern SDL_Texture* g_BackgroundTile;
extern SDL_Texture* g_BrickTile;
extern SDL_Texture* g_LogoTexture;
extern TTF_Font* g_Font;
extern b8 g_ShowDebugInfo;
extern Arrow g_Arrow;
extern PowerBar g_PowerBar;
extern i32 g_StrokeCounter;
extern Level* level;
extern GAME_STATE g_GameState;

void RenderTilemap(void){
	for(i32 y = 0; y < WINDOW_HEIGHT / BRICK_SIZE; y++){
		for(i32 x = 0; x < WINDOW_WIDTH / BRICK_SIZE; x++){
			//Drawing corresponding tile (either background tile or brick tile)
			SDL_Rect destRect = {x * BRICK_SIZE, y * BRICK_SIZE, BRICK_SIZE, BRICK_SIZE};
			SDL_Texture* texture = NULL;
			if(level->tilemap[y][x] == BACKGROUND_TILE){
				texture = g_BackgroundTile;
			}else{
				texture = g_BrickTile;
			}
			SDL_RenderCopy(g_Window.renderer, texture, NULL, &destRect);
		}
	}
}

void RenderHole(void){
	SDL_Rect destRect = {.x = level->hole.pos.x,
						 .y = level->hole.pos.y,
						 .w = HOLE_SIZE,
						 .h = HOLE_SIZE};
	SDL_RenderCopy(g_Window.renderer, level->hole.texture, NULL, &destRect);
}

void RenderBall(void){
	SDL_Rect destRect = {.x = level->ball.pos.x,
						 .y = level->ball.pos.y,
						 .w = BALL_SIZE,
						 .h = BALL_SIZE};
	SDL_RenderCopy(g_Window.renderer, level->ball.texture, NULL, &destRect);
}

void RenderArrow(void){
	// ***Magic numbers***
	SDL_Point pt = {32, 64 + 0.5*BALL_SIZE};
	SDL_Rect destRect = {.x = level->ball.pos.x - 32 + 0.5*BALL_SIZE,
						 .y = level->ball.pos.y - 64,
						 .w = g_Arrow.width,
						 .h = g_Arrow.height};
	SDL_RenderCopyEx(g_Window.renderer, g_Arrow.texture, NULL, &destRect, (f64)g_Arrow.angle, &pt, SDL_FLIP_NONE);
}

void RenderUI(void){ 
	//stroke counter
	char uiString[64];
	snprintf(uiString, array_count(uiString), "Stroke Counter = %i", g_StrokeCounter);
	RenderString(uiString, (v2){0.5*WINDOW_WIDTH - 150, 595}, (v2){300, 30});
	
	//power meter background
	SDL_Rect destRectBackground = {.x = WINDOW_WIDTH - 88 - 0.5*(BRICK_SIZE - 48),
						 			.y = BRICK_SIZE / 8,
						 			.w = 128,
						 			.h = 128};
	SDL_RenderCopy(g_Window.renderer, g_PowerBar.backgroundTexture, NULL, &destRectBackground);

	//power meter foreground
	SDL_Rect destRectForeground = {.x = WINDOW_WIDTH - 88 - 0.5*(BRICK_SIZE - 48) + 1,
						 			.y = BRICK_SIZE / 8,
						 			.w = 128,
						 			.h = (g_PowerBar.currentPower / MAX_VEL_NORM) * 128};
	SDL_RenderCopy(g_Window.renderer, g_PowerBar.foregroundTexture, NULL, &destRectForeground);


}

void RenderDebugInfo(void){
	if(g_ShowDebugInfo == MTHLIB_TRUE){
		char debugString[200];
		snprintf(debugString, array_count(debugString), "Ball's Velocity = (%.4f, %.4f)", level->ball.vel.x, level->ball.vel.y);
		RenderString(debugString, (v2){25, 25}, (v2){400, 20});
		snprintf(debugString, array_count(debugString), "Ball is Moving = %s", level->ball.isMoving ? "TRUE" : "FALSE");
		RenderString(debugString, (v2){25, 45}, (v2){300, 20});
		snprintf(debugString, array_count(debugString), "Stroke Power = %f", g_PowerBar.currentPower);
		RenderString(debugString, (v2){25, 65}, (v2){300, 20});
		snprintf(debugString, array_count(debugString), "Level FI = %s", level->firstInitialized ? "TRUE" : "FALSE");
		RenderString(debugString, (v2){25, 85}, (v2){200, 20});
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

void RenderStartMenu(void){
	SDL_RenderClear(g_Window.renderer);
	RenderTilemap();
	SDL_Rect destRect = {.x = 0.5 * WINDOW_WIDTH - 0.5 * LOGO_WIDTH,
						 .y = 0.5 * WINDOW_HEIGHT - 0.5 * LOGO_HEIGHT,
						 .w = LOGO_WIDTH,
						 .h = LOGO_HEIGHT};
	SDL_RenderCopy(g_Window.renderer, g_LogoTexture, NULL, &destRect);
	RenderString("Start Game", (v2){0.5 * WINDOW_WIDTH - (225 / 2), destRect.y + 160}, (v2){225, 25});
	RenderString("Quit", (v2){0.5 * WINDOW_WIDTH - (75 / 2), destRect.y + 230}, (v2){75, 25});
	SDL_RenderPresent(g_Window.renderer);
}

void RenderLevel(void){
	SDL_RenderClear(g_Window.renderer);
	RenderTilemap();
	RenderHole();
	RenderBall();
	RenderUI();
	if(!level->ball.isMoving) {
		RenderArrow();
	}
	RenderDebugInfo();
	SDL_RenderPresent(g_Window.renderer);
}

void RenderEndMenu(void){
	SDL_RenderClear(g_Window.renderer);
	RenderTilemap();
	RenderString("You Finished!", (v2){0.5*WINDOW_WIDTH - (400 / 2), 0.5*WINDOW_HEIGHT - (55/2)}, (v2){400, 55});
	RenderString("Press Enter to Restart", (v2){75, WINDOW_HEIGHT - BRICK_SIZE - 70}, (v2){400, 25});
	RenderString("Press Esc to Quit", (v2){75, WINDOW_HEIGHT - BRICK_SIZE - 35}, (v2){300, 25});
	SDL_RenderPresent(g_Window.renderer);
}

void RenderGame(void){
	//either start render the menu, a level or the end menu
	switch(g_GameState){
		case GS_STARTMENU:{
			RenderStartMenu();
			break;
		}
		case GS_LEVEL:{
			RenderLevel();
			break;
		}
		case GS_ENDMENU:{
			RenderEndMenu();
			break;
		}
		default:{
			fprintf(stderr, "Invalid Game State\n");
			break;
		}
	}
}