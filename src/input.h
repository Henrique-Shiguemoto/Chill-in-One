#pragma once

#include "mthlib.h"

typedef struct Input{
	b8 debugKeyIsDown;
	b8 closeKeyIsDown;

	b8 debugKeyWasDown;
	b8 closeKeyWasDown;
} Input;

void ProcessInput(void);
void ProcessInputStartMenu(void);
void ProcessInputEndMenu(void);
void ProcessInputLevel(void);
void UpdateArrowAngle(void);
void UpdatePowerBar(void);
v2 GetMousePosition(void);