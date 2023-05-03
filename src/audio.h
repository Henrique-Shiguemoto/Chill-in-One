#pragma once

#include "main.h"

Audio CreateAudio(const char* audioPath);
void CallbackAudio(void* userdata, Uint8* stream, int len);
void PlayAudio(Audio* audio);
void StopAudio(Audio* audio);
void FreeAudio(Audio* audio);