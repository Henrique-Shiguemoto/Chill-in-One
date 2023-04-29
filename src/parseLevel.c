#include "parseLevel.h"
#include <stdlib.h>

extern Level* level1;

Level* CreateLevel(const char* levelPath, const char* songLevelPath){
	FILE* levelConfigFile = fopen(levelPath, "r");
	if(levelConfigFile == NULL){
		fprintf(stderr, "Error: couldn't find file path\n");
		return NULL;
	}

	Level* level = calloc(1, sizeof(Level));
	i8 buffer[MAX_CHAR_NUMBER];
	f32 posX;
	f32 posY;

	//"reading" ball=
	fseek(levelConfigFile, 5, SEEK_CUR);
	
	//Parsing the first position for ball
	for(i32 i = 0; i < MAX_CHAR_NUMBER; i++){
		u8 currentCharacter = fgetc(levelConfigFile);
		if(currentCharacter == ' '){
			buffer[i] = '\0';
			break;
		}
		buffer[i] = currentCharacter;
	}
	posX = atoi(buffer);

	//Parsing the second position for ball
	for(i32 i = 0; i < MAX_CHAR_NUMBER; i++){
		u8 currentCharacter = fgetc(levelConfigFile);
		if(currentCharacter == '\n'){
			buffer[i] = '\0';
			break;
		}
		buffer[i] = currentCharacter;
	}
	posY = atoi(buffer);

	if(!CheckPosition((v2){posX, posY})){
		fprintf(stderr, "Error: Input Ball Positions are invalid\n");
		return NULL;
	}

	//assign converted strings to ball's position
	level->ball.pos = (v2){posX, posY};
	level->ball.vel = (v2){0};
	level->ball.isMoving = MTHLIB_FALSE;

	//"reading" hole=
	fseek(levelConfigFile, 5, SEEK_CUR);

	//Parsing the first position for hole
	for(i32 i = 0; i < MAX_CHAR_NUMBER; i++){
		u8 currentCharacter = fgetc(levelConfigFile);
		if(currentCharacter == ' '){
			buffer[i] = '\0';
			break;
		}
		buffer[i] = currentCharacter;
	}
	posX = atoi((const i8*)buffer);

	//Parsing the second position for hole
	for(i32 i = 0; i < MAX_CHAR_NUMBER; i++){
		u8 currentCharacter = fgetc(levelConfigFile);
		if(currentCharacter == '\n'){
			buffer[i] = '\0';
			break;
		}
		buffer[i] = currentCharacter;
	}
	posY = atoi(buffer);

	if(!CheckPosition((v2){posX, posY})){
		fprintf(stderr, "Error: Input Hole Positions are invalid\n");
		return NULL;
	}

	//assign converted strings to hole's position
	level->hole.pos = (v2){posX, posY};

	//"reading" tilemap=
	fseek(levelConfigFile, 8, SEEK_CUR);
	for (i32 i = 0; i < WINDOW_HEIGHT/BRICK_SIZE; ++i){
		for (i32 j = 0; j < WINDOW_WIDTH/BRICK_SIZE; ++j){
			u8 currentCharacter = fgetc(levelConfigFile);
			while(!CheckTileType(currentCharacter - '0')){
				currentCharacter = fgetc(levelConfigFile);
			}
			// printf("tile(%d, %d) = %s\n", i, j, currentCharacter == '0' ? "BACKGROUND" : "BRICK");
			level->tilemap[i][j] = currentCharacter - '0';
		}
	}

	//Loading level song
	if(SDL_LoadWAV(songLevelPath, &level->song.spec, &level->song.buffer, &level->song.length) == NULL){
		//Error loading wav
		fprintf(stderr, "%s\n", SDL_GetError());
		return NULL;
	}

	fclose(levelConfigFile);
	return level;
}

b8 CheckPosition(v2 position){
	if(position.x < 0 || position.x > WINDOW_WIDTH ||
		position.y < 0 || position.y > WINDOW_HEIGHT){
		return MTHLIB_FALSE;
	}
	return MTHLIB_TRUE;
}

b8 CheckTileType(u8 tileType){
	if(tileType != BRICK_TILE && tileType != BACKGROUND_TILE){
		return MTHLIB_FALSE;
	}
	return MTHLIB_TRUE;
}

void FreeLevel(Level* level){
	free(level);
}