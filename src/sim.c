#include "sim.h"
#include "main.h"
#include "render.h"

extern Ball g_Ball;
extern Hole g_Hole;
extern b8 g_TileMap[WINDOW_HEIGHT/64][WINDOW_WIDTH/64];

void SimulateWorld(void){

	//Checking collisions with brick tiles
	v2 newBallPosX = (v2){g_Ball.pos.x + g_Ball.vel.x * DESIRED_DELTA, g_Ball.pos.y};
	v2 newBallPosY = (v2){g_Ball.pos.x, g_Ball.pos.y + g_Ball.vel.y * DESIRED_DELTA};

	b8 alreadyCollided = MTHLIB_FALSE;
	for(i32 i = 0; i < WINDOW_HEIGHT/64; i++){
		for(i32 j = 0; j < WINDOW_WIDTH/64; j++){
			if(g_TileMap[i][j] == BRICK_TILE){
				AABB2D brick = (AABB2D){.min = {j * BRICK_SIZE, i * BRICK_SIZE}, .max = {((j + 1) * BRICK_SIZE) - 1, ((i + 1) * BRICK_SIZE) - 1}};

				//Checking collision on X axis
				AABB2D ballX = (AABB2D){.min = newBallPosX, .max = {newBallPosX.x + BALL_SIZE, newBallPosX.y + BALL_SIZE}};
				if(CollisionAABB2D(ballX, brick)){
					if(ballX.min.x < brick.max.x || ballX.max.x > brick.min.x){
						g_Ball.vel.x *= -1;
					}
					alreadyCollided = MTHLIB_TRUE;
				}

				//Checking collision on Y axis
				AABB2D ballY = (AABB2D){.min = newBallPosY, .max = {newBallPosY.x + BALL_SIZE, newBallPosY.y + BALL_SIZE}};
				if(CollisionAABB2D(ballY, brick)){
					if(ballY.min.y < brick.max.y || ballY.max.y > brick.min.y){
						g_Ball.vel.y *= -1;
					}
					alreadyCollided = MTHLIB_TRUE;
				}
			}
			if(alreadyCollided){
				goto BallPositionUpdate;
			}
		}
	}

BallPositionUpdate:
	//Update ball's position
	g_Ball.pos = (v2){g_Ball.pos.x + g_Ball.vel.x * DESIRED_DELTA, g_Ball.pos.y + g_Ball.vel.y * DESIRED_DELTA};

	//Update ball's velocity
	g_Ball.vel = ScaleV2(g_Ball.vel, 0.92f);
}