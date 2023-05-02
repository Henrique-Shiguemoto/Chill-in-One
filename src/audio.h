#pragma once

#include "main.h"

Audio CreateAudio(const char* audioPath);
void FreeAudio(Audio* audio);