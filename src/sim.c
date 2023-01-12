#include "sim.h"
#include "main.h"

extern Ball g_Ball;
extern b8 g_TileMap[WINDOW_HEIGHT/64][WINDOW_WIDTH/64];

void SimulateWorld(void){
	f32 velScale = 0.0f;

	//Update ball's position
	g_Ball.pos.x -= g_Ball.vel.x * velScale;
	g_Ball.pos.y -= g_Ball.vel.y * velScale;

	//Check collisions between ball and walls

	//Check collisions between ball and hole
	
}