#include "audio.h"

Audio CreateAudio(const char* audioPath){
	Audio audio = {0};
	if(SDL_LoadWAV(audioPath, &audio.spec, &audio.buffer, &audio.length) == NULL){
		//Error loading wav
		fprintf(stderr, "%s\n", SDL_GetError());
		return (Audio){0};
	}
	audio.deviceID = SDL_OpenAudioDevice(NULL, 0, &audio.spec, NULL, 0);
	if(audio.deviceID == 0){
		fprintf(stderr, "%s\n", SDL_GetError());
		return (Audio){0};
	}
	return audio;
}

void PlayAudio(Audio* audio){
	int status = SDL_QueueAudio(audio->deviceID, audio->buffer, audio->length);
	if(status < 0){
		fprintf(stderr, "%s\n", SDL_GetError());
	}
	SDL_PauseAudioDevice(audio->deviceID, 0);
}

void StopAudio(Audio* audio){
	SDL_PauseAudioDevice(audio->deviceID, 0);
}

void FreeAudio(Audio* audio){
	SDL_CloseAudioDevice(audio->deviceID);
	audio->length = 0;
	audio->spec = (SDL_AudioSpec){0};
	SDL_FreeWAV(audio->buffer);
}