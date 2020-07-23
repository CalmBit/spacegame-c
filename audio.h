#ifndef _SPC_AUDIO_H
#define _SPC_AUDIO_H

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include <stdbool.h>

#define SPC_SEND_SLOT_COUNT 4
#define SPC_EFFECT_SLOT_COUNT 4

#define SPC_AUXEFF_SLOT_REVERB 0
#define SPC_AUXEFF_SLOT_UNUSED_1 1
#define SPC_AUXEFF_SLOT_UNUSED_2 2
#define SPC_AUXEFF_SLOT_UNUSED_3 3

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

typedef struct audio_effect_slot_t {
    ALuint id;
    bool occupied;
} audio_effect_slot_t;

typedef struct audio_reverb_t {
    ALuint id;
    ALfloat density;
    ALfloat diffusion;
    ALfloat gain;
    ALfloat decay;
    ALfloat reflection_gain;
    ALfloat reflection_delay;
    ALfloat late_gain;
    ALfloat late_delay;
    ALfloat rolloff_factor;
    int slot_id;
} audio_reverb_t;

typedef struct audio_system_t {
    ALCdevice* device;
    ALCcontext* context;
    bool has_efx;
    ALCint send_count;
    audio_effect_slot_t effect_slots[4];
} audio_system_t;

static audio_system_t* primary_audio;

typedef struct audio_listener_t {
    ALfloat gain;
    ALfloat pos[3];
    ALfloat vel[3];
    ALfloat orientation[6];
} audio_listener_t;

typedef struct audio_src_t {
    ALuint src;
    ALuint buffer;
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

void audio_slot_reverb(audio_reverb_t* reverb);
void audio_unslot_reverb(audio_reverb_t* reverb);

audio_src_t* audio_src_create(void);
void audio_src_destroy(audio_src_t* src);

audio_reverb_t* audio_reverb_create(void);
void audio_reverb_destroy(audio_reverb_t* reverb);

void audio_reverb_set_density(audio_reverb_t* reverb, ALfloat density);
void audio_reverb_set_diffusion(audio_reverb_t* reverb, ALfloat diffusion);
void audio_reverb_set_gain(audio_reverb_t* reverb, ALfloat gain);
void audio_reverb_set_decay(audio_reverb_t* reverb, ALfloat decay);
void audio_reverb_set_rgain(audio_reverb_t* reverb, ALfloat rgain);
void audio_reverb_set_rdelay(audio_reverb_t* reverb, ALfloat rdelay);
void audio_reverb_set_lgain(audio_reverb_t* reverb, ALfloat lgain);
void audio_reverb_set_ldelay(audio_reverb_t* reverb, ALfloat ldelay);
void audio_reverb_set_rolloff_factor(audio_reverb_t* reverb, ALfloat rolloff_factor);

void audio_check_error(const char* fmt);
void audio_context_check_error(const char* fmt);

extern LPALGENEFFECTS alGenEffects;
extern LPALDELETEEFFECTS alDeleteEffects;
extern LPALISEFFECT alIsEffect;
extern LPALGENAUXILIARYEFFECTSLOTS alGenAuxiliaryEffectSlots;
extern LPALDELETEAUXILIARYEFFECTSLOTS alDeleteAuxiliaryEffectSlots;
extern LPALISAUXILIARYEFFECTSLOT alIsAuxiliaryEffectSlot;
extern LPALAUXILIARYEFFECTSLOTI alAuxiliaryEffectSloti;
extern LPALAUXILIARYEFFECTSLOTIV alAuxiliaryEffectSlotiv;
extern LPALAUXILIARYEFFECTSLOTF alAuxiliaryEffectSlotf;
extern LPALAUXILIARYEFFECTSLOTFV alAuxiliaryEffectSlotfv;
extern LPALEFFECTI alEffecti;
extern LPALEFFECTF alEffectf;

#endif
