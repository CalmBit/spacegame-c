#ifndef _SPC_AUDIO_H
#define _SPC_AUDIO_H

#include <al.h>
#include <alc.h>
#include <alext.h>

#include <stdbool.h>

//===========================================================================//
//******* AUDIO SUBSYSTEM ***************************************************//
// Creates a wrapper around underlying OpenAL functions and attempts to      //
// track some of the state underlying the calls.                             //
//---------------------------------------------------------------------------//
// The general rule for this entire subsystem is to *use the functions*.     //
// If the functions aren't used, and either OpenAL calls are used directly,  //
// or the state of the associated objects are changed, we cannot keep these  //
// two in sync. Therefore, ALWAYS ALWAYS ALWAYS use the functions provided.  //
//---------------------------------------------------------------------------//
// File: audio.h                                                             //
// Date: 2020-07-23                                                          //
// Author: CalmBit <calmbit@posteo.net>                                      //
//===========================================================================//

#define SPC_SEND_SLOT_COUNT 4
#define SPC_EFFECT_SLOT_COUNT 4

typedef struct audio_reverb_t audio_reverb_t;
typedef struct audio_src_t audio_src_t;
typedef struct audio_listener_t audio_listener_t;
typedef struct audio_effect_slot_t audio_effect_slot_t;
typedef struct audio_system_t audio_system_t;

#define SPC_AUXEFF_SLOT_REVERB 0
#define SPC_AUXEFF_SLOT_UNUSED_1 1
#define SPC_AUXEFF_SLOT_UNUSED_2 2
#define SPC_AUXEFF_SLOT_UNUSED_3 3

void audio_init(void);

void audio_cleanup(void);

audio_system_t *audio_create(void);

void audio_destroy(audio_system_t *aud);


audio_src_t *audio_src_create(void);

void audio_src_destroy(audio_src_t *src);

void audio_src_set_pitch(audio_src_t *src, ALfloat pitch);

void audio_src_set_gain(audio_src_t *src, ALfloat gain);

void audio_src_set_pos(audio_src_t *src, ALfloat x, ALfloat y, ALfloat z);

void audio_src_set_vel(audio_src_t *src, ALfloat x, ALfloat y, ALfloat z);

void audio_src_set_looping(audio_src_t *src, bool looping);


audio_reverb_t *audio_reverb_create(void);

void audio_reverb_destroy(audio_reverb_t *reverb);

void audio_reverb_set_density(audio_reverb_t *reverb, ALfloat density);

void audio_reverb_set_diffusion(audio_reverb_t *reverb, ALfloat diffusion);

void audio_reverb_set_gain(audio_reverb_t *reverb, ALfloat gain);

void audio_reverb_set_decay(audio_reverb_t *reverb, ALfloat decay);

void audio_reverb_set_rgain(audio_reverb_t *reverb, ALfloat rgain);

void audio_reverb_set_rdelay(audio_reverb_t *reverb, ALfloat rdelay);

void audio_reverb_set_lgain(audio_reverb_t *reverb, ALfloat lgain);

void audio_reverb_set_ldelay(audio_reverb_t *reverb, ALfloat ldelay);

void
audio_reverb_set_rolloff_factor(audio_reverb_t *reverb, ALfloat rolloff_factor);

void audio_slot_reverb(audio_reverb_t *reverb);

void audio_unslot_reverb(audio_reverb_t *reverb);

#define alGenEffects __alGenEffects
#define alDeleteEffects __alDeleteEffects
#define alIsEffect __alIsEffect
#define alGenAuxiliaryEffectSlots __alGenAuxiliaryEffectSlots
#define alDeleteAuxiliaryEffectSlots __alDeleteAuxiliaryEffectSlots
#define alIsAuxiliaryEffectSlot __alIsAuxiliaryEffectSlot
#define alAuxiliaryEffectSloti __alAuxiliaryEffectSloti
#define alAuxiliaryEffectSlotiv __alAuxiliaryEffectSlotiv
#define alAuxiliaryEffectSlotf __alAuxiliaryEffectSlotf
#define alAuxiliaryEffectSlotfv __alAuxiliaryEffectSlotfv
#define alEffecti __alEffecti
#define alEffectf __alEffectf

extern LPALGENEFFECTS __alGenEffects;
extern LPALDELETEEFFECTS __alDeleteEffects;
extern LPALISEFFECT __alIsEffect;
extern LPALGENAUXILIARYEFFECTSLOTS __alGenAuxiliaryEffectSlots;
extern LPALDELETEAUXILIARYEFFECTSLOTS __alDeleteAuxiliaryEffectSlots;
extern LPALISAUXILIARYEFFECTSLOT __alIsAuxiliaryEffectSlot;
extern LPALAUXILIARYEFFECTSLOTI __alAuxiliaryEffectSloti;
extern LPALAUXILIARYEFFECTSLOTIV __alAuxiliaryEffectSlotiv;
extern LPALAUXILIARYEFFECTSLOTF __alAuxiliaryEffectSlotf;
extern LPALAUXILIARYEFFECTSLOTFV __alAuxiliaryEffectSlotfv;
extern LPALEFFECTI __alEffecti;
extern LPALEFFECTF __alEffectf;

const char *audio_alerror(ALenum code);

const char *audio_alcerror(ALCenum code);

#endif
