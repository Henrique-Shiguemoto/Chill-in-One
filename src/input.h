#pragma once

#include "rmathlib.h"
#include "rtypes.h"

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
rm_v2f GetMousePosition(void);
