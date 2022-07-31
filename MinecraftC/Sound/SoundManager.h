#pragma once
#include "../Utilities/Random.h"
#define STB_VORBIS_HEADER_ONLY
#include <stb_vorbis.c>
#include <cute_sound.h>

typedef struct SoundManager {
	uint64_t lastMusic;
	RandomGenerator random;
	cs_audio_source_t * calm[3];
	cs_audio_source_t * grass[4];
	cs_audio_source_t * gravel[4];
	cs_audio_source_t * stone[4];
	cs_audio_source_t * wood[4];
	float volume[4];
	float pitch[4];
} SoundManager;

void SoundManagerCreate(SoundManager * manager);
void SoundManagerPlayMusic(SoundManager * manager, char * music);
void SoundManagerPlaySound(SoundManager * manager, char * sound, float volume, float pitch);
