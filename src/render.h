#pragma once

#include "rmathlib.h"

void RenderTilemap(void);
void RenderHole(void);
void RenderBall(void);
void RenderArrow(void);
void RenderUI(void);
void RenderDebugInfo(void);
void RenderString(const char* text, rm_v2f pos, rm_v2f size);
void RenderGame(void);
void RenderStartMenu(void);
void RenderLevel(void);
void RenderEndMenu(void);