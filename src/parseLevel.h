#pragma once

#include "main.h"

#define MAX_CHAR_NUMBER 5

Level* CreateLevel(const char* levelPath, const char* songLevelPath);
b8 CheckPosition(v2 position);
b8 CheckTileType(u8 tileType);
void FreeLevel(Level* level);