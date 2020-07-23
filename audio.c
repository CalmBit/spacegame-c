#include "audio.h"

#include "memory.h"
#include "error.h"
#include "wav.h"


#include <AL/alc.h>
#include <AL/alext.h>

#include <stdio.h>

LPALGENEFFECTS alGenEffects = NULL;
LPALDELETEEFFECTS alDeleteEffects = NULL;
LPALISEFFECT alIsEffect = NULL;
LPALGENAUXILIARYEFFECTSLOTS alGenAuxiliaryEffectSlots = NULL;
LPALDELETEAUXILIARYEFFECTSLOTS alDeleteAuxiliaryEffectSlots = NULL;
LPALISAUXILIARYEFFECTSLOT alIsAuxiliaryEffectSlot = NULL;
LPALAUXILIARYEFFECTSLOTI alAuxiliaryEffectSloti = NULL;
LPALAUXILIARYEFFECTSLOTIV alAuxiliaryEffectSlotiv = NULL;
LPALAUXILIARYEFFECTSLOTF alAuxiliaryEffectSlotf = NULL;
LPALAUXILIARYEFFECTSLOTFV alAuxiliaryEffectSlotfv = NULL;
LPALEFFECTI alEffecti = NULL;
LPALEFFECTF alEffectf = NULL;

ALenum last_al_error;
ALCenum last_alc_error;

static void _audio_initialize_alext(void);

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
    ALuint i;
    ALint attribs[4] = {0};
        
    aud = (audio_system_t*)memory_alloc(SPC_MU_SOUND, sizeof(audio_system_t));
    aud->device = alcOpenDevice(NULL);
    if(!aud->device) {
        error("unable to open default audio device");
    }

    if(alcIsExtensionPresent(aud->device, "ALC_EXT_EFX") == AL_FALSE) {
        warning("unable to initialize EFX extension - continuing w/o EFX");
        aud->has_efx = false;
    } else {
        aud->has_efx = true;
        attribs[0] = ALC_MAX_AUXILIARY_SENDS;
        attribs[1] = SPC_SEND_SLOT_COUNT;
    }

    aud->context = alcCreateContext(aud->device, aud->has_efx ? attribs : NULL);
    
    if(!aud->context) {
        error("unable to create audio context: %s",
        audio_alcerror(alcGetError(aud->device)));
    }
    
    if(!alcMakeContextCurrent(aud->context)) {
        error("unable to make OpenAL context current: %s",
         audio_alcerror(alcGetError(aud->device)));
    }

    if(aud->has_efx) {
        alcGetIntegerv(aud->device, ALC_MAX_AUXILIARY_SENDS, 1, &aud->send_count);
        if(aud->send_count < SPC_SEND_SLOT_COUNT) {
            error("reservation of %i send slots was unsuccessful, got %i", 
                    SPC_SEND_SLOT_COUNT, aud->send_count);
        }
        _audio_initialize_alext();

        alGetError();
        for(i = 0;i < SPC_EFFECT_SLOT_COUNT;i++) {
            alGenAuxiliaryEffectSlots(1, &aud->effect_slots[i].id);
            aud->effect_slots[i].occupied = false;
            audio_check_error("unable to create effect slots");
        }
    } else {
        aud->send_count = 0;
    }

    return aud;
}

