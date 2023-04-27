#pragma once

#include "main.h"

Level* ParseLevel(const char* levelConfigFile);
b8 CheckPosition(v2 position);
b8 CheckTileType(i8 tileType);