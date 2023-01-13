#include "sim.h"
#include "main.h"
#include "render.h"

extern Ball g_Ball;
extern Hole g_Hole;
extern b8 g_TileMap[WINDOW_HEIGHT/64][WINDOW_WIDTH/64];

void SimulateWorld(void){
	f32 velScale = 0.5f;

	f32 newTopPosX = g_Ball.pos.x + g_Ball.vel.x * velScale;
	f32 newTopPosY = g_Ball.pos.y + g_Ball.vel.y * velScale;

	//Check for collisions between ball circle and neighboring brick tiles
	i32 newTileX = (newTopPosX + 8) / 64;
	i32 newTileY = (newTopPosY + 8) / 64;

	//The neighbors will be (newTileX - 1, newTileY - 1), (newTileX, newTileY - 1), (newTileX + 1, newTileY - 1), 
	//						(newTileX - 1, newTileY)							  , (newTileX + 1, newTileY), 
	//						(newTileX - 1, newTileY + 1), (newTileX, newTileY + 1), (newTileX + 1, newTileY + 1)
	
	//Calculating ball's circle
	sphere2D ballSphere = (sphere2D){.center = {newTopPosX + 8, newTopPosY + 8}, .radius = 8.0f};
	
	//Calculating AABBs of neighbors
	AABB2D tileAABB0 = (AABB2D){.min = {(newTileX - 1) * 64, (newTileY - 1) * 64}, 		.max = {((newTileX - 1) * 64) + 64, ((newTileY - 1) * 64) + 64}};
	AABB2D tileAABB1 = (AABB2D){.min = {(newTileX) 	   * 64, (newTileY - 1) * 64}, 	   	.max = {((newTileX) 	* 64) + 64, ((newTileY - 1) * 64) + 64}};
	AABB2D tileAABB2 = (AABB2D){.min = {(newTileX + 1) * 64, (newTileY - 1) * 64}, 	   	.max = {((newTileX + 1) * 64) + 64, ((newTileY - 1) * 64) + 64}};
	AABB2D tileAABB3 = (AABB2D){.min = {(newTileX - 1) * 64, (newTileY)     * 64},     	.max = {((newTileX - 1) * 64) + 64, ((newTileY) 	* 64) + 64}};
	AABB2D tileAABB4 = (AABB2D){.min = {(newTileX + 1) * 64, (newTileY)     * 64}, 	   	.max = {((newTileX + 1) * 64) + 64, ((newTileY) 	* 64) + 64}};
	AABB2D tileAABB5 = (AABB2D){.min = {(newTileX - 1) * 64, (newTileY + 1) * 64}, 		.max = {((newTileX - 1) * 64) + 64, ((newTileY + 1) * 64) + 64}};
	AABB2D tileAABB6 = (AABB2D){.min = {(newTileX)     * 64, (newTileY + 1) * 64}, 	   	.max = {((newTileX) 	* 64) + 64, ((newTileY + 1) * 64) + 64}};
	AABB2D tileAABB7 = (AABB2D){.min = {(newTileX + 1) * 64, (newTileY + 1) * 64}, 		.max = {((newTileX + 1) * 64) + 64, ((newTileY + 1) * 64) + 64}};

	//We'll verify if they're brick tiles
	//Checking collisions with neighbor brick tiles
	//Updating ball's velocity
	if(g_TileMap[newTileY - 1][newTileX - 1] == BRICK_TILE && CollisionSphere2DAndAABB2D(tileAABB0, ballSphere)){
		point2D nearestPoint = (point2D){tileAABB0.min.x, tileAABB0.max.y};
		v2 newVel = UnitV2(SubtractV2(ballSphere.center, nearestPoint));
		g_Ball.vel.x = newVel.x;
		g_Ball.vel.y = newVel.y;
	}
	if(g_TileMap[newTileY - 1][newTileX] == BRICK_TILE && CollisionSphere2DAndAABB2D(tileAABB1, ballSphere)){
		g_Ball.vel.y *= -1;
	}
	if(g_TileMap[newTileY - 1][newTileX + 1] == BRICK_TILE && CollisionSphere2DAndAABB2D(tileAABB2, ballSphere)){
		point2D nearestPoint = (point2D){tileAABB2.min.x, tileAABB2.max.y};
		v2 newVel = UnitV2(SubtractV2(ballSphere.center, nearestPoint));
		g_Ball.vel.x = newVel.x;
		g_Ball.vel.y = newVel.y;
	}
	if(g_TileMap[newTileY][newTileX - 1] == BRICK_TILE && CollisionSphere2DAndAABB2D(tileAABB3, ballSphere)){
		g_Ball.vel.x *= -1;
	}
	if(g_TileMap[newTileY][newTileX + 1] == BRICK_TILE && CollisionSphere2DAndAABB2D(tileAABB4, ballSphere)){
		g_Ball.vel.x *= -1;
	}
	if(g_TileMap[newTileY + 1][newTileX - 1] == BRICK_TILE && CollisionSphere2DAndAABB2D(tileAABB5, ballSphere)){
		point2D nearestPoint = (point2D){tileAABB5.min.x, tileAABB5.max.y};
		v2 newVel = UnitV2(SubtractV2(ballSphere.center, nearestPoint));
		g_Ball.vel.x = newVel.x;
		g_Ball.vel.y = newVel.y;
	}
	if(g_TileMap[newTileY + 1][newTileX] == BRICK_TILE && CollisionSphere2DAndAABB2D(tileAABB6, ballSphere)){
		g_Ball.vel.y *= -1;
	}
	if(g_TileMap[newTileY + 1][newTileX + 1] == BRICK_TILE && CollisionSphere2DAndAABB2D(tileAABB7, ballSphere)){
		point2D nearestPoint = (point2D){tileAABB7.min.x, tileAABB7.max.y};
		v2 newVel = UnitV2(SubtractV2(ballSphere.center, nearestPoint));
		g_Ball.vel.x = newVel.x;
		g_Ball.vel.y = newVel.y;
	}

	//Check collisions between ball and hole (circle collision)
	sphere2D holeSphere = {.center = {g_Hole.pos.x + 24.0f, g_Hole.pos.y + 24.0f}, .radius = 24.0f};
	if(CollisionSphere2D(ballSphere, holeSphere)){
		velScale = 0.0f;
		printf("You won!\n");
	}

	//Update ball's position
	g_Ball.pos.x += g_Ball.vel.x * velScale;
	g_Ball.pos.y += g_Ball.vel.y * velScale;
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