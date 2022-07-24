#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "SoundManager.h"
#include "../Utilities/Log.h"
#include "../../Resources/Music/Calm1.h"
#include "../../Resources/Music/Calm2.h"
#include "../../Resources/Music/Calm3.h"
#include "../../Resources/Sound/Step/Grass1.h"
#include "../../Resources/Sound/Step/Grass2.h"
#include "../../Resources/Sound/Step/Grass3.h"
#include "../../Resources/Sound/Step/Grass4.h"
#include "../../Resources/Sound/Step/Gravel1.h"
#include "../../Resources/Sound/Step/Gravel2.h"
#include "../../Resources/Sound/Step/Gravel3.h"
#include "../../Resources/Sound/Step/Gravel4.h"
#include "../../Resources/Sound/Step/Stone1.h"
#include "../../Resources/Sound/Step/Stone2.h"
#include "../../Resources/Sound/Step/Stone3.h"
#include "../../Resources/Sound/Step/Stone4.h"
#include "../../Resources/Sound/Step/Wood1.h"
#include "../../Resources/Sound/Step/Wood2.h"
#include "../../Resources/Sound/Step/Wood3.h"
#include "../../Resources/Sound/Step/Wood4.h"

void SoundManagerCreate(SoundManager * manager) {
	*manager = (SoundManager){
		.lastMusic = 0,
	};
	RandomGeneratorCreate(&manager->random, time(NULL));
	
	manager->calm[0] = cs_read_mem_ogg((void *)Resource_Music_Calm1, sizeof(Resource_Music_Calm1), NULL);
	manager->calm[1] = cs_read_mem_ogg((void *)Resource_Music_Calm2, sizeof(Resource_Music_Calm2), NULL);
	manager->calm[2] = cs_read_mem_ogg((void *)Resource_Music_Calm3, sizeof(Resource_Music_Calm3), NULL);
	manager->grass[0] = cs_read_mem_ogg((void *)Resource_Sound_Step_Grass1, sizeof(Resource_Sound_Step_Grass1), NULL);
	manager->grass[1] = cs_read_mem_ogg((void *)Resource_Sound_Step_Grass2, sizeof(Resource_Sound_Step_Grass2), NULL);
	manager->grass[2] = cs_read_mem_ogg((void *)Resource_Sound_Step_Grass3, sizeof(Resource_Sound_Step_Grass3), NULL);
	manager->grass[3] = cs_read_mem_ogg((void *)Resource_Sound_Step_Grass4, sizeof(Resource_Sound_Step_Grass4), NULL);
	manager->gravel[0] = cs_read_mem_ogg((void *)Resource_Sound_Step_Gravel1, sizeof(Resource_Sound_Step_Gravel1), NULL);
	manager->gravel[1] = cs_read_mem_ogg((void *)Resource_Sound_Step_Gravel2, sizeof(Resource_Sound_Step_Gravel2), NULL);
	manager->gravel[2] = cs_read_mem_ogg((void *)Resource_Sound_Step_Gravel3, sizeof(Resource_Sound_Step_Gravel3), NULL);
	manager->gravel[3] = cs_read_mem_ogg((void *)Resource_Sound_Step_Gravel4, sizeof(Resource_Sound_Step_Gravel4), NULL);
	manager->stone[0] = cs_read_mem_ogg((void *)Resource_Sound_Step_Stone1, sizeof(Resource_Sound_Step_Stone1), NULL);
	manager->stone[1] = cs_read_mem_ogg((void *)Resource_Sound_Step_Stone2, sizeof(Resource_Sound_Step_Stone2), NULL);
	manager->stone[2] = cs_read_mem_ogg((void *)Resource_Sound_Step_Stone3, sizeof(Resource_Sound_Step_Stone3), NULL);
	manager->stone[3] = cs_read_mem_ogg((void *)Resource_Sound_Step_Stone4, sizeof(Resource_Sound_Step_Stone4), NULL);
	manager->wood[0] = cs_read_mem_ogg((void *)Resource_Sound_Step_Wood1, sizeof(Resource_Sound_Step_Wood1), NULL);
	manager->wood[1] = cs_read_mem_ogg((void *)Resource_Sound_Step_Wood2, sizeof(Resource_Sound_Step_Wood2), NULL);
	manager->wood[2] = cs_read_mem_ogg((void *)Resource_Sound_Step_Wood3, sizeof(Resource_Sound_Step_Wood3), NULL);
	manager->wood[3] = cs_read_mem_ogg((void *)Resource_Sound_Step_Wood4, sizeof(Resource_Sound_Step_Wood4), NULL);
}

void SoundManagerPlayMusic(SoundManager * manager, char * music) {
	if (strcmp(music, "Calm") == 0) {
		cs_music_play(manager->calm[RandomGeneratorIntegerRange(&manager->random, 0, 2)], 0.0);
	}
}

void SoundManagerPlaySound(SoundManager * manager, char * sound, float volume, float pitch) {
	if (strcmp(sound, "Grass") == 0) {
		cs_play_sound(manager->grass[RandomGeneratorIntegerRange(&manager->random, 0, 3)], (cs_sound_params_t){ .pan = 0.5, .volume = volume });
	}
	if (strcmp(sound, "Gravel") == 0) {
		cs_play_sound(manager->gravel[RandomGeneratorIntegerRange(&manager->random, 0, 3)], (cs_sound_params_t){ .pan = 0.5, .volume = volume });
	}
	if (strcmp(sound, "Stone") == 0) {
		cs_play_sound(manager->stone[RandomGeneratorIntegerRange(&manager->random, 0, 3)], (cs_sound_params_t){ .pan = 0.5, .volume = volume });
	}
	if (strcmp(sound, "Wood") == 0) {
		cs_play_sound(manager->wood[RandomGeneratorIntegerRange(&manager->random, 0, 3)], (cs_sound_params_t){ .pan = 0.5, .volume = volume });
	}
}
