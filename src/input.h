#pragma once

#include "mthlib.h"

typedef struct Input{
	b8 debugKeyIsDown;
	b8 closeKeyIsDown;

	b8 debugKeyWasDown;
	b8 closeKeyWasDown;
} Input;

void ProcessInput(void);
void UpdateWasKeys(void);