#include "audio.h"

Audio CreateAudio(const char* audioPath){
	Audio audio = {0};
	if(SDL_LoadWAV(audioPath, &audio.spec, &audio.buffer, &audio.length) == NULL){
		//Error loading wav
		fprintf(stderr, "%s\n", SDL_GetError());
		return (Audio){0};
	}
	audio.isLooping = MTHLIB_TRUE;
	return audio;
}

void FreeAudio(Audio* audio){
	SDL_CloseAudioDevice(audio->deviceID);
	audio->length = 0;
	audio->spec = (SDL_AudioSpec){0};
	audio->isLooping = MTHLIB_FALSE;
	SDL_FreeWAV(audio->buffer);
}