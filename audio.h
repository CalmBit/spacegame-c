#ifndef _SPC_AUDIO_H
#define _SPC_AUDIO_H

#include <AL/al.h>
#include <AL/alc.h>


static const char* al_error_codes[] = {
    "AL_NO_ERROR",
    "AL_INVALID_NAME",
    "AL_INVALID_ENUM",
    "AL_INVALID_OPERATION",
    "AL_OUT_OF_MEMORY"
};


static const char* alc_error_codes[] = {
    "ALC_NO_ERROR",
    "ALC_INVALID_DEVICE",
    "ALC_INVALID_CONTEXT",
    "ALC_INVALID_ENUM",
    "ALC_INVALID_VALUE",
    "ALC_OUT_OF_MEMORY"
};

typedef struct audio_system_t {
    ALCdevice* device;
    ALCcontext* context;
} audio_system_t;

static audio_system_t* primary_audio;

typedef struct audio_src_t {
    ALuint src;
    ALfloat pitch;
    ALfloat gain;
    ALfloat pos[3];
    ALfloat vel[3];
    ALboolean looping;
} audio_src_t;

void audio_init(void);
void audio_cleanup(void);

audio_system_t* audio_create(void);
void audio_destroy(audio_system_t* aud);
const char* audio_alerror(ALenum code);
const char* audio_alcerror(ALCenum code);

audio_src_t* audio_src_create(void);
void audio_src_destroy(audio_src_t* src);

#endif