void audio_destroy(audio_system_t* aud) {
    alcMakeContextCurrent(NULL);

    alcDestroyContext(aud->context);
    audio_context_check_error("encountered trouble when closing audio context");

    alcCloseDevice(aud->device);
    audio_context_check_error("encountered trouble when closing audio device");

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

void audio_slot_reverb(audio_reverb_t* reverb) {
    size_t i;
    if(!primary_audio) {
        error("tried to slot reverb effect with NULL audio cntxt");
    }

    if(reverb->slot_id != -1) {
        error("tried to double slot reverb");
    }

    for(i = 0;i < SPC_EFFECT_SLOT_COUNT;i++) {
        if(!primary_audio->effect_slots[i].occupied) {
            alAuxiliaryEffectSloti(primary_audio->effect_slots[i].id, 
                    AL_EFFECTSLOT_EFFECT,
                    reverb->id);
            audio_check_error("unable to slot reverb");
            primary_audio->effect_slots[i].occupied = true;
            reverb->slot_id = i;
            return;
        }
    }

    error("unable to find open effect slot!!!");
}

void audio_unslot_reverb(audio_reverb_t* reverb) {
    if(!primary_audio) {
        error("tried to unslot reverb effect with NULL audio cntxt");
    }

    if(reverb->slot_id == -1) {
        error("tried to unslot free reverb");
    }

    alAuxiliaryEffectSloti(primary_audio->effect_slots[reverb->slot_id].id, 
        AL_EFFECTSLOT_EFFECT,
        AL_EFFECT_NULL);
    audio_check_error("unable to unslot reverb");
    reverb->slot_id = -1;
}



audio_src_t* audio_src_create(void) {
    audio_src_t* src;
    if(!primary_audio) {
        error("tried to create audio source with NULL audio cntxt");
    }

    src = (audio_src_t*)memory_alloc(SPC_MU_SOUND, sizeof(audio_src_t));
    alGenSources(1, &src->src);
    audio_check_error("unable to generate source");

    src->pitch = 1;
    alSourcef(src->src, AL_PITCH, src->pitch);
    audio_check_error("unable to set source 'AL_PITCH'");

    src->gain = 1;
    alSourcef(src->src, AL_GAIN, src->gain);
    audio_check_error("unable to set source 'AL_GAIN'");

    src->pos[0] = src->pos[1] = src->pos[2] = 0.0f;
    alSource3f(src->src, AL_POSITION, src->pos[0], src->pos[1], src->pos[2]);
    audio_check_error("unable to set source 'AL_POSITION'");

    src->vel[0] = src->vel[1] = src->vel[2] = 0.0f;
    alSource3f(src->src, AL_VELOCITY, src->vel[0], src->vel[1], src->vel[2]);
    audio_check_error("unable to set source 'AL_VELOCITY'");

    src->looping = AL_FALSE;
    alSourcei(src->src, AL_LOOPING, src->looping);
    audio_check_error("unable to set source 'AL_LOOPING'");

    if(primary_audio->has_efx) {
        alSource3i(src->src, AL_AUXILIARY_SEND_FILTER, 
                    primary_audio->effect_slots[0].id,
                    0, 0);
        audio_check_error("unable to connect source to effect");
    }

    alGenBuffers(1, &src->buffer);
    audio_check_error("unable to generate audio buffer");

    wav_t* wav = wav_load("schut_lq.wav");

    alBufferData(src->buffer, wav_get_format(wav), 
                    wav->data.buffer, 
                    wav->data.len, 
                    wav->sample_rate);
    audio_check_error("unable to bind data to audio buffer");


    alSourcei(src->src, AL_BUFFER, src->buffer);
    audio_check_error("unable to set source buffer");


    alSourcePlay(src->src);

    wav_destroy(wav);

    return src;
}

void audio_src_destroy(audio_src_t* src) {
    alDeleteSources(1, &src->src);
    audio_check_error("unable to delete source");
    alDeleteBuffers(1, &src->buffer);
    audio_check_error("unable to delete buffer");
    memory_free((void*)src);
}

audio_reverb_t* audio_reverb_create(void) {
    audio_reverb_t* reverb;

    if(primary_audio == NULL) {
        error("tried to create reverb effect with NULL audio cntxt");
    }

    reverb = memory_alloc(SPC_MU_SOUND, sizeof(audio_reverb_t));

    alGenEffects(1, &reverb->id);
    audio_check_error("unable to generate effect");


    // default values according to OpenAL specification
    // we don't need to re-set these, as they're 
    // already the standard.
    reverb->density = 1.0f;
    reverb->diffusion = 1.0f;
    reverb->gain = 0.32f;
    reverb->decay = 1.49f;
    reverb->reflection_gain = 0.05f;
    reverb->reflection_gain = 0.007f;
    reverb->late_gain = 1.26f;
    reverb->late_delay = 0.011f;
    reverb->rolloff_factor = 0.0f;
    reverb->slot_id = -1;

    alEffecti(reverb->id, AL_EFFECT_TYPE, AL_EFFECT_REVERB);
    audio_check_error("unable to set effect type");


    return reverb;
}

void audio_reverb_destroy(audio_reverb_t* reverb) {
    if(reverb->slot_id != -1) {
        audio_unslot_reverb(reverb);
    }
    alDeleteEffects(1, &reverb->id);
    memory_free(reverb);
}

void audio_reverb_set_density(audio_reverb_t* reverb, ALfloat density) {
    if(density < 0.0f || density > 1.0f) {
        warning("invalid reverb density value %f - ignoring", density);
        return;
    }
    reverb->density = density;
    
    alEffectf(reverb->id, AL_REVERB_DENSITY, reverb->density);
    audio_check_error("unable to set reverb density");
}

void audio_reverb_set_diffusion(audio_reverb_t* reverb, ALfloat diffusion) {
    if(diffusion < 0.0f || diffusion > 1.0f) {
        warning("invalid reverb diffusion value %f - ignoring", diffusion);
        return;
    }
    reverb->diffusion = diffusion;
    
    alEffectf(reverb->id, AL_REVERB_DIFFUSION, reverb->diffusion);
    audio_check_error("unable to set reverb diffusion");
}

void audio_reverb_set_gain(audio_reverb_t* reverb, ALfloat gain) {
    if(gain < 0.0f || gain > 1.0f) {
        warning("invalid reverb gain value %f - ignoring", gain);
        return;
    }
    reverb->gain = gain;
    
    alEffectf(reverb->id, AL_REVERB_GAIN, reverb->gain);
    audio_check_error("unable to set reverb gain");
}

void audio_reverb_set_decay(audio_reverb_t* reverb, ALfloat decay) {
    if(decay < 0.0f || decay > 20.0f) {
        warning("invalid reverb decay value %f - ignoring", decay);
        return;
    }
    reverb->decay = decay;
    
    alEffectf(reverb->id, AL_REVERB_DECAY_TIME, reverb->decay);
    audio_check_error("unable to set reverb decay");
}

void audio_reverb_set_rgain(audio_reverb_t* reverb, ALfloat rgain) {
    if(rgain < 0.0f || rgain > 3.16f) {
        warning("invalid reverb rgain value %f - ignoring", rgain);
        return;
    }
    reverb->reflection_gain = rgain;
    
    alEffectf(reverb->id, AL_REVERB_REFLECTIONS_GAIN, reverb->reflection_gain);
    audio_check_error("unable to set reverb rgain");
}

void audio_reverb_set_rdelay(audio_reverb_t* reverb, ALfloat rdelay) {
    if(rdelay < 0.0f || rdelay > 0.3f) {
        warning("invalid reverb rdelay value %f - ignoring", rdelay);
        return;
    }
    reverb->reflection_delay = rdelay;
    
    alEffectf(reverb->id, AL_REVERB_REFLECTIONS_DELAY, reverb->reflection_delay);
    audio_check_error("unable to set reverb rdelay");
}

void audio_reverb_set_lgain(audio_reverb_t* reverb, ALfloat lgain) {
    if(lgain < 0.0f || lgain > 10.0f) {
        warning("invalid reverb lgain value %f - ignoring", lgain);
        return;
    }
    reverb->late_gain = lgain;
    
    alEffectf(reverb->id, AL_REVERB_LATE_REVERB_GAIN, reverb->late_gain);
    audio_check_error("unable to set reverb lgain");
}

void audio_reverb_set_ldelay(audio_reverb_t* reverb, ALfloat ldelay) {
    if(ldelay < 0.0f || ldelay > 0.1f) {
        warning("invalid reverb ldelay value %f - ignoring", ldelay);
        return;
    }
    reverb->late_delay = ldelay;
    
    alEffectf(reverb->id, AL_REVERB_LATE_REVERB_DELAY, reverb->late_delay);
    audio_check_error("unable to set reverb ldelay");
}

void audio_reverb_set_rolloff_factor(audio_reverb_t* reverb, ALfloat rolloff_factor) {
    if(rolloff_factor < 0.0f || rolloff_factor > 10.0f) {
        warning("invalid reverb rolloff_factor value %f - ignoring", rolloff_factor);
        return;
    }
    reverb->rolloff_factor = rolloff_factor;
    
    alEffectf(reverb->id, AL_REVERB_ROOM_ROLLOFF_FACTOR, reverb->rolloff_factor);
    audio_check_error("unable to set reverb rolloff_factor"); 
}


static void _audio_initialize_alext(void) {
    alGenEffects = (LPALGENEFFECTS)alGetProcAddress("alGenEffects");
    if(!alGenEffects) {
        error("unable to get proc of 'alGenEffects'");
    }
    alDeleteEffects = (LPALDELETEEFFECTS)alGetProcAddress("alDeleteEffects");
    if(!alDeleteEffects) {
        error("unable to get proc of 'alDeleteEffects'");
    }
    alIsEffect = (LPALISEFFECT)alGetProcAddress("alIsEffect");
    if(!alIsEffect) {
        error("unable to get proc of 'alIsEffect'");
    }

    alGenAuxiliaryEffectSlots = (LPALGENAUXILIARYEFFECTSLOTS)alGetProcAddress("alGenAuxiliaryEffectSlots");
    if(!alGenAuxiliaryEffectSlots) {
        error("unable to get proc of 'alGenAuxiliaryEffectSlots'");
    }
    alDeleteAuxiliaryEffectSlots = (LPALDELETEAUXILIARYEFFECTSLOTS)alGetProcAddress("alDeleteAuxiliaryEffectSlots");
    if(!alDeleteAuxiliaryEffectSlots) {
        error("unable to get proc of 'alDeleteAuxiliaryEffectSlots'");
    }
    alIsAuxiliaryEffectSlot = (LPALISAUXILIARYEFFECTSLOT)alGetProcAddress("alIsAuxiliaryEffectSlot");
    if(!alIsAuxiliaryEffectSlot) {
        error("unable to get proc of 'alIsAuxiliaryEffectSlot'");
    }

    alAuxiliaryEffectSloti = (LPALAUXILIARYEFFECTSLOTI)alGetProcAddress("alAuxiliaryEffectSloti");
    if(!alAuxiliaryEffectSloti) {
        error("unable to get proc of 'alAuxiliaryEffectSloti'");
    }
    alAuxiliaryEffectSlotiv = (LPALAUXILIARYEFFECTSLOTIV)alGetProcAddress("alAuxiliaryEffectSlotiv");
    if(!alAuxiliaryEffectSlotiv) {
        error("unable to get proc of 'alAuxiliaryEffectSlotiv'");
    }
    alAuxiliaryEffectSlotf = (LPALAUXILIARYEFFECTSLOTF)alGetProcAddress("alAuxiliaryEffectSlotf");
    if(!alAuxiliaryEffectSlotf) {
        error("unable to get proc of 'alAuxiliaryEffectSlotf'");
    }
    alAuxiliaryEffectSlotfv = (LPALAUXILIARYEFFECTSLOTFV)alGetProcAddress("alAuxiliaryEffectSlotfv");
    if(!alAuxiliaryEffectSlotfv) {
        error("unable to get proc of 'alAuxiliaryEffectSlotfv'");
    }

    alEffecti = (LPALEFFECTI)alGetProcAddress("alEffecti");
    if(!alEffecti) {
        error("unable to get proc of 'alEffecti");
    }
    alEffectf = (LPALEFFECTF)alGetProcAddress("alEffectf");
    if(!alEffectf) {
        error("unable to get proc of 'alEffectf");
    }
}

void audio_check_error(const char* fmt) {
    last_al_error = alGetError();
    if(last_al_error != AL_NO_ERROR) {
        error("%s: %s", fmt, audio_alerror(last_al_error));
    }
}

void audio_context_check_error(const char* fmt) {
    last_alc_error = alcGetError(primary_audio->device);
    if(last_alc_error != ALC_NO_ERROR) {
        error("%s: %s", fmt, audio_alcerror(last_alc_error));
    }
}