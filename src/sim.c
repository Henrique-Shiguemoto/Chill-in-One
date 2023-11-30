#include "sim.h"
#include "main.h"
#include "render.h"
#include "audio.h"
#include "parseLevel.h"

extern Level* level;
extern b8 g_GameIsRunning;
extern GAME_STATE g_GameState;
extern Audio g_CollisionSFX;
extern Audio g_HoleSFX;
extern const char* g_LevelPaths[];
extern const char* g_LevelSongPaths[];
extern i32 g_CurrentLevel;
extern i32 g_StrokeCounter;

void SimulateWorld(void){
	if(g_GameState != GS_LEVEL){
		return;
	}

	if(level->firstInitialized){
		//if the level was just created, we start playing the level song
		PlayAudio(&level->song);
		level->firstInitialized = RMATH_FALSE;
	}

	//Checking collisions with brick tiles
	rm_v2f newBallPosX = (rm_v2f){level->ball.pos.x + level->ball.vel.x * DESIRED_DELTA, level->ball.pos.y};
	rm_v2f newBallPosY = (rm_v2f){level->ball.pos.x, level->ball.pos.y + level->ball.vel.y * DESIRED_DELTA};

	b8 alreadyCollided = RMATH_FALSE;
	for(i32 i = 0; i < WINDOW_HEIGHT/64; i++){
		for(i32 j = 0; j < WINDOW_WIDTH/64; j++){
			if(level->tilemap[i][j] == BRICK_TILE){
				rm_AABB2D brick = (rm_AABB2D){.min = {j * BRICK_SIZE, i * BRICK_SIZE}, .max = {((j + 1) * BRICK_SIZE) - 1, ((i + 1) * BRICK_SIZE) - 1}};

				//Checking collision on X axis
				rm_AABB2D ballX = (rm_AABB2D){.min = newBallPosX, .max = {newBallPosX.x + BALL_SIZE, newBallPosX.y + BALL_SIZE}};
				if(rm_collision_AABB2D(ballX, brick)){
					if(ballX.min.x < brick.max.x || ballX.max.x > brick.min.x){
						level->ball.vel.x *= -1;
					}
					alreadyCollided = RMATH_TRUE;
					PlayAudio(&g_CollisionSFX);
				}

				//Checking collision on Y axis
				rm_AABB2D ballY = (rm_AABB2D){.min = newBallPosY, .max = {newBallPosY.x + BALL_SIZE, newBallPosY.y + BALL_SIZE}};
				if(rm_collision_AABB2D(ballY, brick)){
					if(ballY.min.y < brick.max.y || ballY.max.y > brick.min.y){
						level->ball.vel.y *= -1;
					}
					alreadyCollided = RMATH_TRUE;
					PlayAudio(&g_CollisionSFX);
				}
			}
			if(alreadyCollided){
				goto OutOfBrickCollisionDetection;
			}
		}
	}

OutOfBrickCollisionDetection:
	//Update ball's position
	level->ball.pos = (rm_v2f){level->ball.pos.x + level->ball.vel.x * DESIRED_DELTA, level->ball.pos.y + level->ball.vel.y * DESIRED_DELTA};

	//Update ball's velocity
	level->ball.vel = rm_scale_v2f(level->ball.vel, 0.97f);

	//Check collision with hole
	rm_v2f ballAABBOffset = (rm_v2f){BALL_SIZE, BALL_SIZE};
	rm_v2f holeCircleOffset = (rm_v2f){0.5*HOLE_SIZE, 0.5*HOLE_SIZE};
	rm_AABB2D ballAABB = (rm_AABB2D){.min = level->ball.pos, .max = rm_add_v2f(level->ball.pos, ballAABBOffset)};
	rm_circle holeCircle = (rm_circle){.center = rm_add_v2f(level->hole.pos, holeCircleOffset), .radius = 0.35*HOLE_SIZE};
	if(rm_collision_circle_AABB2D(ballAABB, holeCircle)){
		PlayAudio(&g_HoleSFX);
		StopAudio(&level->song);
		g_StrokeCounter = 0;
		g_CurrentLevel++;
		if(g_CurrentLevel < LEVEL_COUNT){
			level = CreateLevel(g_LevelPaths[g_CurrentLevel], g_LevelSongPaths[g_CurrentLevel]);
		}else{
			g_GameState = GS_ENDMENU;
		}
		return;
	}

	// Checking to see if the ball is currently moving or not
	if(rm_mag_v2f(level->ball.vel) <= 0.01f){
		level->ball.isMoving = RMATH_FALSE;
		level->ball.vel = (rm_v2f){0};
	}
}

b8 rm_collision_circle_AABB2D(rm_AABB2D aabb, rm_circle s){
	//Calculating the normal vector of the side of the brick tile we are going into (javidx9)
	rm_point2D nearestPoint = (rm_point2D){0};
	nearestPoint.x = rm_clamp32(aabb.min.x, aabb.max.x, s.center.x);
	nearestPoint.y = rm_clamp32(aabb.min.y, aabb.max.y, s.center.y);
	f32 d = rm_distance_points2D(s.center, nearestPoint);
	if(d <= s.radius){
		return RMATH_TRUE;
	}
	return RMATH_FALSE;
}