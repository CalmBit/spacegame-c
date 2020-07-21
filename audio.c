#include "audio.h"

#include "memory.h"
#include "error.h"

ALenum last_al_error;
ALCenum last_alc_error;

void audio_init(void) {
    primary_audio = audio_create();
}

void audio_cleanup(void) {
    if(primary_audio != NULL) {
        audio_destroy(primary_audio);
    }
}

audio_system_t* audio_create(void) {
    audio_system_t* aud;
        
    aud = (audio_system_t*)memory_alloc(SPC_MU_SOUND, sizeof(audio_system_t));
    aud->device = alcOpenDevice(NULL);
    if(!aud->device) {
        error("unable to open default audio device");
    }
    aud->context = alcCreateContext(aud->device, NULL);
    if(!aud->context) {
        error("unable to create audio context: %s",
        audio_alcerror(alcGetError(aud->device)));
    }
    if(!alcMakeContextCurrent(aud->context)) {
        error("unable to make OpenAL context current: %s",
         audio_alcerror(alcGetError(aud->device)));
    }

    return aud;
}

void audio_destroy(audio_system_t* aud) {
    alcDestroyContext(aud->context);
    last_alc_error = alcGetError(aud->device);
    if(last_alc_error != ALC_NO_ERROR) {
        error("encountered trouble when ending audio context: %s",
                audio_alcerror(last_alc_error));
    }
    alcCloseDevice(aud->device);
        last_alc_error = alcGetError(aud->device);
    if(last_alc_error != ALC_NO_ERROR) {
        error("encountered trouble when closing audio device: %s",
                audio_alcerror(last_alc_error));
    }
    memory_free(aud);
}

const char* audio_alerror(ALenum code) {
    if(code > 0)
        return al_error_codes[code - 0xA000];
    return al_error_codes[code];
}

const char* audio_alcerror(ALCenum code) {
    if(code > 0)
        return alc_error_codes[code - 0xA000];
    return alc_error_codes[code];
}


audio_src_t* audio_src_create(void) {
    audio_src_t* src;
    if(primary_audio == NULL) {
        error("tried to create audio source with NULL audio cntxt");
    }

    src = (audio_src_t*)memory_alloc(SPC_MU_SOUND, sizeof(audio_src_t));
    alGenSources(1, &src->src);
    last_al_error = alGetError();
    if(last_al_error != AL_NO_ERROR) {
        error("unable to create source: %s", audio_alerror(last_al_error));
    }

    src->pitch = 1;
    alSourcef(src->src, AL_PITCH, src->pitch);
    last_al_error = alGetError();
    if(last_al_error != AL_NO_ERROR) {
        error("unable to set source pitch: %s", audio_alerror(last_al_error));
    }

    src->gain = 1;
    alSourcef(src->src, AL_GAIN, src->gain);
    last_al_error = alGetError();
    if(last_al_error != AL_NO_ERROR) {
        error("unable to set source gain: %s", audio_alerror(last_al_error));
    }

    src->pos[0] = src->pos[1] = src->pos[2] = 0.0f;
    alSource3f(src->src, AL_POSITION, src->pos[0], src->pos[1], src->pos[2]);
    last_al_error = alGetError();
    if(last_al_error != AL_NO_ERROR) {
        error("unable to set source pos: %s", audio_alerror(last_al_error));
    }

    src->vel[0] = src->vel[1] = src->vel[2] = 0.0f;
    alSource3f(src->src, AL_VELOCITY, src->vel[0], src->vel[1], src->vel[2]);
    last_al_error = alGetError();
    if(last_al_error != AL_NO_ERROR) {
        error("unable to set source vel: %s", audio_alerror(last_al_error));
    }

    src->looping = AL_FALSE;
    alSourcei(src->src, AL_LOOPING, src->looping);
    last_al_error = alGetError();
    if(last_al_error != AL_NO_ERROR) {
        error("unable to set source loop: %s", audio_alerror(last_al_error));
    }

    return src;
}

void audio_src_destroy(audio_src_t* src) {
    alDeleteSources(1, &src->src);    
    last_al_error = alGetError();
    if(last_al_error != AL_NO_ERROR) {
        error("unable to set source loop: %s", audio_alerror(last_al_error));
    }
    memory_free((void*)src);
}
