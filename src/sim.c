#include "sim.h"
#include "main.h"
#include "render.h"

extern Level* level;
extern b8 g_GameIsRunning;
extern Audio g_CollisionSFX;

void SimulateWorld(void){

	//Checking collisions with brick tiles
	v2 newBallPosX = (v2){level->ball.pos.x + level->ball.vel.x * DESIRED_DELTA, level->ball.pos.y};
	v2 newBallPosY = (v2){level->ball.pos.x, level->ball.pos.y + level->ball.vel.y * DESIRED_DELTA};

	b8 alreadyCollided = MTHLIB_FALSE;
	for(i32 i = 0; i < WINDOW_HEIGHT/64; i++){
		for(i32 j = 0; j < WINDOW_WIDTH/64; j++){
			if(level->tilemap[i][j] == BRICK_TILE){
				AABB2D brick = (AABB2D){.min = {j * BRICK_SIZE, i * BRICK_SIZE}, .max = {((j + 1) * BRICK_SIZE) - 1, ((i + 1) * BRICK_SIZE) - 1}};

				//Checking collision on X axis
				AABB2D ballX = (AABB2D){.min = newBallPosX, .max = {newBallPosX.x + BALL_SIZE, newBallPosX.y + BALL_SIZE}};
				if(CollisionAABB2D(ballX, brick)){
					if(ballX.min.x < brick.max.x || ballX.max.x > brick.min.x){
						level->ball.vel.x *= -1;
					}
					alreadyCollided = MTHLIB_TRUE;
					
					int status = SDL_QueueAudio(g_CollisionSFX.deviceID, g_CollisionSFX.buffer, g_CollisionSFX.length);
					if(status < 0){
						fprintf(stderr, SDL_GetError());
					}
					SDL_PauseAudioDevice(g_CollisionSFX.deviceID, 0);
				}

				//Checking collision on Y axis
				AABB2D ballY = (AABB2D){.min = newBallPosY, .max = {newBallPosY.x + BALL_SIZE, newBallPosY.y + BALL_SIZE}};
				if(CollisionAABB2D(ballY, brick)){
					if(ballY.min.y < brick.max.y || ballY.max.y > brick.min.y){
						level->ball.vel.y *= -1;
					}
					alreadyCollided = MTHLIB_TRUE;
					
					int status = SDL_QueueAudio(g_CollisionSFX.deviceID, g_CollisionSFX.buffer, g_CollisionSFX.length);
					if(status < 0){
						fprintf(stderr, SDL_GetError());
					}
					SDL_PauseAudioDevice(g_CollisionSFX.deviceID, 0);
				}
			}
			if(alreadyCollided){
				goto OutOfBrickCollisionDetection;
			}
		}
	}

OutOfBrickCollisionDetection:
	//Update ball's position
	level->ball.pos = (v2){level->ball.pos.x + level->ball.vel.x * DESIRED_DELTA, level->ball.pos.y + level->ball.vel.y * DESIRED_DELTA};

	//Update ball's velocity
	level->ball.vel = ScaleV2(level->ball.vel, 0.97f);

	//Check collision with hole
	v2 ballAABBOffset = (v2){BALL_SIZE, BALL_SIZE};
	v2 holeCircleOffset = (v2){0.5*HOLE_SIZE, 0.5*HOLE_SIZE};
	AABB2D ballAABB = (AABB2D){.min = level->ball.pos, .max = AddV2(level->ball.pos, ballAABBOffset)};
	sphere2D holeCircle = (sphere2D){.center = AddV2(level->hole.pos, holeCircleOffset), .radius = 0.35*HOLE_SIZE};
	if(CollisionSphere2DAndAABB2D(ballAABB, holeCircle)){
		//Ball and hole collided
		printf("You Won!\n");
		g_GameIsRunning = MTHLIB_FALSE;
	}

	// Checking to see if the ball is currently moving or not
	if(NormV2(level->ball.vel) <= 0.01f){
		level->ball.isMoving = MTHLIB_FALSE;
		level->ball.vel = (v2){0};
	}
}

b8 CollisionSphere2DAndAABB2D(AABB2D aabb, sphere2D s){
	//Calculating the normal vector of the side of the brick tile we are going into (javidx9)
	point2D nearestPoint = (v2){0};
	nearestPoint.x = Clamp32(aabb.min.x, aabb.max.x, s.center.x);
	nearestPoint.y = Clamp32(aabb.min.y, aabb.max.y, s.center.y);
	f32 d = DistanceBetweenPoints2D(s.center, nearestPoint);
	if(d <= s.radius){
		return MTHLIB_TRUE;
	}
	return MTHLIB_FALSE;
}